#include "includes\CairoControls\CairoSPAN.h"
/*
bool CairoSPAN::Draw(IGraphics* pGraphics) {
  //ycairo_set_source_rgba(cr, IColor(255, 0, 0, 0));
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint_with_alpha(cr, 0.8);
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
  ycairo_set_source_rgba(cr, IColor(220, 0, 230, 250));
  cairo_set_line_width(cr, 1);
  cairo_move_to(cr, 250.0, 310.0);

  for (int i = 250; i < 750; i++) {
    for (int j = 300; j < 410; j++) {
      cairo_move_to(cr, i += 20, 410.0);
      cairo_line_to(cr, i, j);

      if (i == 750 && j == 410) {
        i = 250; j = 410;
        cairo_move_to(cr, i, j);
      }
    }
  }
  cairo_stroke(cr);
  ycairo_draw();
  return true;
}

double CairoSPAN::processSignal(double points) {
  return points;
}
*/