/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#pragma once

#include "IGraphics_select.h"
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>

/** IGraphics platform class for macOS
*   @ingroup PlatformClasses */
class IGraphicsMac final : public IGRAPHICS_DRAW_CLASS
{
public:
    
  class MacFont : public PlatformFont
  {
  public:
    MacFont(CTFontDescriptorRef descriptor, CGDataProviderRef provider)
     : mDescriptor(descriptor), mProvider(provider) {}
    ~MacFont();
      
    const void* GetDescriptor() override { return reinterpret_cast<const void*>(mDescriptor); }
    IFontDataPtr GetFontData() override;

  private:
      
    CTFontDescriptorRef mDescriptor;
    CGDataProviderRef mProvider;
  };
    
  IGraphicsMac(IGEditorDelegate& dlg, int w, int h, int fps, float scale);
  virtual ~IGraphicsMac();

  void SetBundleID(const char* bundleID) { mBundleID.Set(bundleID); }

  bool IsSandboxed();

  void* OpenWindow(void* pWindow) override;
  void CloseWindow() override;
  bool WindowIsOpen() override;
  void PlatformResize() override;
  
  void PointToScreen(float& x, float& y);
  void ScreenToPoint(float& x, float& y);

  void HideMouseCursor(bool hide, bool lock) override;
  void MoveMouseCursor(float x, float y) override;
  ECursor SetMouseCursor(ECursor cursorType) override;
    
  void DoCursorLock(float x, float y, float& prevX, float& prevY);
    
  int ShowMessageBox(const char* str, const char* caption, EMessageBoxType type) override;
  void ForceEndUserEdit() override;

  const char* GetPlatformAPIStr() override;

  void UpdateTooltips() override;

  bool RevealPathInExplorerOrFinder(WDL_String& path, bool select) override;
  void PromptForFile(WDL_String& fileName, WDL_String& path, EFileAction action, const char* ext) override;
  void PromptForDirectory(WDL_String& dir) override;
  bool PromptForColor(IColor& color, const char* str) override;

//  void CreateWebView(const IRECT& bounds, const char* url) override;
  
  CTFontDescriptorRef GetCTFontDescriptor(const IText& text);
    
  bool OpenURL(const char* url, const char* msgWindowTitle, const char* confirmMsg, const char* errMsgOnFailure) override;

  void* GetWindow() override;

  const char* GetBundleID() override { return mBundleID.Get(); }
  static int GetUserOSVersion();

  bool GetTextFromClipboard(WDL_String& str) override;

  bool MeasureText(const IText& text, const char* str, IRECT& bounds) override;

  void ContextReady(void* pLayer);

protected:
  IPopupMenu* CreatePlatformPopupMenu(IPopupMenu& menu, const IRECT& bounds, IControl* pCaller) override;
  void CreatePlatformTextEntry(IControl& control, const IText& text, const IRECT& bounds, const char* str) override;
private:
  PlatformFontPtr LoadPlatformFont(const char* fontID, const char* fileNameOrResID) override;
  PlatformFontPtr LoadPlatformFont(const char* fontID, const char* fontName, ETextStyle style) override;
  void CachePlatformFont(const char* fontID, const PlatformFontPtr& font) override;

  void RepositionCursor(CGPoint point);
  void StoreCursorPosition();
  
  void* mView = nullptr;
  CGPoint mCursorLockPosition;
  WDL_String mBundleID;
  friend int GetMouseOver(IGraphicsMac* pGraphics);
};
