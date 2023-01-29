/*
  ==============================================================================

    RGBASynthSounds.h
    Created: 16 Jan 2023 1:36:11pm
    Author:  Jacob

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//TODO : Find out purpose of this class.

class RGBASound1 : public juce::SynthesiserSound
{
public:
    RGBASound1();

    //==============================================================================
    /** Returns true if this sound should be played when a given midi note is pressed.

        The Synthesiser will use this information when deciding which sounds to trigger
        for a given note.
    */
    virtual bool appliesToNote(int midiNoteNumber) override;

    /** Returns true if the sound should be triggered by midi events on a given channel.

        The Synthesiser will use this information when deciding which sounds to trigger
        for a given note.
    */
    virtual bool appliesToChannel(int midiChannel) override;

    /** The class is reference-counted, so this is a handy pointer class for it. */


private:

};