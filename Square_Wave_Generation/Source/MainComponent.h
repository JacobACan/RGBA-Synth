#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
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

    void buttonClicked(juce::Button*) override;
    void sliderValueChanged(juce::Slider*) override;
    

private:
    //==============================================================================

    //Noise Generation Variables
    float angleDelta;
    float currentAngle;
    float frequency;
    bool mute;
    bool combine;
    float combineAmt;
    float sqrWavPitch;


    //GUI Variables
    juce::Slider red;
    juce::Slider green;
    juce::Slider blue;
    juce::Slider alpha;

    juce::Colour backgroundColor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
