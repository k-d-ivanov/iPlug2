#pragma once //__BASSGEN__CAIROENVELOPEGRAPH__

#include <vector>
#include <algorithm>
#include "Params.h"
#include "IControl.h"
#include "Point.h" 

using namespace iplug;
using namespace igraphics;

/**
 *	Cairo Envelope Graph
 */

enum ADSRModes{ //Not used right now
  enAttack = 0,
  //eHold,
  enDecay,
  enSustain,
  enRelease,
  enADSRModes
};


class CairoEnvelopeGraph : public IControl
{
public:
  //TODO: It's custom control, IRECT bounds can be directly constructed here
	CairoEnvelopeGraph(IGraphics* g, IRECT bounds, int paramIdx)
    // constructor initializer list
    : IControl{ bounds, kNoParameter },
    selected{ 0, 0.0, 0.0 },
    mPosX{ bounds.L },
	  mPosY{ bounds.B },
		mTimeLineSpacing{ 17.2f },
    gCairo{ g },
    cr{ static_cast<cairo_t*>(gCairo->GetDrawContext()) }
	{
    initADSRPlot();
  };

	// main draw method for this control.
  void Draw(IGraphics& g) override;

	void setPosX(float x) { mPosX = x; }
	void setPosY(float y) { mPosY = y; }

	float getPosX() { return mPosX; }
	float getPosY() { return mPosY; }

	void setTimeLineSpacing(float tls) { mTimeLineSpacing = tls; }
	float getTimeLineSpacing() { return mTimeLineSpacing; }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnMouseUp(float x, float y, const IMouseMod& mod) override;

	void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;

  void OnInit() override {}
 
	Point getClickedSpot(int x, int y);

  void initADSRPlot();
	void drawSinglePoint(float x, float y, float r);
	void updateADSRPlot();
	void drawTimeLines();
	bool IsDirty() { return true; }

  ~CairoEnvelopeGraph() {
    cairo_destroy(cr);
    points.clear(); gCairo = nullptr;
  };

protected:
	std::vector<Point> points;
	Point selected;
	bool isMouseDrag = false;

private:
  IGraphics* gCairo = nullptr;
  cairo_t* cr = nullptr;

	float mPosX, mPosY;
	float mTimeLineSpacing;
	
	const float zero_time = 0.0F;
	const float atk_time = 20.0F;
	const float decay_time = 20.0F;
	const float sustain_lvl = 1.0F;
	const float release_time = 20.0F;

  const float controlLeftSide = mRECT.L;
	const float controlRightSide = mRECT.R;
	const float controlTopSide = mRECT.T;
	const float controlBottomSide = mRECT.B;

	int mapKnobToCoords(float val, float min_knob, int max_knob, int min_cord, int max_cord);
	float mapCoordsToKnob(int val, int min_cord, int max_cord, float min_knob, float max_knob);
};
