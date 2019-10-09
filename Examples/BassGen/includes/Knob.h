#pragma once

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

class Knob : public IKnobControlBase
           , public IBitmapBase
{
public:
  Knob(IGraphics* ui, const char* name, const char* label,
       float x, float y, const IBitmap& bitmap, int paramIdx,
       EDirection direction = EDirection::Vertical, double gearing = 2.0)
    : IKnobControlBase(IRECT(x, y, bitmap), paramIdx, direction, gearing)
    , IBitmapBase(bitmap)
    , knobText{ new IText(10.0, COLOR_RED, "Roboto-Regular") }
    //, textControl{ new ITextControl(IRECT(x, y + 30, x + 35, y + 40), label, *knobText)}
    //, knobCaption{ new ICaptionControl(IRECT(x, y - 40, x + 35, y + 21), paramIdx, *knobText, false) }
    //, popUpMenu{ new IPopupMenu("Test", 0, false, {"one", "two", "tree"}) }
    , param{ paramIdx }, _x{ x }, _y{ y }
  {
    AttachIControl(this);
    //ui->AttachControl(textControl);
    //ui->AttachControl(knobCaption);
    
  };

  void Draw(IGraphics& g) override
  {
    int i = 1 + int(0.5 + GetValue() * (double)(mBitmap.N() - 1.0));
    g.DrawBitmap(mBitmap, mRECT, i, &mBlend);
  }

 void OnMouseDown(float x, float y, const IMouseMod& mod) override
 {
 }

 void CreateContextMenu(IPopupMenu& contextMenu) override
 {
   contextMenu.AddItem("Detach All Modulations", 0);
   contextMenu.AddItem("Reset", 0);
 };

void OnContextSelection(int itemSelected) override
{
}

 void GrayOut(bool gray) override { IBitmapBase::GrayOut(gray); IControl::GrayOut(gray); }
 void OnRescale() override { mBitmap = GetUI()->GetScaledBitmap(mBitmap); }

 ~Knob() {};
private:
  IText* knobText;
  ITextControl* textControl;
  IPopupMenu* popUpMenu;
  ICaptionControl* knobCaption;
  int param = 0;
  float _x;
  float _y;
};