#pragma once

#include <JuceHeader.h>
#include "RGBADecibelSlider.h"
#include "WaveGen.h"
#include "RGBAWaveDisplay.h"


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

    //Noise Generation Variables
    double currentSampleRate;

    double currentAngle;
    float angleDelta;

    double level;
    double targetLevel;

    double frequency;

    double swtLevel;
    double sawLevel;
    double sqrLevel;
    double maxWaveHeight;


    //GUI Variables
    juce::Slider red;
    juce::Slider green;
    juce::Slider blue;
    RGBADecibelSlider alpha;

    juce::Colour backgroundColor;

    //Midi Keyboard
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    int notesOn;

    //WaveDisplay
    RGBAWaveDisplay waveDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RGBASynth)
};
