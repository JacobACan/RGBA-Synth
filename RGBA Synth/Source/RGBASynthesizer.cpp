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
    // Take all voices / notes and set their preconditions
    for (int voice = 0; voice < getNumVoices(); voice++)
    {
        auto currentVoice = getVoice(voice);
        currentVoice->setKeyDown(true);
    }

}

void RGBASynthesizer::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    //Stopping all sounds playing to the first voice...
    DBG("Note Off");
    if (getNumVoices() > 0)
    {
        auto voice = getVoice(0);
        voice->stopNote(velocity, allowTailOff);
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples)
{
    // TODO : need to set note number?
    // TODO : need to set note on?
    if (getNumVoices() > 0)
    {
        auto firstVoice = getVoice(0);
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }  
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<float>& outPutBuffer, int startSample, int numSamples)
{

    if (getNumVoices() > 0)
    {
        auto firstVoice = getVoice(0);
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
}