/*
  ==============================================================================
    RGBASynthesizer.cpp
    Created: 16 Jan 2023 12:38:55pm
    Author:  Jacob
  ==============================================================================
*/

#include "RGBASynthesizer.h"
#include "RGBASound.h"
#include "RGBAVoice.h"

RGBASynthesizer::RGBASynthesizer() : voicesOn(0)
{
}

void RGBASynthesizer::noteOn(int midiChannel, int midiNoteNumber, float velocity)
{
    auto freeVoice = findFreeVoice(getSound(0).get(), midiChannel, midiNoteNumber, true);
    freeVoice->startNote(midiNoteNumber, velocity, getSound(0).get(), 0);
}

void RGBASynthesizer::noteOff(int /*midiChannel*/, int midiNoteNumber, float velocity, bool allowTailOff)
{
    for (auto voice : voices)
    {
        if (RGBAVoice* rgbaVoice = dynamic_cast<RGBAVoice*>(voice))
        {
            if (rgbaVoice->currentMidiNote == midiNoteNumber)
            {
                rgbaVoice->stopNote(velocity, allowTailOff);
            }
        }
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples)
{
    if (voicesOn > 0)
    {
        for (auto voice : voices)
        {
            voice->renderNextBlock(outPutBuffer, startSample, numSamples);
        }
    }
    else
    {
        outPutBuffer.clear();
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<float>& outPutBuffer, int startSample, int numSamples)
{
    
    for (auto voice : voices)
    {
        voice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
    
}

// =============================================================================================================

void RGBASynthesizer::updateVoiceParameters(juce::AudioProcessorValueTreeState& apvts)
{
    if (getNumVoices() > 0)
    {
        for (int i = 0; i < getNumVoices(); i++)
        {
            auto currentVoice = getVoice(i);
            if (RGBAVoice* rgbaVoice = dynamic_cast<RGBAVoice*>(currentVoice))
            {
                rgbaVoice->setStateInformation(apvts);
            }
        }
    }
}