/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#include "IGraphicsMac.h"

#include "IControl.h"
#include "IPopupMenuControl.h"

#import "IGraphicsMac_view.h"

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

int GetSystemVersion()
{
  static int32_t v;
  if (!v)
  {
    if (NSAppKitVersionNumber >= 1266.0)
    {
      if (NSAppKitVersionNumber >= 1404.0)
        v = 0x10b0;
      else
        v = 0x10a0; // 10.10+ Gestalt(gsv) return 0x109x, so we bump this to 0x10a0
    }
    else
    {
      SInt32 a = 0x1040;
      Gestalt(gestaltSystemVersion,&a);
      v=a;
    }
  }
  return v;
}

//#define IGRAPHICS_MAC_BLIT_BENCHMARK
//#define IGRAPHICS_MAC_OLD_IMAGE_DRAWING

#ifdef IGRAPHICS_MAC_BLIT_BENCHMARK
#include <sys/time.h>
static double gettm()
{
  struct timeval tm={0,};
  gettimeofday(&tm,NULL);
  return (double)tm.tv_sec + (double)tm.tv_usec/1000000;
}
#endif

template <class T>
struct CFLocal
{
  CFLocal(T obj) : mObject(obj) {}
  ~CFLocal() { if (mObject) CFRelease(mObject); }
  
  T Get() { return mObject; }
  
  T Release()
  {
    T prev = mObject;
    mObject = nullptr;
    return prev;
  }
  
  T mObject;
};

// Fonts

IGraphicsMac::MacFont::~MacFont()
{
  CGDataProviderRelease(mProvider);
  if (mDescriptor)
    CFRelease(mDescriptor);
};

IFontDataPtr IGraphicsMac::MacFont::GetFontData()
{
  char styleCString[64];
  
  CFLocal<CFDataRef> rawData = CGDataProviderCopyData(mProvider);
  const UInt8* bytes = CFDataGetBytePtr(rawData.Get());
  CFLocal<CFStringRef> styleString = (CFStringRef) CTFontDescriptorCopyAttribute(mDescriptor, kCTFontStyleNameAttribute);
  CFStringGetCString(styleString.Get(), styleCString, 64, kCFStringEncodingUTF8);
  IFontDataPtr fontData(new IFontData(bytes, static_cast<int>(CFDataGetLength(rawData.Get())), GetFaceIdx(bytes, static_cast<int>(CFDataGetLength(rawData.Get())), styleCString)));
  
  return fontData;
}

struct MacFontDescriptor
{
  MacFontDescriptor(CTFontDescriptorRef descriptor) : mDescriptor(descriptor)
  {
    CFRetain(mDescriptor);
  }
  
  ~MacFontDescriptor()
  {
    CFRelease(mDescriptor);
  }
  
  CTFontDescriptorRef mDescriptor;
};

static StaticStorage<MacFontDescriptor> sFontDescriptorCache;
  
#pragma mark -

IGraphicsMac::IGraphicsMac(IGEditorDelegate& dlg, int w, int h, int fps, float scale)
: IGRAPHICS_DRAW_CLASS(dlg, w, h, fps, scale)
{
  NSApplicationLoad();
  StaticStorage<MacFontDescriptor>::Accessor storage(sFontDescriptorCache);
  storage.Retain();
}

IGraphicsMac::~IGraphicsMac()
{
  StaticStorage<MacFontDescriptor>::Accessor storage(sFontDescriptorCache);
  storage.Release();
  
  CloseWindow();
}

bool IGraphicsMac::IsSandboxed()
{
  NSString* pHomeDir = NSHomeDirectory();

  if ([pHomeDir containsString:@"Library/Containers/"])
  {
    return true;
  }
  return false;
}

IGraphics::PlatformFontPtr IGraphicsMac::LoadPlatformFont(const char* fontID, const char* fileNameOrResID)
{
   WDL_String fullPath;
  const EResourceLocation fontLocation = LocateResource(fileNameOrResID, "ttf", fullPath, GetBundleID(), nullptr);
    
  if (fontLocation == kNotFound)
    return nullptr;

  CFLocal<CFStringRef> path = CFStringCreateWithCString(NULL, fullPath.Get(), kCFStringEncodingUTF8);
  CFLocal<CFURLRef> url = CFURLCreateWithFileSystemPath(NULL, path.Get(), kCFURLPOSIXPathStyle, false);
  CFLocal<CGDataProviderRef> provider = url.Get() ? CGDataProviderCreateWithURL(url.Get()) : nullptr;
  CFLocal<CGFontRef> cgFont = CGFontCreateWithDataProvider(provider.Get());
  CFLocal<CTFontRef> ctFont = CTFontCreateWithGraphicsFont(cgFont.Get(), 0.f, NULL, NULL);
  CFLocal<CTFontDescriptorRef> descriptor = CTFontCopyFontDescriptor(ctFont.Get());
  
  if (!descriptor.Get())
    return nullptr;
  
  return PlatformFontPtr(new MacFont(descriptor.Release(), provider.Release()));
}

IGraphics::PlatformFontPtr IGraphicsMac::LoadPlatformFont(const char* fontID, const char* fontName, ETextStyle style)
{
  CFLocal<CFStringRef> fontStr = CFStringCreateWithCString(NULL, fontName, kCFStringEncodingUTF8);
  CFLocal<CFStringRef> styleStr = CFStringCreateWithCString(NULL, TextStyleString(style), kCFStringEncodingUTF8);
  
  CFStringRef keys[] = { kCTFontFamilyNameAttribute, kCTFontStyleNameAttribute };
  CFTypeRef values[] = { fontStr.Get(), styleStr.Get() };
  
  CFLocal<CFDictionaryRef> dictionary = CFDictionaryCreate(NULL, (const void**)&keys, (const void**)&values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  CFLocal<CTFontDescriptorRef> descriptor = CTFontDescriptorCreateWithAttributes(dictionary.Get());
  CFLocal<CFURLRef> url = (CFURLRef) CTFontDescriptorCopyAttribute(descriptor.Get(), kCTFontURLAttribute);
  CFLocal<CGDataProviderRef> provider = url.Get() ? CGDataProviderCreateWithURL(url.Get()) : nullptr;

  if (!provider.Get())
    return nullptr;
  
  return PlatformFontPtr(new MacFont(descriptor.Release(), provider.Release()));
}

void IGraphicsMac::CachePlatformFont(const char* fontID, const PlatformFontPtr& font)
{
  StaticStorage<MacFontDescriptor>::Accessor storage(sFontDescriptorCache);
 
  CTFontDescriptorRef descriptor = (CTFontDescriptorRef) font->GetDescriptor();
  
  if (!storage.Find(fontID))
    storage.Add(new MacFontDescriptor(descriptor), fontID);
}

bool IGraphicsMac::MeasureText(const IText& text, const char* str, IRECT& bounds)
{
#ifdef IGRAPHICS_LICE
  @autoreleasepool {
    return IGRAPHICS_DRAW_CLASS::MeasureText(text, str, bounds);
  }
#else
  return IGRAPHICS_DRAW_CLASS::MeasureText(text, str, bounds);
#endif
}

void IGraphicsMac::ContextReady(void* pLayer)
{
  OnViewInitialized(pLayer);
  SetScreenScale([[NSScreen mainScreen] backingScaleFactor]);
  GetDelegate()->LayoutUI(this);
  UpdateTooltips();
}

void* IGraphicsMac::OpenWindow(void* pParent)
{
  TRACE;
  CloseWindow();
  mView = (IGRAPHICS_VIEW*) [[IGRAPHICS_VIEW alloc] initWithIGraphics: this];
  
  IGRAPHICS_VIEW* pView = (IGRAPHICS_VIEW*) mView;
  
#ifndef IGRAPHICS_GL
  ContextReady([pView layer]);
#endif
  
  if (pParent) // Cocoa VST host.
  {
    [(NSView*) pParent addSubview: (IGRAPHICS_VIEW*) mView];
  }

  return mView;
}

void IGraphicsMac::CloseWindow()
{
  if (mView)
  {
    IGRAPHICS_VIEW* view = (IGRAPHICS_VIEW*) mView;
    [view removeAllToolTips];
    [view killTimer];
    [view removeFromSuperview];
    [view release];
      
    mView = nullptr;
    OnViewDestroyed();
  }
}

bool IGraphicsMac::WindowIsOpen()
{
  return mView;
}

void IGraphicsMac::PlatformResize()
{
  if (mView)
  {
    NSSize size = { static_cast<CGFloat>(WindowWidth()), static_cast<CGFloat>(WindowHeight()) };

    DBGMSG("%f, %f\n", size.width, size.height);

    [NSAnimationContext beginGrouping]; // Prevent animated resizing
    [[NSAnimationContext currentContext] setDuration:0.0];
    [(IGRAPHICS_VIEW*) mView setFrameSize: size ];
    [NSAnimationContext endGrouping];
  }  
}

void IGraphicsMac::PointToScreen(float& x, float& y)
{
  if (mView)
  {
    x *= GetDrawScale();
    y *= GetDrawScale();
    NSWindow* pWindow = [(IGRAPHICS_VIEW*) mView window];
    NSPoint wndpt = [(IGRAPHICS_VIEW*) mView convertPoint:NSMakePoint(x, y) toView:nil];
    NSPoint pt = [pWindow convertRectToScreen: NSMakeRect(wndpt.x, wndpt.y, 0.0, 0.0)].origin;
      
    x = pt.x;
    y = pt.y;
  }
}

void IGraphicsMac::ScreenToPoint(float& x, float& y)
{
  if (mView)
  {
    NSWindow* pWindow = [(IGRAPHICS_VIEW*) mView window];
    NSPoint wndpt = [pWindow convertRectFromScreen: NSMakeRect(x, y, 0.0, 0.0)].origin;
    NSPoint pt = [(IGRAPHICS_VIEW*) mView convertPoint:NSMakePoint(wndpt.x, wndpt.y) fromView:nil];

    x = pt.x / GetDrawScale();
    y = pt.y / GetDrawScale();
  }
}

void IGraphicsMac::HideMouseCursor(bool hide, bool lock)
{
  if (mCursorHidden == hide)
    return;
  
  mCursorHidden = hide;
  
  if (hide)
  {
    StoreCursorPosition();
    CGDisplayHideCursor(kCGDirectMainDisplay);
    mCursorLock = lock;
  }
  else
  {
    DoCursorLock(mCursorX, mCursorY, mCursorX, mCursorY);
    CGDisplayShowCursor(kCGDirectMainDisplay);
    mCursorLock = false;
  }
}

void IGraphicsMac::MoveMouseCursor(float x, float y)
{
  if (mTabletInput)
    return;
    
  PointToScreen(x, y);
  RepositionCursor(CGPoint{x, y});
  StoreCursorPosition();
}

void IGraphicsMac::DoCursorLock(float x, float y, float& prevX, float& prevY)
{
  if (mCursorHidden && mCursorLock && !mTabletInput)
  {
    RepositionCursor(mCursorLockPosition);
    prevX = mCursorX;
    prevY = mCursorY;
  }
  else
  {
    mCursorX = prevX = x;
    mCursorY = prevY = y;
  }
}

void IGraphicsMac::RepositionCursor(CGPoint point)
{
  point = CGPoint{point.x, CGDisplayPixelsHigh(CGMainDisplayID()) - point.y};
  CGAssociateMouseAndMouseCursorPosition(false);
  CGDisplayMoveCursorToPoint(CGMainDisplayID(), point);
  CGAssociateMouseAndMouseCursorPosition(true);
}

void IGraphicsMac::StoreCursorPosition()
{
  // Get position in screen coordinates
  NSPoint mouse = [NSEvent mouseLocation];
  mCursorX = mouse.x = std::round(mouse.x);
  mCursorY = mouse.y = std::round(mouse.y);
  mCursorLockPosition = CGPoint{mouse.x, mouse.y};
  
  // Convert to IGraphics coordinates
  ScreenToPoint(mCursorX, mCursorY);
}

int IGraphicsMac::ShowMessageBox(const char* str, const char* caption, EMessageBoxType type)
{
  NSInteger ret = 0;
  
  if (!str) str= "";
  if (!caption) caption= "";
  
  NSString *msg = (NSString *) CFStringCreateWithCString(NULL,str,kCFStringEncodingUTF8);
  NSString *cap = (NSString *) CFStringCreateWithCString(NULL,caption,kCFStringEncodingUTF8);
 
  msg = msg ? msg : (NSString *) CFStringCreateWithCString(NULL, str, kCFStringEncodingASCII);
  cap = cap ? cap : (NSString *) CFStringCreateWithCString(NULL, caption, kCFStringEncodingASCII);
  
  switch (type)
  {
    case kMB_OK:
      NSRunAlertPanel(msg, @"%@", @"OK", @"", @"", cap);
      ret = kOK;
      break;
    case kMB_OKCANCEL:
      ret = NSRunAlertPanel(msg, @"%@", @"OK", @"Cancel", @"", cap);
      ret = ret ? kOK : kCANCEL;
      break;
    case kMB_YESNO:
      ret = NSRunAlertPanel(msg, @"%@", @"Yes", @"No", @"", cap);
      ret = ret ? kYES : kNO;
      break;
    case kMB_RETRYCANCEL:
      ret = NSRunAlertPanel(msg, @"%@", @"Retry", @"Cancel", @"", cap);
      ret = ret ? kRETRY : kCANCEL;
      break;
    case kMB_YESNOCANCEL:
      ret = NSRunAlertPanel(msg, @"%@", @"Yes", @"Cancel", @"No", cap);
      ret = (ret == 1) ? kYES : (ret == -1) ? kNO : kCANCEL;
      break;
  }
  
  [msg release];
  [cap release];
  
  return static_cast<int>(ret);
}

void IGraphicsMac::ForceEndUserEdit()
{
  if (mView)
  {
    [(IGRAPHICS_VIEW*) mView endUserInput];
  }
}

void IGraphicsMac::UpdateTooltips()
{
  if (!(mView && TooltipsEnabled()))
    return;

  @autoreleasepool {

  [(IGRAPHICS_VIEW*) mView removeAllToolTips];

  if (GetPopupMenuControl() && GetPopupMenuControl()->GetState() > IPopupMenuControl::kCollapsed)
  {
    return;
  }

  auto func = [this](IControl& control)
  {
    if (control.GetTooltip() && !control.IsHidden())
    {
      IRECT pR = control.GetTargetRECT();
      if (!pR.Empty())
      {
        [(IGRAPHICS_VIEW*) mView registerToolTip: pR];
      }
    }
  };

  ForStandardControlsFunc(func);
  
  }
}

const char* IGraphicsMac::GetPlatformAPIStr()
{
  return "Cocoa";
}

bool IGraphicsMac::RevealPathInExplorerOrFinder(WDL_String& path, bool select)
{
  BOOL success = FALSE;

  @autoreleasepool {
    
  if(path.GetLength())
  {
    NSString* pPath = [NSString stringWithCString:path.Get() encoding:NSUTF8StringEncoding];

    if([[NSFileManager defaultManager] fileExistsAtPath : pPath] == YES)
    {
      if (select)
      {
        NSString* pParentDirectoryPath = [pPath stringByDeletingLastPathComponent];

        if (pParentDirectoryPath)
        {
          success = [[NSWorkspace sharedWorkspace] openFile:pParentDirectoryPath];

          if (success)
            success = [[NSWorkspace sharedWorkspace] selectFile: pPath inFileViewerRootedAtPath:pParentDirectoryPath];
        }
      }
      else {
        success = [[NSWorkspace sharedWorkspace] openFile:pPath];
      }

    }
  }

  }
  return (bool) success;
}

void IGraphicsMac::PromptForFile(WDL_String& fileName, WDL_String& path, EFileAction action, const char* ext)
{
  if (!WindowIsOpen())
  {
    fileName.Set("");
    return;
  }

  NSString* pDefaultFileName;
  NSString* pDefaultPath;
  NSArray* pFileTypes = nil;

  if (fileName.GetLength())
    pDefaultFileName = [NSString stringWithCString:fileName.Get() encoding:NSUTF8StringEncoding];
  else
    pDefaultFileName = [NSString stringWithCString:"" encoding:NSUTF8StringEncoding];

  if(!path.GetLength())
    DesktopPath(path);

  pDefaultPath = [NSString stringWithCString:path.Get() encoding:NSUTF8StringEncoding];

  fileName.Set(""); // reset it

  if (CStringHasContents(ext))
    pFileTypes = [[NSString stringWithUTF8String:ext] componentsSeparatedByString: @" "];

  if (action == kFileSave)
  {
    NSSavePanel* pSavePanel = [NSSavePanel savePanel];

    //[panelOpen setTitle:title];
    [pSavePanel setAllowedFileTypes: pFileTypes];
    [pSavePanel setAllowsOtherFileTypes: NO];

    long result = [pSavePanel runModalForDirectory:pDefaultPath file:pDefaultFileName];

    if (result == NSOKButton)
    {
      NSString* pFullPath = [pSavePanel filename] ;
      fileName.Set([pFullPath UTF8String]);

      NSString* pTruncatedPath = [pFullPath stringByDeletingLastPathComponent];

      if (pTruncatedPath)
      {
        path.Set([pTruncatedPath UTF8String]);
        path.Append("/");
      }
    }
  }
  else
  {
    NSOpenPanel* pOpenPanel = [NSOpenPanel openPanel];

    //[pOpenPanel setTitle:title];
    //[pOpenPanel setAllowsMultipleSelection:(allowmul?YES:NO)];
    [pOpenPanel setCanChooseFiles:YES];
    [pOpenPanel setCanChooseDirectories:NO];
    [pOpenPanel setResolvesAliases:YES];

    long result = [pOpenPanel runModalForDirectory:pDefaultPath file:pDefaultFileName types:pFileTypes];

    if (result == NSOKButton)
    {
      NSString* pFullPath = [pOpenPanel filename] ;
      fileName.Set([pFullPath UTF8String]);

      NSString* pTruncatedPath = [pFullPath stringByDeletingLastPathComponent];

      if (pTruncatedPath)
      {
        path.Set([pTruncatedPath UTF8String]);
        path.Append("/");
      }
    }
  }
}

void IGraphicsMac::PromptForDirectory(WDL_String& dir)
{
  NSString* defaultPath;

  if (dir.GetLength())
  {
    defaultPath = [NSString stringWithCString:dir.Get() encoding:NSUTF8StringEncoding];
  }
  else
  {
    defaultPath = [NSString stringWithCString:DEFAULT_PATH encoding:NSUTF8StringEncoding];
    dir.Set(DEFAULT_PATH);
  }

  NSOpenPanel* panelOpen = [NSOpenPanel openPanel];

  [panelOpen setTitle:@"Choose a Directory"];
  [panelOpen setCanChooseFiles:NO];
  [panelOpen setCanChooseDirectories:YES];
  [panelOpen setResolvesAliases:YES];
  [panelOpen setCanCreateDirectories:YES];

  [panelOpen setDirectoryURL: [NSURL fileURLWithPath: defaultPath]];

  if ([panelOpen runModal] == NSOKButton)
  {
    NSString* fullPath = [ panelOpen filename ] ;
    dir.Set( [fullPath UTF8String] );
    dir.Append("/");
  }
  else
  {
    dir.Set("");
  }
}

bool IGraphicsMac::PromptForColor(IColor& color, const char* str)
{
  //TODO:
  return false;
}

IPopupMenu* IGraphicsMac::CreatePlatformPopupMenu(IPopupMenu& menu, const IRECT& bounds, IControl* pCaller)
{
  IPopupMenu* pReturnMenu = nullptr;

  if (mView)
  {
    NSRect areaRect = ToNSRect(this, bounds);
    pReturnMenu = [(IGRAPHICS_VIEW*) mView createPopupMenu: menu: areaRect];
  }

  //synchronous
  if(pReturnMenu && pReturnMenu->GetFunction())
    pReturnMenu->ExecFunction();

  if(pCaller)
    pCaller->OnPopupMenuSelection(pReturnMenu); // should fire even if pReturnMenu == nullptr

  return pReturnMenu;
}

void IGraphicsMac::CreatePlatformTextEntry(IControl& control, const IText& text, const IRECT& bounds, const char* str)
{
  if (mView)
  {
    NSRect areaRect = ToNSRect(this, bounds);
    [(IGRAPHICS_VIEW*) mView createTextEntry: control : text: str: areaRect];
  }
}

CTFontDescriptorRef IGraphicsMac::GetCTFontDescriptor(const IText& text)
{
  StaticStorage<MacFontDescriptor>::Accessor storage(sFontDescriptorCache);

  MacFontDescriptor* cachedFont = storage.Find(text.mFont);
  
  assert(cachedFont && "font not found - did you forget to load it?");

  return cachedFont->mDescriptor;
}

//void IGraphicsMac::CreateWebView(const IRECT& bounds, const char* url)
//{
//  if (mView)
//  {
//    NSRect areaRect = ToNSRect(this, bounds);
//    [(IGRAPHICS_VIEW*) mView createWebView:areaRect :url];
//  }
//}

ECursor IGraphicsMac::SetMouseCursor(ECursor cursorType)
{
  if (mView)
    [(IGRAPHICS_VIEW*) mView setMouseCursor: cursorType];
    
  return IGraphics::SetMouseCursor(cursorType);
}

bool IGraphicsMac::OpenURL(const char* url, const char* msgWindowTitle, const char* confirmMsg, const char* errMsgOnFailure)
{
  #pragma REMINDER("Warning and error messages for OpenURL not implemented")
  NSURL* pNSURL = nullptr;
  if (strstr(url, "http"))
    pNSURL = [NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];
  else
    pNSURL = [NSURL fileURLWithPath:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];

  if (pNSURL)
  {
    bool ok = ([[NSWorkspace sharedWorkspace] openURL:pNSURL]);
    return ok;
  }
  return true;
}

void* IGraphicsMac::GetWindow()
{
  if (mView) return mView;
  else return 0;
}

// static
int IGraphicsMac::GetUserOSVersion()   // Returns a number like 0x1050 (10.5).
{
  return (int) GetSystemVersion();
}

bool IGraphicsMac::GetTextFromClipboard(WDL_String& str)
{
  NSString* pTextOnClipboard = [[NSPasteboard generalPasteboard] stringForType: NSStringPboardType];

  if (pTextOnClipboard == nil)
  {
    str.Set("");
    return false;
  }
  else
  {
    str.Set([pTextOnClipboard UTF8String]);
    return true;
  }
}

//TODO: THIS IS TEMPORARY, TO EASE DEVELOPMENT
#ifdef IGRAPHICS_AGG
  #include "IGraphicsAGG.cpp"
#elif defined IGRAPHICS_CAIRO
  #include "IGraphicsCairo.cpp"
#elif defined IGRAPHICS_NANOVG
  #include "IGraphicsNanoVG.cpp"
#else
  #include "IGraphicsLice.cpp"
#endif
