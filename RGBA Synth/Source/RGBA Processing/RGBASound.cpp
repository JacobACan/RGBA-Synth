/*
  ==============================================================================

    RGBASynthSounds.cpp
    Created: 16 Jan 2023 1:36:11pm
    Author:  Jacob

  ==============================================================================
*/

#include "RGBASound.h"

RGBASound::RGBASound(){}

bool RGBASound::appliesToNote(int /*midiNoteNumber*/)
{
    return true;
}

bool RGBASound::appliesToChannel(int /*midiChannel*/)
{
    return true;
}