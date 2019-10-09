#include "includes\MIDIReceiver.h"

void MIDIReceiver::onMessageReceived(const IMidiMsg& midiMessage) {
	IMidiMsg::EStatusMsg status = midiMessage.StatusMsg();
	// We're only interested in Note On/Off messages (not CC, pitch, etc.)
	if (status == IMidiMsg::kNoteOn || status == IMidiMsg::kNoteOff)
  {
		mMidiQueue.Add(midiMessage);
	}
}

void MIDIReceiver::advance()
{
	while (!mMidiQueue.Empty())
  {
    const IMidiMsg& midiMessage = mMidiQueue.Peek();
		if (midiMessage.mOffset > mOffset) break;

		IMidiMsg::EStatusMsg status = midiMessage.StatusMsg();
		int noteNumber = midiMessage.NoteNumber();
		int velocity = midiMessage.Velocity();

    // There are only note on/off messages in the queue, see ::OnMessageReceived
		if (status == IMidiMsg::kNoteOn && velocity)
    {
			if (mKeyStatus[noteNumber] == false)
      {
				mKeyStatus[noteNumber] = true;
				mNumKeys += 1;
			  noteOn(noteNumber, velocity);
			}
		}
		else {
				mKeyStatus[noteNumber] = false;
				mNumKeys -= 1;
				noteOff(noteNumber, velocity);
		}
		mMidiQueue.Remove();
	}
	mOffset++;
}

MIDIReceiver::~MIDIReceiver()
{
}
