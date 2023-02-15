
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "RGBADecibelSlider.h"
#include "RGBAWaveDisplay.h"




//==============================================================================
/**
*/
class PluginRGBASynthProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Timer
{
public:
    PluginRGBASynthProcessorEditor (PluginRGBASynthProcessor&);
    ~PluginRGBASynthProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    //==============================================================================
    virtual void timerCallback();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginRGBASynthProcessor& audioProcessor;

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

    juce::MidiKeyboardComponent keyboardComponent;

    RGBAWaveDisplay waveDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginRGBASynthProcessorEditor)
};
