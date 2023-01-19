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
    // I dont think we should be starting all the voices here... only the first one with like getRelevantVoice()
    // And then get the synths current sounds and play the synth voice with the sounds it has... 
    // but how do we specify the sound that a certain voice should play?
    //set midiChannel notenumber and velocity of all synth sounds?

    for (auto voice = 0; voice < getNumVoices(); voice++)
    {
        auto currentVoice = getVoice(voice);
        currentVoice->setKeyDown(true);
        currentVoice->startNote(midiNoteNumber, velocity,); // We must pass in a synth sound, but it is defined as a voice?

    }
}