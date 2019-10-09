#include "BassGen.h"
#include "IControls.h"

/* Custom controls include*/
#include "CairoControls\CairoEnvelopeGraph.h"
#include "CairoControls\CairoOSCWaveSwitch.h"
#include "CairoControls\CairoLfoGraph.h"

//#include "EnvelopeNumber.h"//TODO: Move this file CairoEnvelopeGraph location

//#include "CairoControls\CairoSPAN.h" //TODO uncoment when control is implemented.
// TODO FILTER VISualization

#include "IKeyboardControl.h"
#include "Knob.h"

void BassGen::CreateAllGraphics(IGraphics* pGraphics)
{
  const IBitmap smallKnob = pGraphics->LoadBitmap(KNOB_FN, smallKnobFrames);
  const IBitmap waveformBitmap = pGraphics->LoadBitmap(WAVEFORM_SW_FN, waveformBitmapFrames);
  const IBitmap switchOnOff = pGraphics->LoadBitmap(ON_OFF_SWITCH_FN, switchOnOffFrames);
  const IBitmap oscLvlKnob = pGraphics->LoadBitmap(OSC_LVL_KNOB_FN, oscLvlKnobFrames);
  const IBitmap tabsButt = pGraphics->LoadBitmap(TABS_BUTTON_FN, tabsButtFrames);

  /* Virtual MIDI Keyboard. ------------------------------------------------------------------------------*/
  const IBitmap regular = pGraphics->LoadBitmap(WHITE_KEY_FN, 6);
  const IBitmap sharp = pGraphics->LoadBitmap(BLACK_KEY_FN, 1);

  // Keyboard key X coordinates.
  //                          C#	   D#          F#      G#      A#
  const int coords[12] = { 0, 8, 14, 22, 28, 42, 50, 56, 64, 70, 78, 84 };
  //int coords[12] = { 8, 14, 22, 28, 42, 50, 56, 64, 70, 78, 84, 92 }; //shifted coord 
  IControl* mVirtualKeyboard = new IKeyboardControl(262, 534, 12, 5, regular, sharp, coords);
  pGraphics->AttachControl(mVirtualKeyboard);
  /*-----------------------------------------------------------------------------------------------------*/

  const IVStyle style{
                    false, // Show label
                    true, // Show value
                    {
                      DEFAULT_BGCOLOR, // Background
                      DEFAULT_FGCOLOR, // Foreground
                      DEFAULT_PRCOLOR, // Pressed
                      COLOR_DARK_GRAY, // Frame
                      DEFAULT_HLCOLOR, // Highlight
                      DEFAULT_SHCOLOR, // Shadow
                      IColor(250, 160, 0, 0), // Extra 1
                      DEFAULT_X2COLOR, // Extra 2
                      DEFAULT_X3COLOR,  // Extra 3
                    }, // Colors
                    IText(12.0F, EAlign::Center),
                    IText(12.0F, COLOR_WHITE, "Roboto-Regular", EAlign::Center, EVAlign::Bottom) // Label text
  };

  for (int i = 0; i < kNumParams; ++i)
  {
    const ParamPropertiesStruct properties = paramProps[i];

    //pGraphics->GetDelegate()->GetParam(i)->Value();
    if (i == kMasterVol) pGraphics->AttachControl(new Knob(pGraphics, properties.name, properties.label, properties.x, properties.y, oscLvlKnob, i), i);
  

    /* Amp Graphics/Controls:---------------------------------------------------------------------------*/
    if (i == kAmpAttack || i == kAmpDecay || i == kAmpSustain || i == kAmpRelease)
    {
      pGraphics->AttachControl(new Knob(pGraphics, properties.name, properties.label, properties.x, properties.y, smallKnob, i), i, "envControlGroup");
    }

  /* OSCs Graphics/Controls:--------------------------------------------------------------------------*/
    if (i == kOsc1Waveform)
    {
      pGraphics->AttachControl(new CairoOSCWaveSwitch(
        GetUI(), IRECT(212, 72, 428, 230), i,
        properties.x, properties.y, properties.defaultVal), i);
    }

    if (i == kOsc1VolKnob || i == kOsc2VolKnob)
    {
      pGraphics->AttachControl(new Knob(pGraphics,properties.name, properties.label, properties.x, properties.y, oscLvlKnob, i), i);
    }

    if (i == kOsc2Waveform)
    {
      pGraphics->AttachControl(new CairoOSCWaveSwitch(
        GetUI(), IRECT(592, 72, 808, 230), i,
        properties.x, properties.y, properties.defaultVal), i);
    }

    if (i == kSubOscWaveform) pGraphics->AttachControl(new IBSwitchControl(properties.x, properties.y, waveformBitmap, i), kSubOscWaveform);

    /* Envelope--------------------------------------------------------------------------------*/

    if (i == kEnvelopeGraph)
    {
      pGraphics->AttachControl(new CairoEnvelopeGraph(
        GetUI(), IRECT(properties.x, 265, 682, properties.y), i), kEnvelopeGraph, "envControlGroup");
    }

    //if (i == kEnvelopeNumber) {
    //  pGraphics->AttachControl(new EnvelopeNumber(
    //    IRECT(properties.x, properties.y + 40.0, properties.x + 40.0, properties.y + 80)), i, "envControlGroup");
    //}

  /* LFOs Graphics/Controls:----------------------------------------------------------------*/
    if (i == kLfoWaveform)
    {
      pGraphics->AttachControl(new CairoLfoGraph(
        GetUI(), IRECT(properties.x, 265, 682, properties.y), i), i, "lfoControlGroup");
      pGraphics->GetControlWithTag(i)->Hide(true);
    }

    if (i == kLfoAmount || i == kLfoSpeedByFreq)
    {
      pGraphics->AttachControl(new Knob(pGraphics, properties.name, properties.label, properties.x, properties.y, smallKnob, i),
        i, "lfoControlGroup");
      pGraphics->GetControlWithTag(i)->Hide(true);
    }

  /* Filter Graphics/Controls:----------------------------------------------------------------*/
    //TODO: if statement for  kFilterMode: new ISwitchControl(this, properties.x, properties.y, i, graphic);
    if (i == kOsc1PhaseSlider)
    {
      pGraphics->AttachControl(new IVSliderControl(IRECT(properties.x, properties.y, 192.0F, 124.0F), kOsc1PhaseSlider,
        "Phase", style, false, EDirection::Horizontal), kOsc1PhaseSlider, "PhaseSlider");
    }

    if (i == kOsc2PhaseSlider)
    {
      pGraphics->AttachControl(new IVSliderControl(IRECT(properties.x, properties.y, 904.0F, 124.0F), kOsc2PhaseSlider,
        "Phase", style, false, EDirection::Horizontal), kOsc2PhaseSlider, "PhaseSlider2");
    }

  /* Switch/Button Graphics/Controls:---------------------------------------------------------*/
    if (i == kOsc1OnButton || i == kOsc2OnButton || i == kOscSubOnButton || i == kOscNoiseOnButton)
    {
      pGraphics->AttachControl(new IBSwitchControl(properties.x, properties.y, switchOnOff, i), i);
    }

    if (i == kTabsButton) pGraphics->AttachControl(new IBSwitchControl(properties.x, properties.y, tabsButt, i), i);

    if (i == kLfoResetGraph)
    {
      auto resetAction = [pGraphics](IControl* pCaller)
      {
      };
      pGraphics->AttachControl(
        new IVButtonControl(IRECT(properties.x, properties.y, properties.x + 108, properties.y + 28), resetAction, "Reset"), i);
      pGraphics->GetControlWithTag(i)->Hide(true);
    }
  // TODO: if statement for kSwitchButtonFilter here.

  /* Presets ---------------------------------------------------------------------------------*/
    if (i == kPresetField)
    {
      pGraphics->AttachControl(new IVSwitchControl(
        IRECT(properties.x, properties.y,
          properties.x + 240.0F, properties.y + 50.0F), i));
    }
  /* TODO: if statement for kPresetNext, kPresetPrevious
     new IVButtonControl(IRECT(properties.x, properties.y, properties.x + 40.0f, properties.y + 25.0f));
  */

  /* Knobs Graphics/Controls:---------------------------------------------------------------*/
    if (i == kOsc1PitchMod
      || i == kOsc1FineKnob
      || i == kOsc1DetuneKnob
      || i == kOsc2PitchMod
      || i == kOsc2FineKnob
      || i == kOsc2DetuneKnob
      || i == kSubOscLvl
      || i == kNoiseOscLvl
      //|| i == kFilterMode
      || i == kFilterCutoff
      || i == kFilterResonance
      || i == kFilterAttack
      || i == kFilterDecay
      || i == kFilterSustain
      || i == kFilterRelease
      || i == kFilterEnvelopeAmount
      )
    {
      pGraphics->AttachControl(new Knob(pGraphics, properties.name, properties.label, properties.x, properties.y, smallKnob, i), i);
    }

  }
  
}
