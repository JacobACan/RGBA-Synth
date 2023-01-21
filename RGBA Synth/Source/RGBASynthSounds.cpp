/*
  ==============================================================================

    RGBASynthSounds.cpp
    Created: 16 Jan 2023 1:36:11pm
    Author:  Jacob

  ==============================================================================
*/

#include "RGBASynthSounds.h"

RGBASound1::RGBASound1(){}

bool RGBASound1::appliesToNote(int midiNoteNumber)
{
    return true;
}

bool RGBASound1::appliesToChannel(int midiChannel)
{
    return true;
}