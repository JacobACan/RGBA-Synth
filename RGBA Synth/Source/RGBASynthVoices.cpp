/*
  ==============================================================================

    RGBASynthVoice.cpp
    Created: 19 Jan 2023 12:47:37pm
    Author:  13308

  ==============================================================================
*/

#include "RGBASynthVoices.h"

RGBASin::RGBASin()
    : angle(0),
    angleDelta(0),
    rootFrequency(440)
{
    updateAngleDelta();
}

bool RGBASin::canPlaySound(juce::SynthesiserSound*)
{
    // TODO : Determine if sound can be played by this voice
    return true;
}

void RGBASin::startNote(int midiNoteNumber,
    float velocity,
    juce::SynthesiserSound* sound,
    int currentPitchWheelPosition)
{
    // TODO : do something when starting note.
}

void RGBASin::stopNote(float velocity, bool allowTailOff)
{
    // TODO : do something when note stops.
}

bool RGBASin::isVoiceActive() const
{
    // TODO : determine if this voice is active
    return true;
}

void RGBASin::pitchWheelMoved(int newPitchWheelValue)
{
    // TODO : alter voice when pitch wheel moved
}

void RGBASin::controllerMoved(int controllerNumber, int newControllerValue)
{
    // TODO : altar voice when controller Moved
}

void RGBASin::aftertouchChanged(int newAftertouchValue)
{
    // TODO : alter voice after touch changed
}

void RGBASin::channelPressureChanged(int newChannelPressureValue)
{
    // TODO : alter voice when channel pressure changed
}

bool RGBASin::isPlayingChannel(int midiChannel) const
{
    //TODO determine if voice is playing a channel
    return true;
}

void RGBASin::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int sartSample, int numSamples)
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

void RGBASin::renderNextBlock(juce::AudioBuffer<double>& outputBuffer, int sartSample, int numSamples)
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
    DBG("Setting Sin Voices Playback SampleRate");
    juce::SynthesiserVoice::setCurrentPlaybackSampleRate(newRate);
    updateAngleDelta();
}

