/*
  ==============================================================================

    RGBASynthSounds.cpp
    Created: 16 Jan 2023 1:36:11pm
    Author:  Jacob

  ==============================================================================
*/

#include "RGBASynthSounds.h"

RGBASin::RGBASin()
    : angle(0),
    angleDelta (0),
    rootFrequency(440)
{
    updateAngleDelta();
}

void RGBASin::renderNextBlock(juce::AudioBuffer<double> &outputBuffer, int sartSample, int numSamples)
{
    auto leftChannel = outputBuffer.getWritePointer(0);
    auto rightChannel = outputBuffer.getWritePointer(1);

    if (isKeyDown())
    {
        for (int sample = 0; sample < numSamples; sample++)
        {
            double sinWavNoteSample = std::sin(angle);

            leftChannel[sample] = sinWavNoteSample;
            rightChannel[sample] = sinWavNoteSample;

            angle += angleDelta;
            
        }
    }
}

double RGBASin::getNoteSample()
{
    int currentNoteNumber = getCurrentlyPlayingNote();
    if (currentNoteNumber == -1) return 0;

    int distanceFromA = currentNoteNumber - 48;
    double angleForNote = angle * std::pow<double>(2, (double)distanceFromA / (double)12);

    double bassSinVoiceSample = std::sin(angleForNote / 2);
    double sinWavSample = std::sin(angleForNote);

    double noteSampleVal = (sinWavSample + bassSinVoiceSample); 

    return noteSampleVal;
}


void RGBASin::updateAngleDelta()
{
    float cyclesPerSample = rootFrequency / getSampleRate(); // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

void RGBASin::setCurrentPlaybackSampleRate(double newRate)
{
    setCurrentPlaybackSampleRate(newRate);
    updateAngleDelta();
}