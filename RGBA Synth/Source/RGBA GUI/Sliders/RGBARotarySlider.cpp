/*
  ==============================================================================

	RGBARadialSlider.cpp
	Created: 30 Jul 2023 6:32:50pm
	Author:  jcane

  ==============================================================================
*/

#include "RGBARotarySlider.h"

class RGBARotaryLookAndFeel : public juce::LookAndFeel_V4
{
public:
	RGBARotaryLookAndFeel() {}
	RGBARotaryLookAndFeel(juce::Colour rotarySliderColour) : rotarySliderColour(rotarySliderColour) {}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
	{
		const juce::Rectangle<float> sliderBounds(x, y, width, height);
		const juce::ColourGradient sliderColorGradient(this->rotarySliderColour, x, y, juce::Colours::grey, width, height, false);
		const juce::Image radialSlider(juce::ImageCache::getFromMemory(BinaryData::RadialSliderAsset_png, BinaryData::RadialSliderAsset_pngSize));
		g.setGradientFill(sliderColorGradient);
		g.fillEllipse(sliderBounds);
		g.addTransform(juce::AffineTransform::rotation((rotaryEndAngle - rotaryStartAngle) * sliderPosProportional, static_cast<float>(width) / 2, static_cast<float>(height) / 2));
		g.drawImage(radialSlider, sliderBounds, juce::RectanglePlacement::stretchToFit);
	};

private:
	juce::Colour rotarySliderColour;
};


RGBARotarySlider::RGBARotarySlider()
{
	setSliderStyle(juce::Slider::RotaryVerticalDrag);
	setLookAndFeel(new RGBARotaryLookAndFeel(juce::Colours::white));
}


RGBARotarySlider::RGBARotarySlider(juce::Colour thumbColor)
{
	setSliderStyle(juce::Slider::RotaryVerticalDrag);
	setLookAndFeel(new RGBARotaryLookAndFeel(thumbColor));
}
