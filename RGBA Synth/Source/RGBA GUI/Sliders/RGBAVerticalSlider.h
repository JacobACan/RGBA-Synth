/*
  ==============================================================================

	RGBAVerticalSlider.h
	Created: 24 Apr 2023 12:17:24pm
	Author:  13308

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../RGBAColours.h"

class RGBAVerticalSlider : public juce::Slider {
public:
	RGBAVerticalSlider();
	RGBAVerticalSlider(juce::Colour thumbColor, juce::String sliderName = "");
	void paint(juce::Graphics& g) override;

private:
	juce::Colour thumbColor;
	juce::String sliderName;
};