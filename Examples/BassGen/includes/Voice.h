#pragma once //__BASSGEN__VOICE__

//#include "Oscillators.h"
#include "PolyBLEPOscillator.h"
#include "LfoGenerator.h" 
#include "EnvelopeGenerator.h"
#include "Filter.h"

class Voice {
public:
	friend class VoiceManager;
	Voice() :
		mNoteNumber{ -1 },
		mVelocity{ 0 },
		mFilterEnvelopeAmount{ 0.0 },
		mFilterLFOAmount{ 0.0 },
		mOscOnePitchAmount{ 0.0 },
		mOscTwoPitchAmount{ 0.0 },
		mSubOscPitchAmount{ 0.0 },
    mOscOneFineTuning{ 0.0 },
		mLFOValue{ 0.0 },
		mMasterVol{ 0.8 },
		isActive(false) 
	{
		// Set myself free everytime my volume envelope has fully faded out of RELEASE stage:
		mVolumeEnvelope.finishedEnvelopeCycle.Connect(this, &Voice::setFree);
	};

	double nextSample();

	inline void setMasterVolValue(double value) { mMasterVol = value; }

  inline void setOscOnePitchAmount(double amount) { mOscOnePitchAmount = amount; }
	inline void setOscTwoPitchAmount(double amount) { mOscTwoPitchAmount = amount; }
	inline void setSubOscPitchAmount(double amount) { mSubOscPitchAmount = amount; }

  inline void setOscOneFineTuningAmount(double amount) { mSubOscPitchAmount = amount; }

	inline void setFilterEnvelopeAmount(double amount) { mFilterEnvelopeAmount = amount; }
	inline void setFilterLFOAmount(double amount) { mFilterLFOAmount = amount; }
	inline void setLFOValue(double value) { mLFOValue = value; }

	void setNoteNumber(int noteNumber);

	void setFree();
	void reset();
	~Voice();
private:
	//Oscillators mOscillatorOne;
	//Oscillators mOscillatorTwo;
	PolyBLEPOscillator mOscillatorOne;
	PolyBLEPOscillator mOscillatorTwo;
	PolyBLEPOscillator mSubOscillator;
	PolyBLEPOscillator mNoiseOscillator;
	
	EnvelopeGenerator mVolumeEnvelope;
	EnvelopeGenerator mFilterEnvelope;

	Filter mFilter;

	int mNoteNumber;
	int mVelocity;
	double mOscOnePitchAmount;
	double mOscTwoPitchAmount;
	double mSubOscPitchAmount;

  double mOscOneFineTuning;
	double mFilterEnvelopeAmount;
	double mFilterLFOAmount;
	double mLFOValue;
	double mMasterVol;

	bool isActive;
};