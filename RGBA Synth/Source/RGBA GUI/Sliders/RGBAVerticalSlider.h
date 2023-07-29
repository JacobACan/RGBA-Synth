/*
  ==============================================================================

	RGBAVerticalSlider.h
	Created: 24 Apr 2023 12:17:24pm
	Author:  13308

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RGBAVerticalSlider : public juce::Slider {
public:
	RGBAVerticalSlider();
	RGBAVerticalSlider(juce::Colour thumbColor);
private:
	juce::Colour thumbColor;
};