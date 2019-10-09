#pragma once

#include <vector>
#include <algorithm>
#include <list>
#include "Params.h"
#include "IControl.h"
#include "Point.h"

using namespace iplug;
using namespace igraphics;

/**
 *	Cairo LFO Graph
 */
class CairoLfoGraph : public IControl
{
public:
  //TODO: LFO - It's custom control, IRECT bounds can be directly constructed here
  //TODO: LFO - On/Off buttons grid for 6 LFO's.
  //TODO: LFO - Presets with shapes.
  CairoLfoGraph(IGraphics* g, IRECT bounds, int paramIdx)
    // constructor initializer list
    : IControl{ bounds, kNoParameter },
    selected{ 0, 0.0, 0.0},
    gCairo{ g },
    cr{ static_cast<cairo_t*>(gCairo->GetDrawContext()) }
  {
    initLfoPlot();
  };

  // main draw method for this control.
  void Draw(IGraphics& g) override;

  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnMouseUp(float x, float y, const IMouseMod& mod) override;
  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override;
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;

  void OnInit() override {}

  Point getClickedPoint(int x, int y);

  void initLfoPlot();
  void drawSinglePoint(float x, float y, float r);
  void updateLfoPlot();
  void drawGrid();

  bool IsDirty() { return true; }

  ~CairoLfoGraph() {
    cairo_destroy(cr);
    points.clear(); gCairo = nullptr;
  };

protected:
  std::list<Point> points;

  Point selected;

  bool isMouseDrag = false;

private:
  IGraphics* gCairo = nullptr;
  cairo_t* cr = nullptr;

  const float controlLeftSide = mRECT.L;
  const float controlRightSide = mRECT.R;
  const float controlTopSide = mRECT.T;
  const float controlBottomSide = mRECT.B;
};

