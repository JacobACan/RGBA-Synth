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
#include "RGBA GUI/Sliders/RGBAVerticalSlider.h"
#include "RGBA GUI/Sliders/RGBARotarySlider.h"
#include "RGBA GUI/RGBAColours.h"
#include "RGBA GUI/RGBAColourGradient.h"
#include "RGBA GUI/Fonts.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//==============================================================================
/**
*/
class RGBASynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	RGBASynthAudioProcessorEditor(RGBASynthAudioProcessor&, juce::AudioProcessorValueTreeState& apvts);
	~RGBASynthAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:

	juce::AudioProcessorValueTreeState& editorApvts;

	juce::MidiKeyboardComponent keyboardComponent;

	RGBAWaveDisplay waveDisplay;

	juce::Colour backgroundColor;

	juce::Label targetLevelLabel;
	RGBAVerticalSlider targetLevelSlider;
	std::unique_ptr<SliderAttachment> targetLevelAttatchment;

	juce::Label swtLevelLabel;
	RGBAVerticalSlider swtLevelSlider;
	std::unique_ptr<SliderAttachment> swtLevelAttatchment;
	RGBARotarySlider swtPhaseSlider;
	std::unique_ptr<SliderAttachment> swtPhaseAttatchment;

	juce::Label sawLevelLabel;
	RGBAVerticalSlider sawLevelSlider;
	std::unique_ptr<SliderAttachment> sawLevelAttatchment;
	RGBARotarySlider sawPhaseSlider;
	std::unique_ptr<SliderAttachment> sawPhaseAttatchment;

	juce::Label sqrLevelLabel;
	RGBAVerticalSlider sqrLevelSlider;
	std::unique_ptr<SliderAttachment> sqrLevelAttatchment;
	RGBARotarySlider sqrPhaseSlider;
	std::unique_ptr<SliderAttachment> sqrPhaseAttatchment;

	RGBAVerticalSlider detuneAmountSlider;
	std::unique_ptr<SliderAttachment> detuneAmountAttatchment;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RGBASynthAudioProcessorEditor)
};

