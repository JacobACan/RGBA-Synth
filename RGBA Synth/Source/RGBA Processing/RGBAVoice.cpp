/*
  ==============================================================================

    RGBASynthVoice.cpp
    Created: 19 Jan 2023 12:47:37pm
    Author:  13308

  ==============================================================================
*/

#include "RGBAVoice.h"
#include "WaveGen.h"

RGBAVoice::RGBAVoice()
    : angle(0),
    angleDelta(0),
    rootFrequency(440),
    currentMidiNote(-1),
    attackLevel(0),
    realeaseLevel(0),
    level(0),
    targetLevel(0),
    detuneAmount(0),
    sawLevel(0),
    sqrLevel(0),
    swtLevel(0)
{
    updateAngleDelta();
}

bool RGBAVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<RGBASound*> (sound) != nullptr;
}

void RGBAVoice::startNote(int midiNoteNumber,
    float velocity,
    juce::SynthesiserSound* /*sound*/,
    int /*currentPitchWheelPosition*/)
{
    currentMidiNote = midiNoteNumber;
    attackLevel = 0;
    realeaseLevel = 0;

    updateAngleDelta();


    setKeyDown(true);
}

void RGBAVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        if (realeaseLevel == 0.0)
            realeaseLevel = 1.0;
    }
    else
    {
        angleDelta = 0.0;  // Stops note from playing
        clearCurrentNote();
        setKeyDown(false);
    }

}

bool RGBAVoice::isVoiceActive() const
{
    return realeaseLevel > 0.0 || isKeyDown();
}

void RGBAVoice::pitchWheelMoved(int newPitchWheelValue)
{
    // TODO : alter voice when pitch wheel moved
}

void RGBAVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    // TODO : altar voice when controller Moved
}

void RGBAVoice::aftertouchChanged(int newAftertouchValue)
{
    // TODO : alter voice after touch changed
}

void RGBAVoice::channelPressureChanged(int newChannelPressureValue)
{
    // TODO : alter voice when channel pressure changed
}

bool RGBAVoice::isPlayingChannel(int midiChannel) const
{
    return midiChannel <2 && midiChannel >= 0;
}

void RGBAVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    double attackRamp = (1 - attackLevel) / outputBuffer.getNumSamples();
    int samplesAfterRelease = 5000;
    double releaseRamp = (double)attackLevel / (double)samplesAfterRelease;

    level = targetLevel;


    auto leftChannel = outputBuffer.getWritePointer(0);
    auto rightChannel = outputBuffer.getWritePointer(1);
    if (angleDelta != 0)
    {
        if (realeaseLevel > 0.0)
        {
            for (int sample = startSample; sample < numSamples; sample++)
            {
                double sinWavNoteSample = getNoteSample();

                leftChannel[sample] += sinWavNoteSample * level * attackLevel * realeaseLevel *.125;
                rightChannel[sample] += sinWavNoteSample * level * attackLevel * realeaseLevel *.125;

                angle += angleDelta;
                realeaseLevel -= releaseRamp;

                if (realeaseLevel < .005)
                {
                    angleDelta = 0;
                    clearCurrentNote();
                    setKeyDown(false);
                    realeaseLevel = 0;

                    break;
                }
            }
        }
        else 
        {
            for (int sample = startSample; sample < numSamples; sample++)
            {
                double sinWavNoteSample = getNoteSample();

                leftChannel[sample] += sinWavNoteSample * level * attackLevel *.125;
                rightChannel[sample] += sinWavNoteSample * level * attackLevel *.125;

                angle += angleDelta;
                attackLevel += attackRamp;

            }
            attackLevel = 1;
        }
    }
}

void RGBAVoice::renderNextBlock(juce::AudioBuffer<double>& outputBuffer, int startSample, int numSamples)
{
    double attackRamp = (1 - attackLevel) / outputBuffer.getNumSamples();
    int samplesAfterRelease = 5000;
    double releaseRamp = (double)attackLevel / (double)samplesAfterRelease;

    level = targetLevel;


    auto leftChannel = outputBuffer.getWritePointer(0);
    auto rightChannel = outputBuffer.getWritePointer(1);
    if (angleDelta != 0)
    {
        if (realeaseLevel > 0.0)
        {
            for (int sample = startSample; sample < numSamples; sample++)
            {
                double sinWavNoteSample = getNoteSample();

                leftChannel[sample] += sinWavNoteSample * level * attackLevel * realeaseLevel *.125;
                rightChannel[sample] += sinWavNoteSample * level * attackLevel * realeaseLevel *.125;

                angle += angleDelta;
                realeaseLevel -= releaseRamp;

                if (realeaseLevel < .005)
                {
                    angleDelta = 0;
                    clearCurrentNote();
                    setKeyDown(false);
                    realeaseLevel = 0;

                    break;
                }
            }
        }
        else
        {
            for (int sample = startSample; sample < numSamples; sample++)
            {
                double sinWavNoteSample = getNoteSample();

                leftChannel[sample] += sinWavNoteSample * level * attackLevel *.125;
                rightChannel[sample] += sinWavNoteSample * level * attackLevel *.125;

                angle += angleDelta;
                attackLevel += attackRamp;

            }
            attackLevel = 1;
        }
    }
}

double RGBAVoice::getNoteSample()
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


void RGBAVoice::updateAngleDelta()
{

    float cyclesPerSample = rootFrequency / getSampleRate(); // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

void RGBAVoice::setCurrentPlaybackSampleRate(double newRate)
{
    juce::SynthesiserVoice::setCurrentPlaybackSampleRate(newRate);
    updateAngleDelta();
}

void RGBAVoice::setStateInformation(juce::AudioProcessorValueTreeState& apvts)
{
    targetLevel.store(apvts.getRawParameterValue("targetLevel")->load());
    swtLevel.store(apvts.getRawParameterValue("swtLevel")->load());
    sawLevel.store(apvts.getRawParameterValue("sawLevel")->load());
    sqrLevel.store(apvts.getRawParameterValue("sqrLevel")->load());
    detuneAmount.store(apvts.getRawParameterValue("detuneAmount")->load());
}
