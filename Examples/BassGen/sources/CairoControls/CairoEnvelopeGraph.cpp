#include "CairoControls\CairoEnvelopeGraph.h"

/**
 *	Cairo Envelope Graph
 **/
void CairoEnvelopeGraph::Draw(IGraphics & g) {
  //drawTimeLines();

	updateADSRPlot();
}

void CairoEnvelopeGraph::initADSRPlot()
{
  Point init_point{ 0, 0.0, 0.0 };

  float x = getPosX(); //control x
  float y = getPosY(); //control y

  unsigned int sustainPoint_Y = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpSustain)->GetParam()->Value(), zero_time, sustain_lvl, controlBottomSide, controlTopSide);

  //TODO: add proper timing (what is 10000??)
  unsigned int attackPoint_X = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpAttack)->GetParam()->Value(), zero_time, atk_time, controlLeftSide, 10000);

  unsigned int decayPoint_X = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpDecay)->GetParam()->Value(), zero_time, decay_time, attackPoint_X, 10000);

  unsigned int releasePoint_X = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpRelease)->GetParam()->Value(), zero_time, release_time, decayPoint_X, 10000);

  // Start point
  init_point.setPointID(1);
  init_point.setPointCordX(x);
  init_point.setPointCordY(y);
  points.push_back(init_point);

  // Attack point
  init_point.setPointID(2);
  init_point.setPointCordX(attackPoint_X);
  init_point.setPointCordY(sustainPoint_Y);
  points.push_back(init_point);

  // TODO HOLD point

  // Decay/Sustain point
  init_point.setPointID(3);
  init_point.setPointCordX(decayPoint_X);
  init_point.setPointCordY(sustainPoint_Y);
  points.push_back(init_point);

  // Release point
  init_point.setPointID(4);
  init_point.setPointCordX(releasePoint_X);
  init_point.setPointCordY(y);
  points.push_back(init_point);
}

void CairoEnvelopeGraph::drawSinglePoint(float x, float y, float r)
{
  cairo_set_line_width(cr, 2.0);
  cairo_set_source_rgba(cr, 255.0, 255.0, 255.0, 0.8);
  cairo_arc(cr, x, y, r, 0.0, 2.0 * PI);
  cairo_fill(cr);
}

void CairoEnvelopeGraph::drawTimeLines()
{
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgba(cr, 100.0, 100.0, 100.0, 0.2);

	float tls = getTimeLineSpacing();
	
	for (int i = 0; i < (tls - 1); i++) {
		cairo_move_to(cr, controlLeftSide + ((controlRightSide*i) / tls), controlTopSide);
		cairo_line_to(cr, controlLeftSide + ((controlRightSide*i) / tls), controlBottomSide);
	}
  cairo_stroke(cr);
 }

void CairoEnvelopeGraph::updateADSRPlot()
{
  unsigned int attackPoint_X = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpAttack)->GetValue(), zero_time, atk_time, controlLeftSide, 10000);
  points[1].setPointCordX(attackPoint_X);
  points[1].setPointCordY(controlTopSide);

  unsigned int sustainPoint_Y = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpSustain)->GetValue(), zero_time, sustain_lvl, controlBottomSide, controlTopSide);
  unsigned int decayPoint_X = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpDecay)->GetValue(), zero_time, decay_time, attackPoint_X, 10000);
  points[2].setPointCordX(decayPoint_X);
  points[2].setPointCordY(sustainPoint_Y);

  unsigned int releasePoint_X = mapKnobToCoords(
    gCairo->GetControlWithTag(kAmpRelease)->GetValue(), zero_time, release_time, decayPoint_X, 10000);
  points[3].setPointCordX(releasePoint_X);

  Point previous = points[0];
	cairo_move_to(cr, previous.getPointCordX(), previous.getPointCordY());

  cairo_pattern_t* pat3;
  pat3 = cairo_pattern_create_linear(
    controlLeftSide + 120.0, controlTopSide,
    controlLeftSide + 120.0, controlTopSide + 200.0);
  cairo_pattern_add_color_stop_rgba(pat3, 0.0, 162.0, 0.0, 0.0, 0.3);
  cairo_pattern_add_color_stop_rgba(pat3, 1.0, 0.0, 0.0, 0.0, 0.1);

  for (auto& p : points)
  {
    auto p_x = p.getPointCordX();
    auto p_y = p.getPointCordY();

    if (previous.getPointID() > 0) cairo_line_to(cr, p_x, p_y);
    previous = p;
  }

  cairo_set_line_width(cr, 2.0);
  cairo_set_source_rgba(cr, 162.0, 0.0, 0.0, 0.6);
  cairo_stroke_preserve(cr);
  cairo_set_source(cr,pat3);
  cairo_fill(cr);
  cairo_pattern_destroy(pat3);

  for (auto& p : points)
  {
    auto p_x = p.getPointCordX();
    auto p_y = p.getPointCordY();

    if (previous.getPointID() > 0) drawSinglePoint(p_x, p_y, 3.0F);
  }
 
}

Point CairoEnvelopeGraph::getClickedSpot(int x, int y)
{
	for (auto& spot : points) {

		auto spot_x = spot.getPointCordX();
		auto spot_y = spot.getPointCordY();

		if (((spot_x - 6) <= x) && ((spot_x + 6) >= x)
			&& ((spot_y - 6) <= y) && ((spot_y + 6) >= y)) {
			return spot;
		};
	}

	Point none{ 0, 0.0, 0.0 };
	return none;
};

void CairoEnvelopeGraph::OnMouseDown(float x, float y, const IMouseMod& mod)
{
  /*TODO: Make animation or marker for selected point.*/
	Point clicked = getClickedSpot(x, y);
	
	if (clicked.getPointID() == 0) { 
    Point none{ 0, 0.0, 0.0 };
    selected = none;
		isMouseDrag = false;
		SetDirty();
	}
	else {
		selected = clicked;

		isMouseDrag = true;
		SetDirty(true);
	}
}

void CairoEnvelopeGraph::OnMouseUp(float x, float y, const IMouseMod& mod)
{
	isMouseDrag = false;
}

void CairoEnvelopeGraph::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod)
{
	int selected_id = selected.getPointID();

	// disables dragging if points id is = 0 or 1(0 point and first point).
  if (selected_id == 0 || selected_id == 1) { isMouseDrag = false; return; }
	if (isMouseDrag == false) return;

	//stores which point is selected.
	auto result = std::find(points.begin(), points.end(), selected);

	int point_id = result->getPointID();

	if (result != points.end())
  {
		if ((x > controlLeftSide) && (y < controlBottomSide && y > controlTopSide))
		{
			if (point_id == 2) { //Attack Knob change.
        result->setPointCordX(x);
        gCairo->GetDelegate()->SendParameterValueFromDelegate(kAmpAttack,
          mapCoordsToKnob(result->getPointCordX(), controlLeftSide, 10000, zero_time, atk_time), true);
      }
			//TODO: HOLD point 
			/*if () {//Hold Knob change.
				GetGUI()->SetParameterFromGUI(kAmpDecay,
					mapCoordsToKnob(x, points[1].getPointCordX(), points[4].getPointCordX(), 0.0f, 1.0f));
				result->setPointCordX(x);
				GetGUI()->SetParameterFromGUI(kAmpSustain,
					mapCoordsToKnob(y, controlBottomSide, controlTopSide, 0.0, 1.0));
			}*/
      if (point_id == 3) //Decay/Sustain Knob change. 
      {
        result->setPointCordX(x);
        result->setPointCordY(y);
        gCairo->GetDelegate()->SendParameterValueFromDelegate(kAmpDecay,
          mapCoordsToKnob(result->getPointCordX(), points[1].getPointCordX(), 10000, zero_time, decay_time), true);
        gCairo->GetDelegate()->SendParameterValueFromDelegate(kAmpSustain,
          mapCoordsToKnob(result->getPointCordY(), controlBottomSide, controlTopSide, zero_time, sustain_lvl), true);
        //points[3].setPointCordX((result->getPointCordX() - points[2].getPointCordX()) + x);

        if (result->getPointCordX() <= points[1].getPointCordX())
          result->setPointCordX(points[1].getPointCordX());
        if (result->getPointCordX() >= points[3].getPointCordX())
          result->setPointCordX(points[3].getPointCordX());
			}

			if (point_id == 4) //Release Knob change.
      {
        result->setPointCordX(x);
        gCairo->GetDelegate()->SendParameterValueFromDelegate(kAmpRelease,
					mapCoordsToKnob(result->getPointCordX(), points[2].getPointCordX(), 10000, zero_time, release_time),true);

				if (result->getPointCordX() <= (points[2].getPointCordX()))   
          result->setPointCordX(points[2].getPointCordX());
        
				if (x > controlRightSide) {
					setTimeLineSpacing(40.0F);
				}
				else {
					setTimeLineSpacing(27.2F);
				}
			}
		}
    if (x == controlLeftSide) x = controlLeftSide;
    if (y == controlBottomSide) y = controlBottomSide;
    if (y == controlTopSide) y = controlTopSide;
	}
	SetDirty(true);
}

int CairoEnvelopeGraph::mapKnobToCoords(
	float val, float min_knob, int max_knob, int min_cord, int max_cord)
{
	//(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	return 	(val - min_knob)*(max_cord - min_cord) / (max_knob - min_knob) + min_cord;
}

float CairoEnvelopeGraph::mapCoordsToKnob(
	int val, int min_cord, int max_cord, float min_knob, float max_knob)
{
	return (val - min_cord)*(max_knob - min_knob) / (max_cord - min_cord) + min_knob;
}

