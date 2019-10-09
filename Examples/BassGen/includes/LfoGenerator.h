#pragma once //__BASSGEN__SUBOSCILLATOR__

#include "Oscillator.h"

enum LfoModes
{
  FREE_MODE, SYNC_MODE
};

class LfoGenerator : public Oscillator
{
public:
  LfoGenerator() :
    mLfoMode{ FREE_MODE },
    mLfoFrequency{ 1.0 },
    mLfoAmount{ 0.0 },
    mLfoSpeed{ 0.0 },
    mLfoSpeedByBpm{ 0.0 }
  {
  }

  inline void setLfoSpeed(double lfoSpeed);
  inline void setLfoAmountValue(float amount);
  inline void setLfoFrequency(float amount);
  inline void setMode(LfoModes mode);

  double nextSample() override;
  inline void reset();

  ~LfoGenerator();
private:
  LfoModes mLfoMode;
  // TODO: Lfo Delay
  // TODO: Lfo Transitions
  float mLfoAmount;
  float  mLfoFrequency;
  double mLfoSpeed, mLfoSpeedByBpm;

};

