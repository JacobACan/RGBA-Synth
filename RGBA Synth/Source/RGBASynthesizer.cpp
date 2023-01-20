/*
  ==============================================================================

    RGBASynthesizer.cpp
    Created: 16 Jan 2023 12:38:55pm
    Author:  Jacob

  ==============================================================================
*/

#include "RGBASynthesizer.h"

RGBASynthesizer::RGBASynthesizer() 
{

}

void RGBASynthesizer::noteOn(int midiChannel, int midiNoteNumber, float velocity)
{
    // Getting every sound and playing it to the fist voice...
    // In this case, for testing purposes, there is one sound and one voice.

    if (getNumVoices() > 0)
    {
        auto voice = getVoice(0);
        for (int i = 0; i < getNumSounds(); i++)
        {
            auto sound = getSound(i).get();
            voice->startNote(midiNoteNumber, velocity,  sound, 0);
        }
    }
}