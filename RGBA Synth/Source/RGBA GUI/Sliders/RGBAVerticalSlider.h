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

class RGBALinearSliderLookAndFeel : public  juce::LookAndFeel_V4
{
public:
	RGBALinearSliderLookAndFeel();
	RGBALinearSliderLookAndFeel(juce::Colour thumbColor);

	void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override;
	void drawLinearSliderBackground(juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider&) override;
	void drawLinearSliderThumb(juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override;
	void drawLabel(juce::Graphics&, juce::Label&) override;

private:
	juce::Colour thumbColor;
};


class RGBAVerticalSlider : public juce::Slider {
public:
	RGBAVerticalSlider();
	RGBAVerticalSlider(juce::Colour thumbColor, juce::String sliderName = "");
	void paint(juce::Graphics& g) override;



	juce::Colour thumbColor;
private:
	juce::String sliderName;
};