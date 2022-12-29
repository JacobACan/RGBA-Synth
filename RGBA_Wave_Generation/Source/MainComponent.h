#pragma once

#include <JuceHeader.h>
#include "DecibelsSlider.h" 

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
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

    

private:
    //==============================================================================

    //Functions
    void updateAngleDelta();

    //Noise Generation Variables
    float currentSampleRate;

    float currentAngle;
    float angleDelta;

    float level;
    float targetLevel;

    float frequency;
    float targetFrequency;


    //GUI Variables
    juce::Slider red;
    juce::Slider green;
    juce::Slider blue;
    DecibelSlider alpha;

    juce::Colour backgroundColor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
