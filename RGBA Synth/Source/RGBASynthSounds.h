/*
  ==============================================================================

    RGBASynthSounds.h
    Created: 16 Jan 2023 1:36:11pm
    Author:  Jacob

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class RGBASin : public juce::SynthesiserVoice
{
public:
    RGBASin();

    void renderNextBlock(juce::AudioBuffer<double>& outputBuffer, int startSample, int numSamples) override;
    void setCurrentPlaybackSampleRate(double newRate) override;

private:

    //Helper Functions
    double getNoteSample();
    void updateAngleDelta();

    double angleDelta;
    double angle;
    float rootFrequency;
};