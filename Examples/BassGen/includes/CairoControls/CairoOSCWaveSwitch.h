#pragma once 
#include <vector>
#include <algorithm>
#include "IControl.h"

using namespace iplug;
using namespace igraphics;

/**
 *	Cairo Oscilator Waveform switch.
 */
class CairoOSCWaveSwitch : public IControl
{
public:
  CairoOSCWaveSwitch(IGraphics* g, IRECT bounds, int paramIdx, float posX, float posY, int mode)
    : IControl(bounds, paramIdx),
    mCairoOscMode{ mode },
    mPosX{ posX },
    mPosY{ posY },
    g{ g },
    cr{ static_cast<cairo_t *>(g->GetDrawContext())}
    //constructor body
  {};

  void setCairoOscMode(int mode) { mCairoOscMode = mode; }
  int getCairoOscMode() { return mCairoOscMode; }
	
 	void setPosX(float x) { mPosX = x; }
 	void setPosY(float y) { mPosY = y; }
 
 	double getPosX() { return mPosX; }
 	double getPosY() { return mPosY; }
 
 	void drawOscEmpty(double posX, double posY);
 	void drawOscSine(double posX, double posY);
 	void drawOscSaw(double posX, double posY);
 	void drawOscSquare(double posX, double posY);
 	void drawOscTriangle(double posX, double posY);
 
  void Draw(IGraphics& g) override;
 	bool IsDirty() { return true; }
 
 	void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  ~CairoOSCWaveSwitch() {};

private:
  IGraphics* g;
  cairo_t* cr;
  int mCairoOscMode;
  float mPosX, mPosY;
};
