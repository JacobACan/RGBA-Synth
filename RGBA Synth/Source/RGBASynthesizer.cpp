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
    // TODO : Figure out how to actually set currently playing note of voice
    // TODO : Check for number of voices available to prevent crashing
    voicesOn += 1;

    for (int i = 0; i < voicesOn; i++)
    {
        auto voice = getVoice(i);
        if (!voice->isKeyDown()) 
            voice->startNote(midiNoteNumber, velocity, getSound(0).get(), 0);
    }
}

void RGBASynthesizer::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    // TODO : Figure out how to actually set currently playing note
    // TODO : Handle notes that arent of sinVoice type
    voicesOn -= 1;

    for (auto voice : voices)
    {
        if (RGBASin* sinVoice = dynamic_cast<RGBASin*>(voice))
        {
            /*DBG("midi NN : " << midiNoteNumber);
            DBG("voice NN : " << sinVoice->currentMidiNote);*/
            if (sinVoice->currentMidiNote == midiNoteNumber)
            {
                sinVoice->stopNote(velocity, allowTailOff);
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