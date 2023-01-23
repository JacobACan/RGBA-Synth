/*
  ==============================================================================

    RGBASynthVoice.h
    Created: 19 Jan 2023 12:47:37pm
    Author:  13308

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RGBASynthVoice : public juce::SynthesiserVoice
{
    RGBASynthVoice();

    /** Must return true if this voice object is capable of playing the given sound.

        If there are different classes of sound, and different classes of voice, a voice can
        choose which ones it wants to take on.

        A typical implementation of this method may just return true if there's only one type
        of voice and sound, or it might check the type of the sound object passed-in and
        see if it's one that it understands.
    */
    bool canPlaySound(juce::SynthesiserSound*);

    /** Called to start a new note.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void startNote(int midiNoteNumber,
        float velocity,
        juce::SynthesiserSound* sound,
        int currentPitchWheelPosition);

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
    void stopNote(float velocity, bool allowTailOff);

    /** Returns true if this voice is currently busy playing a sound.
        By default this just checks the getCurrentlyPlayingNote() value, but can
        be overridden for more advanced checking.
    */
    bool isVoiceActive() const;

    /** Called to let the voice know that the pitch wheel has been moved.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void pitchWheelMoved(int newPitchWheelValue);

    /** Called to let the voice know that a midi controller has been moved.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void controllerMoved(int controllerNumber, int newControllerValue);

};
