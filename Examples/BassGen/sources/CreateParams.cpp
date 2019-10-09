#include "BassGen.h"

void BassGen::CreateParams() {

  const double parameterStep = 0.01;

  for (int i = 0; i < kNumParams; ++i)
  {
    const ParamPropertiesStruct properties = paramProps[i];
    switch (i)
    {
    /* OSCs params:----------------------------------------------------------------------------------*/
    case kOsc1Waveform: case kOsc2Waveform: case kSubOscWaveform:
      GetParam(i)->InitEnum(properties.name, properties.defaultVal, Oscillator::kNumOscillatorModes);
      break;

    case kOsc1PhaseSlider: case kOsc2PhaseSlider:
      GetParam(i)->InitDouble(properties.name, properties.defaultVal, properties.minVal, properties.maxVal, parameterStep);
      break;
  /* Filter params:--------------------------------------------------------------------------------*/
  /*case kFilterMode:
    param->InitEnum(properties.name, Filter::FILTER_MODE_LOWPASS, Filter::kNumFilterModes,"%","3");
    param->SetDisplayText(0, properties.name);
    break;*/

  /* Switches:--------------------------------------------------------------------------------------*/
    case kOsc1OnButton: case kOsc2OnButton: case kOscSubOnButton: case kOscNoiseOnButton: case kLfoResetGraph:
      GetParam(i)->InitBool(properties.name, properties.defaultVal);
      break;

    case kTabsButton: //TODO: make custom Tabs control.
      GetParam(i)->InitEnum(properties.name, properties.defaultVal, 5);
      break;

    case kPresetField:
      GetParam(i)->InitEnum(properties.name, properties.defaultVal, kNumPrograms);
      break;
    /*case kPresetNext: case kPresetPrevious: param->InitBool(properties.name, properties.defaultVal); break;*/

    default:
      /* All other Double Parameters------------------------------------------------------------------ */
      GetParam(i)->InitDouble(properties.name, properties.defaultVal,
        properties.minVal, properties.maxVal, parameterStep);
      break;
    }
  }
}
