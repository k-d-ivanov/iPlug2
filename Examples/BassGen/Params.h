#pragma once

const int kNumPrograms = 11;

enum EParams
{
    kMasterVol = 0,
  /* Amp Section:---------------------------------------------------------------------------*/
    kAmpAttack,
    //kAmpHold,
    kAmpDecay,
    kAmpSustain,
    kAmpRelease,
  /* Envelop Section:------------------------------------------------------------------------*/
    kEnvelopeGraph,
  /* Oscillators Section:--------------------------------------------------------------------*/
  //OSC 1------------------------------------/
    kOsc1Waveform,		
    kOsc1PitchMod,
    kOsc1VolKnob,
    kOsc1FineKnob,
    kOsc1DetuneKnob,
    kOsc1PhaseSlider,
  //OSC 2------------------------------------/
    kOsc2Waveform,		
    kOsc2PitchMod,
    kOsc2VolKnob,
    kOsc2FineKnob,
    kOsc2DetuneKnob,
    kOsc2PhaseSlider,
  //OSC Sub-----------------------------------/
    kSubOscWaveform,	
    kSubOscLvl,
  //OSC Noise---------------------------------/
    kNoiseOscLvl,		
  /* LFO Section:----------------------------------------------------------------------------*/
    kLfoAmount,
    kLfoWaveform,
    kLfoSpeedByFreq,
    kLfoSpeedByNote,
    kLfoResetGraph,
  /* Filter Section:------------------------------------------------------------------------*/
    //kFilterMode,
    kFilterCutoff,
    kFilterResonance,
    kFilterAttack,
    kFilterDecay,
    kFilterSustain,
    kFilterRelease,
    kFilterEnvelopeAmount,
  /* Buttons/Switches------------------------------------------------------------------------*/
    kOsc1OnButton,
    kOsc2OnButton,
    kOscSubOnButton,
    kOscNoiseOnButton,
    //kSwitchButtonFilter,
    kTabsButton,
  /* Presets------------------------------------------------------------------------------*/
    kPresetField,
    //kPresetNext,
    //kPresetPrevious,
    //kEnvelopeNumber,
  /* Parameters count number. */
  kNumParams
};

struct ParamPropertiesStruct {
  const char* name;
  const char* label;
  const float x;
  const float y;
  const double defaultVal;
  const double minVal;
  const double maxVal;
};

const ParamPropertiesStruct paramProps[kNumParams] =
{ //{ name,                 label              x,     y,    defaultVal, minVal, maxVal }
  { "Master Vol",          "Master",           480.0, 90.0, 0.5, 0.0, 1.0 },
  /* Amp:----------------------------------------------------------------------------------*/
  { "Amp Attack",          "Attack",           700.0, 280.0, 0.01, 0.0, 1.0 },
  //{ "Hold", 450.0, 470.0, 0.0, 0.0, 10.0 },
  { "Amp Decay",           "Decay",            750.0, 280.0, 0.3, 0.0, 1.0 },
  { "Amp Sustain",         "Sustain",          700.0, 350.0, 1.0, 0.0, 1.0 },
  { "Amp Release",         "Release",          750.0, 350.0, 0.01, 0.0, 1.0 },
  /* Envelope:------------------------------------------------------------------------------*/
  { "Envelope",            "Envelope",         228.0, 440.0, 0.0 }, 
  /* OSC1:----------------------------------------------------------------------------------*/
  { "OSC1 Form",           "Shape",            230.0, 150.0, 1.0 },
  { "OSC1 Pitch",          "Pitch",            30.0, 90.0, 0.5, 0.0, 1.0 }, 
  { "OSC1 Lvl",            "Level",            120.0, 168.0, 0.8, 0.0, 1.0 }, 
  { "Fine knob",           "Fine",             30.0, 144.0, 0.0, 0.0, 1.0 }, 
  { "Detune knob",         "Detune",           30.0, 196.0, 0.0, 0.0, 1.0 }, 
  { "OSC1 Phase Slider",   "Phase",            98.0, 84.0, 0.0, 0.0, 1.0 }, 
  /* OSC2:----------------------------------------------------------------------------------*/
  { "OSC2 Form",           "Shape",            591.0, 150.0, 0.0 },
  { "OSC2 Pitch",          "Pitch",            960.0, 90.0, 0.5, 0.0, 1.0 }, 
  { "OSC2 Lvl",            "Level",            840.0, 168.0, 0.8, 0.0, 1.0}, 
  { "Fine knob",           "Fine",             960.0, 144.0, 0.0, 0.0, 1.0 }, 
  { "Detune knob",         "Detune",           960.0, 196.0, 0.0, 0.0, 1.0 },
  { "OSC2 Phase Slider",   "Phase",            810.0, 84.0, 0.0, 0.0, 1.0 }, 
  /* SUBOSC:--------------------------------------------------------------------------------*/
  { "Sub OSC Form",        "Shape",            822.0, 276.0, 1.0 },
  { "Sub OSC Lvl",         "Level",            820.0, 340.0, 0.0, 0.0, 1.0 }, 
  { "Noise OSC Lvl",       "Level",            818.0, 414.0, 0.0, 0.0, 1.0 }, 
  /* LFOs:----------------------------------------------------------------------------------*/
  { "LFO Amount",          "Amount",           705.0, 280.0, 0.0, 0.0, 1.0 }, 
  { "LFO Form",            "Shape",            228.0, 440.0, 0.0 }, 
  { "LFO SpeedFq",         "Hz",               705.0, 350.0, 0.0, 0.0, 1.0 }, 
  { "LFO SpeedNote ",      "Speed",            760.0, 322.0, 0.0, 0.0, 1.0 },
  { "LFO Reset ",          "",                 690.0, 438.0, 0.0, 0.0, 1.0 },
  /* Filter:--------------------------------------------------------------------------------*/
  //{ "Filter Mode", 300, 310 }, // Filter Mode
  { "Filter Cutoff",      "Cutoff",            21.0, 340.0, 1.0, 0.0, 0.99 },
  { "Filter Resonance",   "Resonance",         90.0, 340.0, 0.1, 0.0, 1.0 }, 
  { "Filter Attack",      "Attack",            20.0, 410.0, 0.01, 0.0, 1.0 },
  { "Filter Decay",       "Decay",             67.0, 410.0, 0.5, 0.0, 1.0 },
  { "Filter Sustain",     "Sustain",           116.0, 410.0, 1.0, 0.0, 1.0 }, 
  { "Filter Release",     "Release",           168.0, 410.0, 0.01, 0.0, 1.0 }, 
  { "Filter EnvAmount",   "Amount",            161.0, 340.0, 0.0, -1.0, 1.0 },
  /* ON-OFF Butt----------------------------------------------------------------------------*/
  {"On/Off OSC1",         "",                  192.0, 59.0, 1.0, 0.0, 1.0 },
  {"On/Off OSC2",         "",                  801.0, 59.0, 0.0, 0.0, 1.0 },
  {"On/Off SubOsc",       "",                  801.0, 242.0, 0.0, 0.0, 1.0 },
  {"On/Off NoiseOsc",     "",                  801.0, 387.0, 0.0, 0.0, 1.0 },
  //{"On/Off Filter",     "",                  182.0, 253.0, 0.0 },
  {"Tabs buttons",        "",                  222.0, 242.0, 0.0 },
  /* Presets--------------------------------------------------------------------------------*/
  { "Presets field",      "",                  720.0, 0.0, 0.0 }
  //{ "Preset Next",      "",                  750.0, 30.0, 0.0 },
  //{ "Preset Previous",  "",                  800.0, 30.0, 0.0 },
  //{ "EnvNum",           "",                  705.0, 350.0, 0.0 }
};

/* Frames used in bitmap controls */
const int smallKnobFrames = 100;
const int waveformBitmapFrames = 4;
const int switchOnOffFrames = 2;
const int oscLvlKnobFrames = 100;
const int tabsButtFrames = 5;