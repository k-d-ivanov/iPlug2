#pragma once //__BASSGEN__
#pragma warning( suppress : 4101 4129 )

#include "IPlug_include_in_plug_hdr.h" //Important Framework header.

#include "Params.h" // contains Enums params
#include "includes/MIDIReceiver.h"
#include "includes/VoiceManager.h"

using namespace iplug;
using namespace igraphics;

class BassGen : public Plugin
{
public:
  BassGen(const InstanceInfo& info); // Main plugin constructor.

#if IPLUG_DSP // All DSP methods and member variables should be within an IPLUG_DSP guard, should you want distributed UI
public:
  // Needed for the GUI keyboard:
 // Should return non-zero if one or more keys are playing.
  inline int GetNumKeys() const { return mMIDIReceiver.getNumKeys(); };
  // Should return true if the specified key is playing.
  inline bool GetKeyStatus(int key) const { return mMIDIReceiver.getKeyStatus(key); };

  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;
#endif

private:
  const double plugSampleRate = 48000.0;
  const std::vector<int> MasterBlock{ kMasterVol };
  const std::vector<int> Osc1Block{ kOsc1Waveform, kOsc1PitchMod, kOsc1VolKnob, kOsc1FineKnob, kOsc1DetuneKnob, kOsc1PhaseSlider, kOsc1OnButton };
  const std::vector<int> Osc2Block{ kOsc2Waveform, kOsc2PitchMod, kOsc2VolKnob, kOsc2FineKnob, kOsc2DetuneKnob, kOsc2PhaseSlider, kOsc2OnButton };
  const std::vector<int> SubOscBlock{ kSubOscWaveform,	kSubOscLvl, kOscSubOnButton };
  const std::vector<int> NoiseOscBlock{ kNoiseOscLvl, kOscNoiseOnButton };
  const std::vector<int> EnvelopeBlock{ kAmpAttack, kAmpDecay, kAmpSustain, kAmpRelease};
  const std::vector<int> FilterBlock{ kFilterCutoff, kFilterResonance, kFilterAttack, kFilterDecay,
                                kFilterSustain, kFilterRelease, kFilterEnvelopeAmount, };
  const std::vector<int> LfoBlock{ kLfoAmount, kLfoSpeedByFreq, kLfoSpeedByNote, kLfoWaveform, kLfoResetGraph };
  const std::vector<int> TabsBlock{ kTabsButton };
  const std::vector<int> PresetsBlock{ kPresetField };
  
  //TODO methods for other blocks e. g. void OnChangeFilter();

 void OnChangeMasterBlock(int& paramIdx);
 void OnChangeOsc1Block(int& paramIdx);
 void OnChangeOsc2Block(int& paramIdx);
 void OnChangeSubOscBlock(int& paramIdx);
 void OnChangeNoiseOscBlock(int& paramIdx);
 void OnChangeEnvelopeBlock(int & paramIdx);
 void OnChangeFilterBlock(int& paramIdx);
 void OnChangePresetsBlock(int& paramIdx);
 void OnChangeLfoBlock(int& paramIdx);
 void OnChangeTabsBlock(int& paramIdx);
 
  void CreateParams();

  void CreateAllGraphics(IGraphics* pGraphics);

  MIDIReceiver mMIDIReceiver;
  VoiceManager voiceManager;
  VoiceManager::VoiceChangerFunction changer;
};

