#pragma once

#define PLUG_NAME "BassGen"
#define PLUG_MFR "KirilIvanov"
#define PLUG_VERSION_HEX 0x00010000 // Format  0xMAJR.MN.BG - in HEX! so version 10.1.5 would be 0x000A0105
#define PLUG_VERSION_STR "0.0.1"
#define PLUG_UNIQUE_ID 'Bsgn'
#define PLUG_MFR_ID 'Kdiv'
#define PLUG_URL_STR "https://iplug2.github.io"
#define PLUG_EMAIL_STR "spam@me.com"
#define PLUG_COPYRIGHT_STR "Copyright 2019 Acme Inc"
#define PLUG_CLASS_NAME BassGen

#define BUNDLE_NAME "BassGen"
#define BUNDLE_MFR "KirilIvanov"
#define BUNDLE_DOMAIN "com"

#define PLUG_CHANNEL_IO "0-2"
#define SHARED_RESOURCES_SUBPATH "BassGen"

#define PLUG_LATENCY 0
#define PLUG_TYPE 1
#define PLUG_DOES_MIDI_IN 1
#define PLUG_DOES_MIDI_OUT 1
#define PLUG_DOES_MPE 1
#define PLUG_DOES_STATE_CHUNKS 0
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 1020
#define PLUG_HEIGHT 600
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0

#define AUV2_ENTRY BassGen_Entry
#define AUV2_ENTRY_STR "BassGen_Entry"
#define AUV2_FACTORY BassGen_Factory
#define AUV2_VIEW_CLASS BassGen_View
#define AUV2_VIEW_CLASS_STR "BassGen_View"

#define AAX_TYPE_IDS 'EFN1', 'EFN2'
#define AAX_PLUG_MFR_STR "Acme"
#define AAX_PLUG_NAME_STR "BassGen\nIPIS"
#define AAX_DOES_AUDIOSUITE 0
#define AAX_PLUG_CATEGORY_STR "Synth"

#define VST3_SUBCATEGORY "Instrument|Synth"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_RESIZABLE 0
#define APP_SIGNAL_VECTOR_SIZE 64

#define ROBOTTO_FN "Roboto-Regular.ttf"

// Image resource locations for this plug.
#define KNOB_FN       "vst_small_knob.png"	
#define BG_FN         "vst_bg.png"
//#define ABOUTBOX_FN   "resources/img/about.png"
#define WHITE_KEY_FN  "vst_wkey.png"
#define BLACK_KEY_FN  "vst_bkey.png"
#define IFADERCONTROL_VERT_FN "vst_slider.png"
#define WAVEFORM_SW_FN "vst_waveform.png"
#define FILTER_FORM_FN "vst_filter_form.png"
#define ON_OFF_SWITCH_FN "vst_on_off.png"
#define OSC_LVL_KNOB_FN "vst_big_knob.png"
#define TABS_BUTTON_FN "vst_tabs.png"
