/*
  ==============================================================================

    RGBASynthSounds.cpp
    Created: 16 Jan 2023 1:36:11pm
    Author:  Jacob

  ==============================================================================
*/

#include "RGBASynthSounds.h"

void RGBASin::renderNextBlock(juce::AudioBuffer<double> &outputBuffer, int sartSample, int numSamples)
{
    auto leftChannel = outputBuffer.getWritePointer(0);
    auto rightChannel = outputBuffer.getWritePointer(1);

    if (isKeyDown())
    {
        for (int sample = 0; sample < numSamples; sample++)
        {
            double sinWavSample = std::sin(angle);

            leftChannel[sample] = sinWavSample;
            rightChannel[sample] = sinWavSample;

            angle += angleDelta;
        }
    }
}

void RGBASin::setCurrentPlaybackSampleRate(double newRate)
{
    setCurrentPlaybackSampleRate(newRate);
    updateAngleDelta();
}