#pragma once //__BASSGEN__VOICEMANAGER__

#include "Voice.h"
//#include <tr1/functional> 
//if that doesn't work
#include <functional> 

class VoiceManager {
public:
  VoiceManager();

	// Functions to change a single voice:
  static void setMasterVolume(Voice& voice, double value);

  static void setVolumeEnvelopeStageValue(Voice& voice, EnvelopeGenerator::EnvelopeStage stage, double value);
  static void setFilterEnvelopeStageValue(Voice& voice, EnvelopeGenerator::EnvelopeStage stage, double value);

  static void setOscillatorMode(Voice& voice, int oscillatorNumber, Oscillator::OscillatorMode mode);
  static void setOscillatorPitchMod(Voice& voice, int oscillatorNumber, double amount);
  static void setSingleOSCVolumeLvl(Voice& voice, int paramNumber, float lvlAmount);

  static void setOscillatorFineTune(Voice& voice, int oscillatorNumber, double amount);

  static void setFilterCutoff(Voice& voice, double cutoff);
  static void setFilterResonance(Voice& voice, double resonance);
  static void setFilterMode(Voice& voice, Filter::FilterMode mode);
  static void setFilterEnvAmount(Voice& voice, double amount);
  static void setFilterLFOAmount(Voice& voice, double amount);

	void onNoteOn(int noteNumber, int velocity);
	void onNoteOff(int noteNumber, int velocity);
	double nextSample();

	void setSampleRate(float sampleRate)
  {
		EnvelopeGenerator::setSampleRate(sampleRate);
		for (int i = 0; i < NumberOfVoices; i++)
    {
			Voice& voice = voices[i];
			voice.mOscillatorOne.setSampleRate(sampleRate);
			voice.mOscillatorTwo.setSampleRate(sampleRate);
      voice.mSubOscillator.setSampleRate(sampleRate);
      voice.mNoiseOscillator.setSampleRate(sampleRate);
		}
		mLFO.setSampleRate(sampleRate);
	}
	void setLFOMode(LfoModes mode) { mLFO.setMode(mode); };
	void setLFOFrequency(double frequency) { mLFO.setFrequency(frequency); };

	typedef std::function<void (Voice &)> VoiceChangerFunction;

	inline void changeAllVoices(VoiceChangerFunction changer)
  {
		for (int i = 0; i < NumberOfVoices; i++)
    {
			changer(voices[i]);
		}
	}

	~VoiceManager();
private:
	static const int NumberOfVoices = 2;
	Voice voices[NumberOfVoices];
  LfoGenerator mLFO;
	Voice* findFreeVoice();
};