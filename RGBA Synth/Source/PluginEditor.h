/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RGBA GUI/RGBAWaveDisplay.h"
#include "RGBA Processing/WaveGen.h"
#include "RGBA GUI/Sliders/AlphaSlider.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//==============================================================================
/**
*/
class RGBASynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RGBASynthAudioProcessorEditor (RGBASynthAudioProcessor&, juce::AudioProcessorValueTreeState& apvts);
    ~RGBASynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& editorApvts;

    juce::MidiKeyboardComponent keyboardComponent;

    RGBAWaveDisplay waveDisplay;

    juce::Colour backgroundColor;

    AlphaSlider targetLevelSlider;
    std::unique_ptr<SliderAttachment> targetLevelAttatchment;

    juce::Slider swtLevelSlider;
    std::unique_ptr<SliderAttachment> swtLevelAttatchment;
    juce::Slider swtPhaseSlider;
    std::unique_ptr<SliderAttachment> swtPhaseAttatchment;

    juce::Slider sawLevelSlider;
    std::unique_ptr<SliderAttachment> sawLevelAttatchment;
    juce::Slider sawPhaseSlider;
    std::unique_ptr<SliderAttachment> sawPhaseAttatchment;

    juce::Slider sqrLevelSlider;
    std::unique_ptr<SliderAttachment> sqrLevelAttatchment;
    juce::Slider sqrPhaseSlider;
    std::unique_ptr<SliderAttachment> sqrPhaseAttatchment;

    juce::Slider detuneAmountSlider;
    std::unique_ptr<SliderAttachment> detuneAmountAttatchment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RGBASynthAudioProcessorEditor)
};

