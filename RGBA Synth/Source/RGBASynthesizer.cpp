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
    for (int voice = 0; voice < getNumVoices(); voice++)
    {
        auto currentVoice = getVoice(voice);
        if (!currentVoice->isKeyDown())
        {
            currentVoice->startNote(midiNoteNumber, velocity,  getSound(0).get(), 0);
            break;
        }
    }

}

void RGBASynthesizer::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    // TODO : use more sophistocated logic to de-activate Voices
    if (getNumVoices() > 0)
    {
        auto currentVoice = getVoice(voice);
        auto nextVoice = getVoice(voice + 1);
        if (!nextVoice->isKeyDown())
        {
            currentVoice->stopNote(velocity, true);
            break;
        }
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples)
{
    for (int voice = 0; voice < getNumVoices(); voice++)
    {
        auto currentVoice = getVoice(voice);
        currentVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<float>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.

    if (getNumVoices() > 0)
    {
        auto firstVoice = getVoice(0);
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
}

// =============================================================================================================

void RGBASynthesizer::updateVoiceParameters(juce::AudioProcessorValueTreeState &apvts)
{
    for (int voice = 0; voice < getNumVoices(); voice++)
    {
        auto firstVoice = getVoice(0);
        if (RGBASin* sinVoice = dynamic_cast<RGBASin*>(firstVoice))
        {
            sinVoice->setStateInformation(apvts);
        }
    }
}