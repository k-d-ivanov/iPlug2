#include "Params.h"
#include "includes/VoiceManager.h"

Voice* VoiceManager::findFreeVoice() {
	Voice* freeVoice = nullptr;
	for (int i = 0; i < NumberOfVoices; i++) {
		if (!voices[i].isActive) {
			freeVoice = &(voices[i]);
			break;
		}
	}
	return freeVoice;
}

void VoiceManager::setSingleOSCVolumeLvl(Voice & voice, int paramNumber, float lvlAmount)
{
  if (paramNumber == kOsc1VolKnob) voice.mOscillatorOne.setSingleOSCLvlValue(lvlAmount);
  if (paramNumber == kOsc2VolKnob) voice.mOscillatorTwo.setSingleOSCLvlValue(lvlAmount);
  if (paramNumber == kSubOscLvl) voice.mSubOscillator.setSingleOSCLvlValue(lvlAmount);
  if (paramNumber == kNoiseOscLvl) voice.mNoiseOscillator.setSingleOSCLvlValue(lvlAmount);
}

void VoiceManager::setOscillatorFineTune(Voice & voice, int oscillatorNumber, double amount)
{
  if(oscillatorNumber == 1) voice.mOscillatorOne.setFineTuning(amount);
  if (oscillatorNumber == 2) voice.mOscillatorTwo.setFineTuning(amount);
}

void VoiceManager::setFilterCutoff(Voice& voice, double cutoff)
{
  voice.mFilter.setCutoff(cutoff);
}

void VoiceManager::setFilterResonance(Voice& voice, double resonance)
{
  voice.mFilter.setResonance(resonance);
}

void VoiceManager::setFilterMode(Voice& voice, Filter::FilterMode mode)
{
  voice.mFilter.setFilterMode(mode);
}

void VoiceManager::setFilterEnvAmount(Voice& voice, double amount)
{
  voice.setFilterEnvelopeAmount(amount);
}

void VoiceManager::setFilterLFOAmount(Voice& voice, double amount)
{
  voice.setFilterLFOAmount(amount);
}

void VoiceManager::onNoteOn(int noteNumber, int velocity)
{
	Voice* voice = findFreeVoice();
	if (!voice) return;

	voice->reset();
	voice->setNoteNumber(noteNumber);
	voice->mVelocity = velocity;
	voice->isActive = true;
	voice->mVolumeEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
	voice->mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
}

void VoiceManager::onNoteOff(int noteNumber, int velocity)
{
	// Find the voice(s) with the given noteNumber:
	for (int i = 0; i < NumberOfVoices; ++i)
  {
		Voice& voice = voices[i];
		if (voice.isActive && voice.mNoteNumber == noteNumber)
    {
			voice.mVolumeEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
			voice.mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
			voice.isActive = false;
		}
	}
}

double VoiceManager::nextSample() {
	double output = 0.0;
	double lfoValue = mLFO.nextSample();

	for (int i = 0; i < NumberOfVoices; i++)
  {
		Voice& voice = voices[i];
		//voice.setLFOValue(lfoValue);
		output += voice.nextSample();
	}
	return output;
}

VoiceManager::VoiceManager()
{
}

// Functions to change a single voice:

void VoiceManager::setMasterVolume(Voice& voice, double value)
{
	voice.setMasterVolValue(value);
}

void VoiceManager::setVolumeEnvelopeStageValue(Voice& voice, EnvelopeGenerator::EnvelopeStage stage, double value)
{
	voice.mVolumeEnvelope.setStageValue(stage, value);
}

void VoiceManager::setFilterEnvelopeStageValue(Voice& voice, EnvelopeGenerator::EnvelopeStage stage, double value)
{
	voice.mFilterEnvelope.setStageValue(stage, value);
}

void VoiceManager::setOscillatorMode(Voice& voice, int oscillatorNumber, Oscillator::OscillatorMode mode)
{
  switch (oscillatorNumber)
  {
  case 1:
    voice.mOscillatorOne.setMode(mode);
    break;
  case 2:
    voice.mOscillatorTwo.setMode(mode);
    break;
  case 3:
    voice.mSubOscillator.setMode(mode);
    break;
  case 4:
    voice.mNoiseOscillator.setMode(mode);
    break;
  default:
    break;
  }
}

void VoiceManager::setOscillatorPitchMod(Voice& voice, int oscillatorNumber, double amount)
{
  switch (oscillatorNumber)
  {
  case 1:
    voice.setOscOnePitchAmount(amount);
    break;
  case 2:
    voice.setOscTwoPitchAmount(amount);
    break;
  default:
    break;
  }
}

VoiceManager::~VoiceManager()
{
}
