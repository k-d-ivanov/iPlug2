#pragma once //__BASSGEN__MIDIReceiver__

#include "IPlugMidi.h"
#include "IPlugQueue.h"

#include "GallantSignal.h"
using Gallant::Signal2;

using namespace iplug;

class MIDIReceiver {
public:
  MIDIReceiver() :
    mNumKeys(0),
    mOffset{ 0 }  
  {};

	// Returns true if the key with a given index is currently pressed
	inline bool getKeyStatus(int keyIndex) const { return mKeyStatus[keyIndex]; }
	// Returns the number of keys currently pressed
	inline int getNumKeys() const { return mNumKeys; }
	void advance();
	void onMessageReceived(const IMidiMsg& midiMessage);
	inline void Flush(int nFrames) { mMidiQueue.Flush(nFrames); mOffset = 0; }
	inline void Resize(int blockSize) { mMidiQueue.Resize(blockSize); }

	Signal2< int, int > noteOn;
	Signal2< int, int > noteOff;
	~MIDIReceiver();
private:
	IMidiQueue mMidiQueue;
	static const int keyCount = 128;
	int mNumKeys; // how many keys are being played at the moment (via midi)
	bool mKeyStatus[keyCount] = { false }; // array of on/off for each key (index is note number)
	int mOffset;
};