#include "IPlugMidiEffect.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

IPlugMidiEffect::IPlugMidiEffect(IPlugInstanceInfo instanceInfo)
: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
{
  GetParam(kParamGain)->InitDouble("Gain", 100., 0., 100.0, 0.01, "%");
  
  SetTailSize(4410000);

#if IPLUG_EDITOR // All UI methods and member variables should be within an IPLUG_EDITOR guard, should you want distributed UI
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, 1.);
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    
    auto actionFunc = [&](IControl* pCaller) {
      
      IMidiMsg msg;
      
      static bool onoff = false;
      
      onoff = !onoff;
      
      if(onoff)
        msg.MakeNoteOnMsg(60, 60, 0);
      else
        msg.MakeNoteOffMsg(60, 0);
      
      SendMidiMsgFromUI(msg);
      
      FlashCircleClickActionFunc(pCaller);
    };
    
    pGraphics->LoadFont(ROBOTTO_FN);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->AttachControl(new IVButtonControl(pGraphics->GetBounds().GetPadded(-10), actionFunc, "Trigger Chord"));
  };
#endif
}

#if IPLUG_DSP
void IPlugMidiEffect::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const double gain = GetParam(kParamGain)->Value() / 100.;
  const int nChans = NOutChansConnected();

  for (auto s = 0; s < nFrames; s++) {
    for (auto c = 0; c < nChans; c++) {
      outputs[c][s] = outputs[c][s] * gain;
    }
  }
}

void IPlugMidiEffect::ProcessMidiMsg(const IMidiMsg& msg)
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
    {
      goto handle;
    }
    default:
      return;
  }
  
handle:
  SendMidiMsg(msg);
}
#endif
