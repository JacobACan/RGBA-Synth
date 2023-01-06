#pragma once

#include <JuceHeader.h>
#include "RGBADecibelSlider.h"


class MainComponent  : public juce::AudioAppComponent,
                       public juce::MidiKeyboardState::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

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
    double angleDelta;

    double level;
    double targetLevel;

    double frequency;
    double targetFrequency;


    //GUI Variables
    juce::Slider red;
    juce::Slider green;
    juce::Slider blue;
    RGBADecibelSlider alpha;

    juce::Colour backgroundColor;

    //Midi Keyboard
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    bool noteOn;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
