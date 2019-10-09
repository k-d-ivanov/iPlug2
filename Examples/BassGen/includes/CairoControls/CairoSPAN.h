#pragma once

/**
*	Cairo SPAN
*
class CairoSPAN : public IControl, ycairo_gui {
public:
  CairoSPAN(IPlugBase* pPlug, ycairo_base *ycairo_base, IRECT pR)
    : IControl(pPlug, pR),
    ycairo_gui(ycairo_base, this)
  {}

  bool Draw(IGraphics* pGraphics);
  double processSignal(double mPoints);
  bool IsDirty() { return true; }
  ~CairoSPAN() { cairo_surface_destroy(adsr_p); };
private:
  double mPoints;
  cairo_surface_t *adsr_p;
};*/