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
    auto firstVoice = getVoice(0);
    auto secondVoice = getVoice(1);
    firstVoice->startNote(midiNoteNumber, velocity, getSound(0).get(), 0);
    secondVoice->startNote(midiNoteNumber + 7, velocity, getSound(0).get(), 0);
}

void RGBASynthesizer::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    // TODO : use more sophistocated logic to de-activate Voices
    voicesOn -= 1;
    auto firstVoice = getVoice(0);
    auto secondVoice = getVoice(1);
    firstVoice->stopNote(velocity, allowTailOff);
    secondVoice->stopNote(velocity, allowTailOff);
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.
    auto firstVoice = getVoice(0);
    auto secoundVoice = getVoice(1);
    firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    secoundVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<float>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.
    auto firstVoice = getVoice(0);
    auto secoundVoice = getVoice(1);
    firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    secoundVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
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