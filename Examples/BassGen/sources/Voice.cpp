#include "includes/Voice.h"

double Voice::nextSample()
{
	if (!isActive) return 0.0;

	double oscillatorOneOutput = mOscillatorOne.nextSample();
	double oscillatorTwoOutput = mOscillatorTwo.nextSample();
	double oscillatorSubOutput = mSubOscillator.nextSample();
	double oscillatorNoiseOutput = mNoiseOscillator.nextSample();
	
	float OSC1Lvl = mOscillatorOne.getSingleOSCLvlValue();
	float OSC2Lvl = mOscillatorTwo.getSingleOSCLvlValue();
	float SubOSCLvl = mSubOscillator.getSingleOSCLvlValue();
	float NoiseOSCLvl = mNoiseOscillator.getSingleOSCLvlValue();

	double oscillatorSum = (
		(oscillatorOneOutput*OSC1Lvl) + (oscillatorTwoOutput*OSC2Lvl) +
		(oscillatorSubOutput*SubOSCLvl) + (oscillatorNoiseOutput*NoiseOSCLvl));

	double volumeEnvelopeValue = mVolumeEnvelope.nextSample();
	double filterEnvelopeValue = mFilterEnvelope.nextSample();

	mFilter.setCutoffMod(filterEnvelopeValue * mFilterEnvelopeAmount + mLFOValue * mFilterLFOAmount);
	//mOscillatorOne.setPitchMod(mLFOValue * mOscOnePitchAmount);
	//mOscillatorTwo.setPitchMod(mLFOValue * mOscTwoPitchAmount);

	mOscillatorOne.setPitchMod(mOscOnePitchAmount);
	mOscillatorTwo.setPitchMod(mOscTwoPitchAmount);
	mSubOscillator.setPitchMod(mSubOscPitchAmount);

	return mFilter.process(mMasterVol * oscillatorSum * volumeEnvelopeValue * mVelocity / 127.0);
}

void Voice::setNoteNumber(int noteNumber)
{
	mNoteNumber = noteNumber;
	double frequency = 440.0 * pow(2.0, (mNoteNumber - 69.0) / 12.0);
	mOscillatorOne.setFrequency(frequency);
	mOscillatorTwo.setFrequency(frequency);
	mSubOscillator.setFrequency(frequency * 0.5);
}

void Voice::setFree(){
	isActive = false;
}

void Oscillator::setFineTuning(double amount) {
  mFineTuning = amount;
  updateIncrement();
}

void Voice::reset() 
{
	mNoteNumber = -1;
	mVelocity = 0;
	mOscillatorOne.reset();
	mOscillatorTwo.reset();
	mSubOscillator.reset();
	mVolumeEnvelope.reset();
	mFilterEnvelope.reset();
	mFilter.reset();
}

Voice::~Voice()
{
}
