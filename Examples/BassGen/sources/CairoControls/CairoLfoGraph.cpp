#include "CairoControls\CairoLfoGraph.h"

/**
 *	Cairo LFO Graph
 **/
void CairoLfoGraph::Draw(IGraphics& g)
{
  //drawGrid();

  //drawPositionMark(); //TODO: do this metod

  updateLfoPlot();
}

void CairoLfoGraph::initLfoPlot()
{
  //Start with empty container.
  points.clear();

  Point init_point { 0, 0.0, 0.0 };

  // Start point
  init_point.setPointID(0);
  init_point.setPointCordX(controlLeftSide);
  init_point.setPointCordY(controlBottomSide);
  points.push_back(init_point);

  init_point.setPointID(1);
  init_point.setPointCordX(((controlRightSide - controlLeftSide)/2.0F) + controlLeftSide);
  init_point.setPointCordY(controlTopSide);
  points.push_back(init_point);

  init_point.setPointID(2);
  init_point.setPointCordX(controlRightSide);
  init_point.setPointCordY(controlBottomSide);
  points.push_back(init_point);
}

void CairoLfoGraph::drawSinglePoint(float x, float y, float r)
{
  cairo_set_line_width(cr, 2.0);
  cairo_set_source_rgba(cr, 255.0, 255.0, 255.0, 0.8);
  cairo_arc(cr, x, y, r, 0.0, 2.0 * PI);
  cairo_fill(cr);
}

void CairoLfoGraph::drawGrid()
{
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgba(cr, 100.0, 100.0, 100.0, 0.2);

  cairo_stroke(cr);
}

void CairoLfoGraph::updateLfoPlot()
{
  cairo_pattern_t* pat3;
  pat3 = cairo_pattern_create_linear(
    controlLeftSide + 120.0, controlTopSide,
    controlLeftSide + 120.0, controlTopSide + 200.0);
  cairo_pattern_add_color_stop_rgba(pat3, 0.0, 162.0, 0.0, 0.0, 0.4);
  cairo_pattern_add_color_stop_rgba(pat3, 1.0, 0.0, 0.0, 0.0, 0.1);

  for (auto& p : points)
  {
    auto p_x = p.getPointCordX();
    auto p_y = p.getPointCordY();

    cairo_line_to(cr, p_x, p_y);
  }

  cairo_set_line_width(cr, 2.0);
  cairo_set_source_rgba(cr, 162.0, 0.0, 0.0, 0.6);
  cairo_stroke_preserve(cr);
  cairo_set_source(cr, pat3);
  cairo_fill(cr);
  for (auto& p : points)
  {
    auto p_x = p.getPointCordX();
    auto p_y = p.getPointCordY();

    drawSinglePoint(p_x, p_y, 3.0F);
  }

  cairo_pattern_destroy(pat3);

}

Point CairoLfoGraph::getClickedPoint(int x, int y)
{
  for (auto& spot : points)
  {
    auto spot_x = spot.getPointCordX();
    auto spot_y = spot.getPointCordY();

    if (((spot_x - 6) <= x) && ((spot_x + 6) >= x)
      && ((spot_y - 6) <= y) && ((spot_y + 6) >= y)) {
      return spot;
    }
  }

  Point none{ 0, 0.0, 0.0 };
  return none;
};

void CairoLfoGraph::OnMouseDblClick(float x, float y, const IMouseMod& mod)
{
  auto nextPoint = std::find_if(points.begin(), points.end(), [x](const Point& p) {return p.getPointCordX() > x; });
  int pointID = nextPoint->getPointID();

  selected.setPointID(pointID - 1);
  selected.setPointCordX(x);
  selected.setPointCordY(y);
  points.insert(nextPoint, selected);
}


void CairoLfoGraph::OnMouseDown(float x, float y, const IMouseMod& mod)
{
  /*TODO: Make animation or marker for selected point.*/
  Point clickedPoint = getClickedPoint(x, y);
  auto clickedPointId = clickedPoint.getPointID();

  if (clickedPointId == 0)
  {
    Point none{ 0, 0.0, 0.0 };
    selected = none;
    isMouseDrag = false;
  }
  else {
    selected = clickedPoint;
    isMouseDrag = true;
  }
  SetDirty(true);
}

void CairoLfoGraph::OnMouseUp(float x, float y, const IMouseMod& mod)
{
  isMouseDrag = false;
}

void CairoLfoGraph::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod)
{
  if (isMouseDrag == false) return;

  //stores which point is selected.
  //auto result = std::find(points.begin(), points.end(), selected);
  auto result = std::find(points.begin(), points.end(), selected);


  if (result != points.end())
  {
    if ((x > controlLeftSide && x < controlRightSide) || (y > controlTopSide && y < controlBottomSide))
    {
      result->setPointCordX(x);
      result->setPointCordY(y);
    }
    if (x < controlLeftSide) result->setPointCordX(controlLeftSide);
    if (x > controlRightSide) result->setPointCordX(controlRightSide);
    if (y > controlBottomSide) result->setPointCordY(controlBottomSide);
    if (y < controlTopSide) result->setPointCordY(controlTopSide);

  }
  SetDirty(true);
}
