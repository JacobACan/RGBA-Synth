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
        currentVoice->startNote(midiNoteNumber, velocity,  getSound(0).get(), 0);
    }


}

void RGBASynthesizer::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    // TODO : use more sophistocated logic to de-activate Voices
    voicesOn -= 1;
    if (getNumVoices() > 0)
    {
        auto voice = getVoice(0);
        voice->stopNote(velocity, allowTailOff);
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.

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
        auto firstVoice = getVoice(0);
        auto secondVoice = getVoice(1);
        auto thirdVoice = getVoice(2);
        auto fourthVoice = getVoice(3);

        voicesOn == 0 ? outPutBuffer.clear()
            : voicesOn == 1 ? firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples)
            : voicesOn == 2 ? secondVoice->renderNextBlock(outPutBuffer, startSample, numSamples)
            : voicesOn == 3 ? thirdVoice->renderNextBlock(outPutBuffer, startSample, numSamples)
            : voicesOn == 4 ? fourthVoice->renderNextBlock(outPutBuffer, startSample, numSamples)
            : voicesOn;
    }
}

// =============================================================================================================

void RGBASynthesizer::updateVoiceParameters(juce::AudioProcessorValueTreeState &apvts)
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