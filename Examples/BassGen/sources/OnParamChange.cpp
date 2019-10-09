#include "BassGen.h"
#include "IControls.h"   // needed for GetUI()
#include "CairoControls/CairoLfoGraph.h"

void BassGen::OnChangeMasterBlock(int& paramIdx)
{
  if (paramIdx == kMasterVol)
  {
    changer = [&](Voice& voices) {
      VoiceManager::setMasterVolume(voices, GetParam(paramIdx)->Value());
    };
  }
}

void BassGen::OnChangeOsc1Block(int& paramIdx)
{
  //OSC Section-------------------------------------------------------------------------|
  if (paramIdx == kOsc1Waveform)
  {
    if (GetParam(kOsc1OnButton)->Int() == 1) {
      changer = [&](Voice& voices) {
        VoiceManager::setOscillatorMode(voices, 1,
          static_cast<Oscillator::OscillatorMode>(GetParam(kOsc1Waveform)->Int()));
      };
    }
    else {
      changer = [&](Voice& voices) { VoiceManager::setOscillatorMode(voices, 1,
        static_cast<Oscillator::OscillatorMode>(0));

      };
    }
  }
  
  if (paramIdx == kOsc1PitchMod) {
    changer = [&](Voice& voices) { VoiceManager::setOscillatorPitchMod(voices, 1, GetParam(paramIdx)->Value()); };
  }

  if (paramIdx == kOsc1VolKnob) {
    changer = [&](Voice& voices) {
      VoiceManager::setSingleOSCVolumeLvl(voices, paramIdx, GetParam(paramIdx)->Value());
    };
  }
  if (paramIdx == kOsc1FineKnob) {
    changer = [&](Voice& voices) { VoiceManager::setOscillatorFineTune(voices, 1, GetParam(paramIdx)->Value());
    };
  }

  if (paramIdx == kOsc1OnButton) {
    changer = [&](Voice& voices) {
      (GetParam(kOsc1OnButton)->Bool())
        ? VoiceManager::setOscillatorMode(voices, 1, static_cast<Oscillator::OscillatorMode>(1))
        : VoiceManager::setOscillatorMode(voices, 1, static_cast<Oscillator::OscillatorMode>(0));
    };
  }

  if (paramIdx == kOsc1PhaseSlider) {
    //TODO: logic for this onparamchange
    changer = [&](Voice& voices) { VoiceManager::setOscillatorPitchMod(voices, 1, GetParam(paramIdx)->Value()); };
  }
}

void BassGen::OnChangeOsc2Block(int& paramIdx)
{
    if (paramIdx == kOsc2Waveform) {
      if (GetParam(kOsc2OnButton)->Int() == 1) {
        changer = [&](Voice& voices) { VoiceManager::setOscillatorMode(voices, 2,
          static_cast<Oscillator::OscillatorMode>(GetParam(paramIdx)->Int()));
        };
      }
      else {
        changer = [&](Voice& voices) { VoiceManager::setOscillatorMode(voices, 2,
          static_cast<Oscillator::OscillatorMode>(0));
        };
      }
    }

    if (paramIdx == kOsc2PitchMod) {
      changer = [&](Voice& voices) { VoiceManager::setOscillatorPitchMod(voices, 2, GetParam(paramIdx)->Value()); };
    }

    if (paramIdx == kSubOscWaveform) {
      if (GetParam(kOscSubOnButton)->Int() == 1) {
        changer = [&](Voice& voices) { VoiceManager::setOscillatorMode(voices, 3,
          static_cast<Oscillator::OscillatorMode>(GetParam(paramIdx)->Int()));
        };
      }
      else {
        changer = [&](Voice& voices) { VoiceManager::setOscillatorMode(voices, 3,
          static_cast<Oscillator::OscillatorMode>(0));
        };
      }
    }

    if (paramIdx == kOsc2VolKnob) {
      changer = [&](Voice& voices) {
        VoiceManager::setSingleOSCVolumeLvl(voices, paramIdx, GetParam(paramIdx)->Value());
      };
    }

    if (paramIdx == kOsc2OnButton) {
      changer = [&](Voice& voices) {
        (GetParam(kOsc2OnButton)->Bool())
          ? VoiceManager::setOscillatorMode(voices, 2, static_cast<Oscillator::OscillatorMode>(1))
          : VoiceManager::setOscillatorMode(voices, 2, static_cast<Oscillator::OscillatorMode>(0));
      };
    }

    if (paramIdx == kOsc1FineKnob) {
      changer = [&](Voice& voices) { VoiceManager::setOscillatorFineTune(voices, 1, GetParam(paramIdx)->Value());
      };
    }
}

void BassGen::OnChangeSubOscBlock(int& paramIdx)
{

  if (paramIdx == kSubOscLvl) {
    changer = [&](Voice& voices) {
      VoiceManager::setSingleOSCVolumeLvl(voices, paramIdx, GetParam(paramIdx)->Value());
    };
  }

  if (paramIdx == kOscSubOnButton) {
    changer = [&](Voice& voices) {
      (GetParam(kOscSubOnButton)->Bool())
        ? VoiceManager::setOscillatorMode(voices, 3, static_cast<Oscillator::OscillatorMode>(1))
        : VoiceManager::setOscillatorMode(voices, 3, static_cast<Oscillator::OscillatorMode>(0));
    };
  }
}

void BassGen::OnChangeNoiseOscBlock(int& paramIdx)
{
  if (paramIdx == kNoiseOscLvl) {
    changer = [&](Voice& voices) {
      VoiceManager::setSingleOSCVolumeLvl(voices, paramIdx, GetParam(paramIdx)->Value());
    };
  }
  if (paramIdx == kOscNoiseOnButton){
    changer = [&](Voice& voices) {
      (GetParam(kOscNoiseOnButton)->Bool())
        ? VoiceManager::setOscillatorMode(voices, 4, static_cast<Oscillator::OscillatorMode>(5))
        : VoiceManager::setOscillatorMode(voices, 4, static_cast<Oscillator::OscillatorMode>(0));
    };
  }
}

void BassGen::OnChangeEnvelopeBlock(int& paramIdx)
{
    // Volume Envelope:------------------------------------------------------------------|
  if (paramIdx == kAmpAttack) { //case kAmpHold:
    changer = [&](Voice& voices) {
      VoiceManager::setVolumeEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, GetParam(1)->Value());
    };
  }

  if (paramIdx == kAmpDecay) {
    changer = [&](Voice& voices) {
      VoiceManager::setVolumeEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_DECAY, GetParam(paramIdx)->Value());

    };
  }

  if (paramIdx == kAmpSustain) {
    changer = [&](Voice& voices) {
      VoiceManager::setVolumeEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, GetParam(paramIdx)->Value());
    };
  }

  if (paramIdx == kAmpRelease) {
    changer = [&](Voice& voices) {
      VoiceManager::setVolumeEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, GetParam(paramIdx)->Value());
    };
  }
}

void BassGen::OnChangeFilterBlock(int& paramIdx)
{
  switch (paramIdx)
  {
    // Filter Section:-------------------------------------------------------------------|
      //case kFilterMode:
        //changer = [&](Voice &voices) { VoiceManager::setFilterMode(voices, static_cast<Filter::FilterMode>(param->Int())); };
        //break;
  case kFilterCutoff:
    changer = [&](Voice& voices) { VoiceManager::setFilterCutoff(voices, GetParam(paramIdx)->Value()); };
    break;
  case kFilterResonance:
    changer = [&](Voice& voices) { VoiceManager::setFilterResonance(voices, GetParam(paramIdx)->Value()); };
    break;
    // Filter Envelope:
  case kFilterAttack:
    changer = [&](Voice& voices) {
      VoiceManager::setFilterEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, GetParam(paramIdx)->Value());
    };
    break;
  case kFilterDecay:
    changer = [&](Voice& voices) {
      VoiceManager::setFilterEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_DECAY, GetParam(paramIdx)->Value());

    };
    break;
  case kFilterSustain:
    changer = [&](Voice& voices) {
      VoiceManager::setFilterEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, GetParam(paramIdx)->Value());
    };
    break;
  case kFilterRelease:
    changer = [&](Voice& voices) {
      VoiceManager::setFilterEnvelopeStageValue(
        voices, EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, GetParam(paramIdx)->Value());
    };
    break;
  case kFilterEnvelopeAmount:
    changer = [&](Voice& voices) { VoiceManager::setFilterEnvAmount(voices, GetParam(paramIdx)->Value()); };
    break;
    // case kFilterLfoAmount:
      //changer = [&](Voice &voices) { VoiceManager::setFilterLFOAmount(voices, param->Value()); };
      //break;
     //case kSwitchButtonFilter:
  }
}

void BassGen::OnChangeLfoBlock(int& paramIdx)
{
  if (paramIdx == kLfoAmount) {}
  if (paramIdx == kLfoSpeedByFreq) {}
  if (paramIdx == kLfoSpeedByNote) {}
  if (paramIdx == kLfoResetGraph)
  {
  }
}

void BassGen::OnChangeTabsBlock(int& paramIdx)
{
  if (paramIdx == kTabsButton)
  {
    changer = [&](Voice& voices) {
      int state = GetParam(kTabsButton)->Int();
      if (state == 1)
      {
        GetUI()->GetControlWithTag(kEnvelopeGraph)->Hide(true);
        GetUI()->GetControlWithTag(kAmpAttack)->Hide(true);
        GetUI()->GetControlWithTag(kAmpDecay)->Hide(true);
        GetUI()->GetControlWithTag(kAmpSustain)->Hide(true);
        GetUI()->GetControlWithTag(kAmpRelease)->Hide(true);

        GetUI()->GetControlWithTag(kLfoWaveform)->Hide(false);
        GetUI()->GetControlWithTag(kLfoSpeedByFreq)->Hide(false);
        GetUI()->GetControlWithTag(kLfoAmount)->Hide(false);
        GetUI()->GetControlWithTag(kLfoResetGraph)->Hide(false);
        //GetUI()->GetControlWithTag(kLfoSpeedByNote)->Hide(false);
      }

      if (state == 2){
        GetUI()->GetControlWithTag(kLfoWaveform)->Hide(true);
        GetUI()->GetControlWithTag(kLfoSpeedByFreq)->Hide(true);
        GetUI()->GetControlWithTag(kLfoAmount)->Hide(true);
        GetUI()->GetControlWithTag(kLfoResetGraph)->Hide(true);
        //GetUI()->GetControlWithTag(kLfoSpeedByNote)->Hide(true);
      }

      if (state >= 4)
      {
        GetUI()->GetControlWithTag(kEnvelopeGraph)->Hide(false);
        GetUI()->GetControlWithTag(kAmpAttack)->Hide(false);
        GetUI()->GetControlWithTag(kAmpDecay)->Hide(false);
        GetUI()->GetControlWithTag(kAmpSustain)->Hide(false);
        GetUI()->GetControlWithTag(kAmpRelease)->Hide(false);
      }
    };
  }
}

void BassGen::OnChangePresetsBlock(int& paramIdx)
{
}
