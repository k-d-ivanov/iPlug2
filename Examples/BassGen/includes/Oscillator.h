#pragma once //__BASSGEN__OSCILLATORS__

#include <math.h>
class Oscillator {
public:
	enum OscillatorMode {
		OSCILLATOR_MODE_OFF = 0,
		OSCILLATOR_MODE_SINE,
		OSCILLATOR_MODE_SAW,
		OSCILLATOR_MODE_SQUARE,
		OSCILLATOR_MODE_TRIANGLE,
		OSCILLATOR_MODE_NOISE,
		kNumOscillatorModes
	};
	void setMode(OscillatorMode mode);
	void setFrequency(double frequency);
	void setSampleRate(float sampleRate);

	void setSingleOSCLvlValue(float param) { mlvlParam = param; };
  float getSingleOSCLvlValue() { return mlvlParam; };

	inline void setMuted(bool muted) { isMuted = muted; }
	virtual double nextSample();

	Oscillator() :
		mOscillatorMode{ OSCILLATOR_MODE_OFF },
		mPI{ 2 * acos(0.0) },
		twoPI{ 2 * mPI },
		mFrequency{ 440.0 },
		isMuted{ true },
		mPitchMod{ 0.0 },
		mPhase{ 0.0 },
    mPhaseShift{ 0.0 },
    mFineTuning{ 0.0 },
    mPhaseIncrement{ 0.0 },
		mlvlParam{ 0.0 }
	{
		updateIncrement();
	};
	void setPitchMod(double amount);
  void setFineTuning(double amount);
  void setPhaseShift(double amount);
  void setDetuning(double amount);

	void reset() { mPhase = 0.0; }
	~Oscillator();

protected:
	double naiveWaveformForMode(OscillatorMode mode);
	OscillatorMode mOscillatorMode;
	const double mPI;
	const double twoPI;
	bool isMuted;
	double mFrequency, mPitchMod;
  double mPhase, mPhaseShift;
  double mFineTuning;
	static float mSampleRate;
	double mPhaseIncrement;
	float mlvlParam;
	void updateIncrement();
};