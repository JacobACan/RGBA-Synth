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

    voicesOn == 1 ? firstVoice->startNote(midiNoteNumber, velocity, getSound(0).get(), 0)
        : voicesOn == 2 ? secondVoice->startNote(midiNoteNumber, velocity, getSound(0).get(), 0)
        : voicesOn;
}

void RGBASynthesizer::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    // TODO : use more sophistocated logic to de-activate Voices
    voicesOn -= 1;
    auto firstVoice = getVoice(0);
    auto secondVoice = getVoice(1);

    
    voicesOn == 0 ? firstVoice->stopNote(velocity, allowTailOff)
        : voicesOn == 1 ? secondVoice->stopNote(velocity, allowTailOff)
        : voicesOn;
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.
    auto firstVoice = getVoice(0);
    auto secoundVoice = getVoice(1);

    if (voicesOn == 0)
    {
        outPutBuffer.clear();
    }
    if (voicesOn == 1)
    {
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
    else if (voicesOn == 2)
    {
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
        secoundVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
}

void RGBASynthesizer::renderVoices(juce::AudioBuffer<float>& outPutBuffer, int startSample, int numSamples)
{
    //TODO : start multiple notes here.
    //TODO : condense and track the last note being pressed.
    auto firstVoice = getVoice(0);
    auto secoundVoice = getVoice(1);

    if (voicesOn == 0) 
    {
        outPutBuffer.clear();
    }
    if (voicesOn == 1)
    {
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
    }
    else if (voicesOn == 2)
    {
        firstVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
        secoundVoice->renderNextBlock(outPutBuffer, startSample, numSamples);
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