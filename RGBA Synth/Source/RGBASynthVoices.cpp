/*
  ==============================================================================

    RGBASynthVoice.cpp
    Created: 19 Jan 2023 12:47:37pm
    Author:  13308

  ==============================================================================
*/

#include "RGBASynthVoices.h"
#include "WaveGen.h"

RGBASin::RGBASin()
    : angle(0),
    angleDelta(0),
    rootFrequency(440),
    currentMidiNote(-1),
    attackLevel(0),
    level(.125),
    targetLevel(.125),
    detuneAmount(0),
    sawLevel(1),
    sqrLevel(1),
    swtLevel(1)
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
    //TODO : use getCurrentlyPlayingNote Instead of instantaneously setting a note (causing artifacts)
    currentMidiNote = midiNoteNumber;
    attackLevel = 0;
    setKeyDown(true);
}

void RGBASin::stopNote(float velocity, bool allowTailOff)
{
    // TODO : do something when note stops.
    setKeyDown(false);

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

void RGBASin::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    double attackRamp = (1 - attackLevel) / outputBuffer.getNumSamples();
    int samplesAfterRelease = 0;
    double releaseRamp = (double)attackLevel / (double)samplesAfterRelease;

    level = targetLevel;


    auto leftChannel = outputBuffer.getWritePointer(0);
    auto rightChannel = outputBuffer.getWritePointer(1);
    DBG("Level : " << level);
    if (isKeyDown())
    {
        for (int sample = startSample; sample < numSamples; sample++)
        {
            double sinWavNoteSample = getNoteSample();

            leftChannel[sample] += sinWavNoteSample * level * attackLevel;
            rightChannel[sample] += sinWavNoteSample * level * attackLevel;

            angle += angleDelta;
            attackLevel += attackRamp;

        }
        attackLevel = 1;
    }
}

void RGBASin::renderNextBlock(juce::AudioBuffer<double>& outputBuffer, int startSample, int numSamples)
{
    double attackRamp = (1 - attackLevel) / outputBuffer.getNumSamples();
    int samplesAfterRelease = 0;
    double releaseRamp = (double)attackLevel / (double)samplesAfterRelease;

    level = targetLevel;


    auto leftChannel = outputBuffer.getWritePointer(0);
    auto rightChannel = outputBuffer.getWritePointer(1);

    if (isKeyDown())
    {
        for (int sample = startSample; sample < numSamples; sample++)
        {
            double sinWavNoteSample = getNoteSample();

            leftChannel[sample] = sinWavNoteSample * level * attackLevel;
            rightChannel[sample] = sinWavNoteSample * level * attackLevel;

            angle += angleDelta;
            attackLevel += attackRamp;

        }
        attackLevel = 1;
    }
}

double RGBASin::getNoteSample()
{
    int currentNoteNumber = currentMidiNote;
    if (currentNoteNumber == -1) return 0;

    int noteOffset = 21;
    currentNoteNumber -= noteOffset;

    int distanceFromA = currentNoteNumber - 48;
    double angleForNote = angle * std::pow<double>(2, (double)distanceFromA / (double)12);

    double bassSinVoiceSample = std::sin(angleForNote / 2);


    double sinWavSample = std::sin(angleForNote);

    double swtWavSample = WaveGen::swt(angleForNote);
    double swtVoiceSample2 = WaveGen::swt(angleForNote / (1 + detuneAmount));
    double swtVoiceSample3 = WaveGen::swt(angleForNote * (1 + detuneAmount));

    double sawWavSample = WaveGen::saw(angleForNote);
    double sawVoiceSample2 = WaveGen::saw(angleForNote / (1 + detuneAmount));
    double sawVoiceSample3 = WaveGen::saw(angleForNote * (1 + detuneAmount));

    double sqrWavSample = WaveGen::sqr(angleForNote);
    double sqrVoiceSample2 = WaveGen::sqr(angleForNote / (1 + detuneAmount));
    double sqrVoiceSample3 = WaveGen::sqr(angleForNote * (1 + detuneAmount));

    double noteSampleVal;

    noteSampleVal = (
        sinWavSample
        + ((swtWavSample + swtVoiceSample2 + swtVoiceSample3) * swtLevel)
        + ((sawWavSample + sawVoiceSample2 + sawVoiceSample3) * sawLevel)
        + ((sqrWavSample + sqrVoiceSample2 + sqrVoiceSample3) * sqrLevel)

        + bassSinVoiceSample
        )
        / ((swtLevel * 3) + (sawLevel * 3) + (sqrLevel * 3) + 1); // Max combined in phase level reached by adding these wave samples.

    return noteSampleVal;
}


void RGBASin::updateAngleDelta()
{

    float cyclesPerSample = rootFrequency / getSampleRate(); // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

void RGBASin::setCurrentPlaybackSampleRate(double newRate)
{
    juce::SynthesiserVoice::setCurrentPlaybackSampleRate(newRate);
    updateAngleDelta();
}

void RGBASin::setStateInformation(juce::AudioProcessorValueTreeState& apvts)
{
    targetLevel.store(apvts.getRawParameterValue("targetLevel")->load());
    swtLevel.store(apvts.getRawParameterValue("swtLevel")->load());
    sawLevel.store(apvts.getRawParameterValue("sawLevel")->load());
    sqrLevel.store(apvts.getRawParameterValue("sqrLevel")->load());
    detuneAmount.store(apvts.getRawParameterValue("detuneAmount")->load());
}