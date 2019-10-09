#pragma once 
//#include "Params.h"
#include "IControl.h"

using namespace iplug;
using namespace igraphics;

class EnvelopeNumber : public IControl
{
public:
  EnvelopeNumber(IRECT bounds) :
    IControl{ bounds, kNoParameter },
    isDragged{ false },
    mouseX{0}, mouseY{0}
  {};

  // main draw method for this control.
  void Draw(IGraphics& g) override
  {
    g.FillRect(COLOR_DARK_GRAY, mRECT);
    //g.DrawDottedLine(COLOR_GREEN, mRECT.W() / 2, mRECT.H() / 2, mouseX, mouseY);
  }
  
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    if (isDragged == true)
    {
      mouseX = x;
      mouseY = y;
    }
    SetDirty(true);
  };

  void OnMouseDown(float x, float y, const IMouseMod& mod) override { isDragged = true;
  mouseX = x;
  mouseY = y;
  };
  void OnMouseUp(float x, float y, const IMouseMod& mod) override { isDragged = false; };

  bool IsDirty() { return true; }

  ~EnvelopeNumber() {};
private:
  bool isDragged;
  float mouseX, mouseY;
};