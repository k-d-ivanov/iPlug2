#include "includes\LfoGenerator.h"

void LfoGenerator::setLfoSpeed(double lfoSpeed)
{
  mLfoSpeed = lfoSpeed;
}

inline void LfoGenerator::setLfoAmountValue(float amount) { mLfoAmount = amount; }

inline void LfoGenerator::setLfoFrequency(float amount) { mLfoFrequency = amount; }

inline void LfoGenerator::setMode(LfoModes mode)
{
  mLfoMode = mode;
}

inline void LfoGenerator::reset() { mPhase = 0.0; }

LfoGenerator::~LfoGenerator()
{
}

double LfoGenerator::nextSample()
{
  return 0.0;
}