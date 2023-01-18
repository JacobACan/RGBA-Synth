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
    //~RGBASin() override;


    bool canPlaySound(juce::SynthesiserSound*) override;

    /** Called to start a new note.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void startNote(int midiNoteNumber,
        float velocity,
        juce::SynthesiserSound* sound,
        int currentPitchWheelPosition) override;

    /** Called to stop a note.

        This will be called during the rendering callback, so must be fast and thread-safe.

        The velocity indicates how quickly the note was released - 0 is slowly, 1 is quickly.

        If allowTailOff is false or the voice doesn't want to tail-off, then it must stop all
        sound immediately, and must call clearCurrentNote() to reset the state of this voice
        and allow the synth to reassign it another sound.

        If allowTailOff is true and the voice decides to do a tail-off, then it's allowed to
        begin fading out its sound, and it can stop playing until it's finished. As soon as it
        finishes playing (during the rendering callback), it must make sure that it calls
        clearCurrentNote().
    */
    void stopNote(float velocity, bool allowTailOff) override;

    /** Returns true if this voice is currently busy playing a sound.
        By default this just checks the getCurrentlyPlayingNote() value, but can
        be overridden for more advanced checking.
    */
    bool isVoiceActive() const override;

    /** Called to let the voice know that the pitch wheel has been moved.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void pitchWheelMoved(int newPitchWheelValue) override;

    /** Called to let the voice know that a midi controller has been moved.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void controllerMoved(int controllerNumber, int newControllerValue) override;

    /** Called to let the voice know that the aftertouch has changed.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void aftertouchChanged(int newAftertouchValue) override;

    /** Called to let the voice know that the channel pressure has changed.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void channelPressureChanged(int newChannelPressureValue) override;

    //==============================================================================
    /** Renders the next block of data for this voice.

        The output audio data must be added to the current contents of the buffer provided.
        Only the region of the buffer between startSample and (startSample + numSamples)
        should be altered by this method.

        If the voice is currently silent, it should just return without doing anything.

        If the sound that the voice is playing finishes during the course of this rendered
        block, it must call clearCurrentNote(), to tell the synthesiser that it has finished.

        The size of the blocks that are rendered can change each time it is called, and may
        involve rendering as little as 1 sample at a time. In between rendering callbacks,
        the voice's methods will be called to tell it about note and controller events.
    */
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
        int startSample,
        int numSamples) override;

    /** A double-precision version of renderNextBlock() */
    void renderNextBlock(juce::AudioBuffer<double>& outputBuffer,
        int startSample,
        int numSamples) override;

    /** Changes the voice's reference sample rate.

        The rate is set so that subclasses know the output rate and can set their pitch
        accordingly.

        This method is called by the synth, and subclasses can access the current rate with
        the currentSampleRate member.
    */
    void setCurrentPlaybackSampleRate(double newRate) override;

    /** Returns true if the voice is currently playing a sound which is mapped to the given
        midi channel.

        If it's not currently playing, this will return false.
    */
    bool isPlayingChannel(int midiChannel) const override;

private:

    //Helper Functions
    double getNoteSample();
    void updateAngleDelta();

    double angleDelta;
    double angle;
    float rootFrequency;
};