#include "includes\CairoControls\CairoOSCWaveSwitch.h"

/*
 * Cairo Oscillator switch graphics
 */

void CairoOSCWaveSwitch::Draw(IGraphics & g) {

  unsigned char mode = getCairoOscMode();

  cairo_set_source_rgba(cr, 66.0, 0.0, 0.0, 0.4F);
  cairo_set_line_width(cr, 2);

  switch (mode) {
  case 1:
    drawOscSine(getPosX(), getPosY());
    break;
  case 2:
    drawOscSaw(getPosX(), getPosY());
    break;
  case 3:
    drawOscSquare(getPosX(), getPosY());
    break;
  case 4:
    drawOscTriangle(getPosX(), getPosY());
    break;
  default:
    drawOscEmpty(getPosX(), getPosY());
    break;
  }
}

void CairoOSCWaveSwitch::OnMouseDown(float x, float y, const IMouseMod& mod) {
  unsigned char osc_mode = getCairoOscMode();
  double value = GetValue();

  ++osc_mode;

  if(mod.L && osc_mode != 5) {
    switch (osc_mode) {
    case 2:
      setCairoOscMode(2); value = 0.4; //Saw shape.
      break;
    case 3:
      setCairoOscMode(3); value = 0.5; //Square shape.
      break;
    case 4:
      setCairoOscMode(4); value = 0.7; //Triangle shape.
      break;
    default:
      setCairoOscMode(1); value = 0.2; // Sine shape.
      break;
    }
  }
  else {
    setCairoOscMode(1); value = 0.2; osc_mode = 1; //Back to default.
  }
  SetValue(value);
  SetDirty(true);
}

void CairoOSCWaveSwitch::drawOscEmpty(double posX, double posY) {
  //                 x      y
  cairo_move_to(cr, posX, posY);
}

void CairoOSCWaveSwitch::drawOscSine(double posX, double posY) {
  //                 x      y
  cairo_move_to(cr, posX, posY);
  cairo_curve_to(cr,
    posX + 100, posY - 216, posX + 100, posY + 216, posX + 198, posY);
  cairo_fill_preserve(cr);
  cairo_stroke(cr);
}

void CairoOSCWaveSwitch::drawOscSaw(double posX, double posY) {
  //                 x      y
  cairo_move_to(cr, posX, posY);
  cairo_line_to(cr, posX, posY - 62);
  cairo_line_to(cr, posX + 198, posY + 62);
  cairo_line_to(cr, posX + 198, posY);
  cairo_fill_preserve(cr);
  cairo_stroke(cr);
}

void CairoOSCWaveSwitch::drawOscSquare(double posX, double posY) {
  //                 x      y
  cairo_move_to(cr, posX, posY);
  cairo_line_to(cr, posX, posY - 62);
  cairo_line_to(cr, posX + 100, posY - 62);
  cairo_line_to(cr, posX + 100, posY + 62);
  cairo_line_to(cr, posX + 200, posY + 62);
  cairo_line_to(cr, posX + 200, posY);
  cairo_fill_preserve(cr);
  cairo_stroke(cr);
}

void CairoOSCWaveSwitch::drawOscTriangle(double posX, double posY) {
  //                 x      y
  cairo_move_to(cr, posX, posY);
  cairo_line_to(cr, posX + 50, posY - 62);
  cairo_line_to(cr, posX + 150, posY + 62);
  cairo_line_to(cr, posX + 200, posY);
  cairo_fill_preserve(cr);
  cairo_stroke(cr);
}