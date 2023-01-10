#pragma once

#include <JuceHeader.h>
#include "RGBADecibelSlider.h"
#include "RGBAWaveDisplay.h"
#include "WaveGen.h"


class RGBASynth : public juce::AudioAppComponent,
                  public juce::MidiKeyboardState::Listener
{
public:
    //==============================================================================
    RGBASynth();
    ~RGBASynth() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    //===============================================================================
    void handleNoteOn(juce::MidiKeyboardState* source,
        int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source,
        int midiChannel, int midiNoteNumber, float velocity) override;

private:
    //==============================================================================

    //Functions
    void updateAngleDelta();
    double getNoteSample(int noteNumber, double angle);

    //Noise Generation Variables
    double currentSampleRate;

    double currentAngle;
    float angleDelta;

    double level;
    double targetLevel;

    double rootFrequency;
    int noteNumber1;
    int noteNumber2;
    int noteNumber3;
    int noteNumber4;
    double noteSample1;
    double noteSample2;
    double noteSample3;
    double noteSample4;

    double swtLevel;
    double sawLevel;
    double sqrLevel;

    double detuneAmount;
    int extraVoices;


    //GUI Variables
    juce::Slider red;
    juce::Slider green;
    juce::Slider blue;
    RGBADecibelSlider alpha;

    juce::Slider swtPhase;
    juce::Slider sawPhase;
    juce::Slider sqrPhase;
    juce::Slider detune;

    juce::Colour backgroundColor;

    //Midi Keyboard
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    int notesOn;

    //WaveDisplay
    RGBAWaveDisplay waveDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RGBASynth)
};
