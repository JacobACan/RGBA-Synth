/*
  ==============================================================================
    RGBASynthesizer.cpp
    Created: 16 Jan 2023 12:38:55pm
    Author:  Jacob
  ==============================================================================
*/

#include "RGBASynthesizer.h"
#include "RGBASynthSounds.h"
#include "RGBASynthVoices.h"

RGBASynthesizer::RGBASynthesizer() : voicesOn(0)
{

}

void RGBASynthesizer::noteOn(int midiChannel, int midiNoteNumber, float velocity)
{
    // TODO : use more sophistocated logic to activate Voices
    voicesOn += 1;
    for (juce::SynthesiserVoice* voice : voices)
    {
        voice->startNote(midiNoteNumber, velocity, getSound(0).get(), 0);
    }
}

void RGBASynthesizer::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    // TODO : use more sophistocated logic to de-activate Voices
    voicesOn -= 1;
    for (auto voice : voices)
    {
        voice->stopNote(velocity, allowTailOff);
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.
    voicesOn -= 1;
    if (getNumVoices() > 0)
    {
        auto firstVoice = getVoice(0);
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<float>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.
    if (getNumVoices() > 0)
    {
        for (auto voice : voices)
        {
            voice->renderNextBlock(outPutBuffer, startSample, numSamples);
        }
    }
}

// =============================================================================================================

void RGBASynthesizer::updateVoiceParameters(juce::AudioProcessorValueTreeState& apvts)
{
    // TODO : more sophisticated method of updating multiple voice parameters.
    if (getNumVoices() > 0)
    {
        for (int i = 0; i < getNumVoices(); i++)
        {
            auto currentVoice = getVoice(i);
            if (RGBASin* sinVoice = dynamic_cast<RGBASin*>(currentVoice))
            {
                sinVoice->setStateInformation(apvts);
            }
        }
    }
}