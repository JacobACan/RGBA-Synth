/*
  ==============================================================================

    RGBASynthVoice.cpp
    Created: 19 Jan 2023 12:47:37pm
    Author:  13308

  ==============================================================================
*/

#include "RGBASynthVoice.h"

RGBASynthVoice::RGBASynthVoice()
{

}

bool RGBASynthVoice::canPlaySound(juce::SynthesiserSound*) { return true; }

void RGBASynthVoice::startNote(int midiNoteNumber,
    float velocity,
    juce::SynthesiserSound * sound,
    int currentPutchWheelPosition)
{
    //Well dang... what do I do here? if I render the next block, there is nothing to render it on?

    // SO basically here Ill set the pitch and velocity or whatever of the sound.
    
}
