#include "includes/Oscillator.h"
#include <cstdlib>
#include <ctime>

//float Oscillator::mSampleRate = 44100.0f;
float Oscillator::mSampleRate = 48000.0F; // New directives 2018

void Oscillator::setMode(OscillatorMode mode)
{
	mOscillatorMode = mode;
}

void Oscillator::setFrequency(double frequency)
{
	mFrequency = frequency;
	updateIncrement();
}

void Oscillator::setSampleRate(float sampleRate)
{
	mSampleRate = sampleRate;
	updateIncrement();
}

double Oscillator::nextSample() 
{
	double value = naiveWaveformForMode(mOscillatorMode);
	mPhase += mPhaseIncrement;
	while (mPhase >= twoPI)
  {
		mPhase -= twoPI;
	}
	return value;
}

void Oscillator::setPitchMod(double amount) {
	mPitchMod = amount;
	updateIncrement();
}

void Oscillator::setPhaseShift(double amount) {
  mPhaseShift = amount;
  updateIncrement();
}

void Oscillator::setDetuning(double amount) {
  //mPhase = amount;
  updateIncrement();
}

void Oscillator::updateIncrement() 
{
	double pitchModAsFrequency = pow(2.0, fabs(mPitchMod) * 14.0) - 1;

	if (mPitchMod < 0) 
	{
		pitchModAsFrequency = -pitchModAsFrequency;
	}
	double calculatedFrequency = fmin(fmax(mFrequency + pitchModAsFrequency + (mFineTuning*12), 0), mSampleRate / 2.0);

	mPhaseIncrement = calculatedFrequency * 2 * mPI / mSampleRate;

  /*
  float getOscFineTuneValue(float value)
  {
    return (value - 0.5f) * 2.0f;
  }

  float getOscTuneValue(float value)
  {
    return (float)(int)(value * 48.0f - 24.0f);
  }*/
}

double Oscillator::naiveWaveformForMode(OscillatorMode mode) 
{
  double value = 0.0;
  if (mode == OSCILLATOR_MODE_OFF) { value = 0.0; mPhase = 0.0; }
  if (mode == OSCILLATOR_MODE_SINE) value = sin(mPhase);
  if (mode == OSCILLATOR_MODE_SAW) value = (2.0 * mPhase / twoPI) - 1.0;
  if (mode == OSCILLATOR_MODE_SQUARE) (mPhase < mPI) ? value = 1.0 : value = -1.0;
  if (mode == OSCILLATOR_MODE_TRIANGLE)
  {
    value = -1.0 + (2.0 * mPhase / twoPI);
    value = 2.0 * (fabs(value) - 0.5);
  }
  if (mode == OSCILLATOR_MODE_NOISE) value = (2.0 * rand()/RAND_MAX)-1.0;

	return value;
}

Oscillator::~Oscillator(){
}