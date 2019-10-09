#include "BassGen.h"
#include "IPlug_include_in_plug_src.h"

BassGen::BassGen(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPrograms))
{
  //Initialize params.
  CreateParams();

#if IPLUG_EDITOR // All UI methods and member variables should be within an IPLUG_EDITOR guard, should you want distributed UI

/* Plugin Window--------------------------------------------------------*/
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, 1.);
  };

/*----------------------------------------------------------------------*/

/* Plugin Layout--------------------------------------------------------*/
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->HandleMouseOver(true);
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachBackground(BG_FN);
    pGraphics->EnableTooltips(true);
    pGraphics->AttachTextEntryControl();
    pGraphics->AttachPopupMenuControl();
    pGraphics->LoadFont("Roboto-Regular", ROBOTTO_FN);

    CreateAllGraphics(pGraphics);
  };
/*----------------------------------------------------------------------*/
#endif
  //MakeDefaultPreset("Init",1);
  mMIDIReceiver.noteOn.Connect(&voiceManager, &VoiceManager::onNoteOn);
  mMIDIReceiver.noteOff.Connect(&voiceManager, &VoiceManager::onNoteOff);
}

#if IPLUG_DSP //DSP Guard 

void BassGen::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int nChans = NOutChansConnected();

  for (auto s = 0; s < nFrames; s++) {
    for (auto c = 0; c < nChans; c++) {
      mMIDIReceiver.advance();
      outputs[c][s] = voiceManager.nextSample();   
    }
  }
  mMIDIReceiver.Flush(nFrames);
}

void BassGen::OnIdle()
{
}

void BassGen::OnReset()
{
  TRACE;
  
  voiceManager.setSampleRate(plugSampleRate);
}

void BassGen::ProcessMidiMsg(const IMidiMsg& msg)
{
  TRACE;
  
  int status = msg.StatusMsg();

  switch (status)
  {
    case IMidiMsg::kNoteOn:
    case IMidiMsg::kNoteOff:
    case IMidiMsg::kPolyAftertouch:
    case IMidiMsg::kControlChange:
    case IMidiMsg::kProgramChange:
    case IMidiMsg::kChannelAftertouch:
    case IMidiMsg::kPitchWheel:
      mMIDIReceiver.onMessageReceived(msg);
      break;
    default:
      return;
      break;
  }
  SendMidiMsg(msg);
}

void BassGen::OnParamChange(int paramIdx)
{
  if (std::find(MasterBlock.begin(), MasterBlock.end(), paramIdx) != MasterBlock.end()) OnChangeMasterBlock(paramIdx);
  
  if (std::find(Osc1Block.begin(), Osc1Block.end(), paramIdx) != Osc1Block.end()) OnChangeOsc1Block(paramIdx);

  if (std::find(Osc2Block.begin(), Osc2Block.end(), paramIdx) != Osc2Block.end()) OnChangeOsc2Block(paramIdx);

  if (std::find(SubOscBlock.begin(), SubOscBlock.end(), paramIdx) != SubOscBlock.end()) OnChangeSubOscBlock(paramIdx);

  if (std::find(NoiseOscBlock.begin(), NoiseOscBlock.end(), paramIdx) != NoiseOscBlock.end()) OnChangeNoiseOscBlock(paramIdx);

  if (std::find(EnvelopeBlock.begin(), EnvelopeBlock.end(), paramIdx) != EnvelopeBlock.end()) OnChangeEnvelopeBlock(paramIdx);

  if (std::find(FilterBlock.begin(), FilterBlock.end(), paramIdx) != FilterBlock.end()) OnChangeFilterBlock(paramIdx);

  if (std::find(PresetsBlock.begin(), PresetsBlock.end(), paramIdx) != PresetsBlock.end()) OnChangePresetsBlock(paramIdx);

  if (std::find(LfoBlock.begin(), LfoBlock.end(), paramIdx) != LfoBlock.end()) OnChangeLfoBlock(paramIdx);

  //if (std::find(TabsBlock.begin(), TabsBlock.end(), paramIdx) != TabsBlock.end()) OnChangeTabsBlock(paramIdx);
  
  voiceManager.changeAllVoices(changer);
}
#endif
