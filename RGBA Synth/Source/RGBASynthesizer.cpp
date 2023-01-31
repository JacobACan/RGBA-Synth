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

RGBASynthesizer::RGBASynthesizer() 
{

}

void RGBASynthesizer::noteOn(int midiChannel, int midiNoteNumber, float velocity)
{
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
    for (int voice = 0; voice < getNumVoices() - 1; voice++)
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
    DBG("Start Sample : " << startSample);
    auto currentVoice = getVoice(0);
    currentVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
}

// =============================================================================================================

void RGBASynthesizer::updateVoiceParameters(juce::AudioProcessorValueTreeState &apvts)
{
    for (int voice = 0; voice < getNumVoices(); voice++)
    {
        auto currentVoice = getVoice(voice);
        if (RGBASin* currentSinVoice = dynamic_cast<RGBASin*>(currentVoice))
        {
            currentSinVoice->setStateInformation(apvts);
        }
    }
}