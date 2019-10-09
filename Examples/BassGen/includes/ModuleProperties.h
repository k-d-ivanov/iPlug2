#pragma once

struct ParamProperties {
  const char* name;
  const float x;
  const float y;
  const double defaultVal;
  const double minVal;
  const double maxVal;
};

constexpr int Osc1ParamNumber = 3;
constexpr int Osc2ParamNumber = 3;
constexpr int SubOscParamNumber = 2;
constexpr int NoiseOscParamNumber = 1;
//constexpr int WaveOsc1ParamNumber = 3;
//constexpr int WaveOsc2ParamNumber = 3;

/** OSC1:----------------------------------------------------------------------------------*/
ParamProperties Osc1Params[Osc1ParamNumber] = {
  { "OSC1 Form", 222.0f, 148.0f, 1.0f},
  { "OSC1 Pitch", 30.0f, 90.0f, 0.5f, 0.1f, 1.0f },
  { "OSC1 Lvl", 120.0f, 168.0f, 0.8f, 0.01f, 1.0f}
};

/** OSC2:----------------------------------------------------------------------------------*/
ParamProperties Osc2Params[Osc2ParamNumber] = {
  { "OSC2 Form", 602.0f, 148.0f, 0.0f},
  { "OSC2 Pitch", 826.0f, 90.0f, 0.5f, 0.1f, 1.0f },
  { "OSC1 Lvl", 840.0f, 168.0f, 0.8f, 0.01f, 1.0f }
};

/** SUBOSC:--------------------------------------------------------------------------------*/
ParamProperties SubOscParams[SubOscParamNumber] = {
  { "Sub OSC Form", 832.0f, 278.0f, 1.0f },
  { "Sub OSC Lvl", 828.0f, 340.0f, 0.0f, 0.01f, 1.0f }
};

/** NoiseOSC:--------------------------------------------------------------------------------*/
ParamProperties NoiseOscParams[NoiseOscParamNumber] = {
  { "Noise OSC Lvl", 828.0f, 410.0f, 0.0f, 0.01f, 1.0f }
};

