#pragma once //__BASSGEN__ENVELOPEGENERATOR__
#include <cmath>
#include "GallantSignal.h"
using Gallant::Signal0;

class EnvelopeGenerator {
public:
	enum EnvelopeStage {
		ENVELOPE_STAGE_OFF = 0,
		ENVELOPE_STAGE_ATTACK,
		ENVELOPE_STAGE_DECAY,
		ENVELOPE_STAGE_SUSTAIN,
		ENVELOPE_STAGE_RELEASE,
		kNumEnvelopeStages
	};
	void enterStage(EnvelopeStage newStage);
	double nextSample();
	static void setSampleRate(double newSampleRate);
	inline EnvelopeStage getCurrentStage() const { return currentStage; };
	const double minimumLevel;
	void setStageValue(EnvelopeStage stage, double value);
	void reset() {
		currentStage = ENVELOPE_STAGE_OFF;
		currentLevel = minimumLevel;
		multiplier = 1.0;
		currentSampleIndex = 0;
		nextStageSampleIndex = 0;
	}

	Signal0<> beganEnvelopeCycle;
	Signal0<> finishedEnvelopeCycle;

	EnvelopeGenerator() :
		minimumLevel(0.001),
		currentStage(ENVELOPE_STAGE_OFF),
		currentLevel(minimumLevel),
		multiplier(1.0),
		currentSampleIndex(0),
		nextStageSampleIndex(0) {
		stageValue[ENVELOPE_STAGE_OFF] = 0.0;
		stageValue[ENVELOPE_STAGE_ATTACK] = 0.0;
		stageValue[ENVELOPE_STAGE_DECAY] = 1.0;
		stageValue[ENVELOPE_STAGE_SUSTAIN] = 1.0;
		stageValue[ENVELOPE_STAGE_RELEASE] = 5.0;
	};
	~EnvelopeGenerator();
private:
	EnvelopeStage currentStage;
	double currentLevel;
	double multiplier;
	static double sampleRate;
	double stageValue[kNumEnvelopeStages];
	void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);
	unsigned long long currentSampleIndex;
	unsigned long long nextStageSampleIndex;
};