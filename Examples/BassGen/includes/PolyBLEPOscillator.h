#pragma once

#include "Oscillator.h"

class PolyBLEPOscillator : public Oscillator {
public:
	PolyBLEPOscillator() : lastOutput(0.0)
	{
		Oscillator::OscillatorMode(0);
		updateIncrement(); 
	};
	double nextSample();

	~PolyBLEPOscillator();
private:
	double poly_blep(double t);
	double lastOutput;
};
