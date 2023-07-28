/*
  ==============================================================================

	RGBAHorizontalSlider.cpp
	Created: 24 Apr 2023 12:17:24pm
	Author:  13308

  ==============================================================================
*/

#include "RGBAHorizontalSlider.h"

class RGBAHorizontalSliderLookAndFeel : public juce::LookAndFeel_V4 {
public:

	RGBAHorizontalSliderLookAndFeel(juce::Colour thumbColor) : thumbColor(thumbColor)
	{}

	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const juce::Slider::SliderStyle sliderStyle, juce::Slider& slider) override {

		if (sliderStyle == juce::Slider::LinearBar || sliderStyle == juce::Slider::LinearBarVertical)
		{
			g.setColour(juce::Colour(255, 255, 255));
			g.drawRect(juce::Rectangle<float>(sliderPos, 0, width, height), 2.f);

		}
		else
		{
			drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, sliderStyle, slider);
			drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, sliderStyle, slider);
		}
	};

	void drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
	{
		constexpr float maxRange = 255;
		const float totalSliderDistance = static_cast<float>( width )+ static_cast<float> (getSliderThumbRadius(slider));
		const float calculatedBackgroundColorValue = (( maxRange) / (totalSliderDistance)) * (sliderPos - minSliderPos);
		juce::Colour calculatedBackgroundColor = juce::Colours::black;

		coloursEqual(this->thumbColor, juce::Colours::red)
			? calculatedBackgroundColor = juce::Colour(calculatedBackgroundColorValue, 0, 0)
			: coloursEqual(this->thumbColor, juce::Colours::green)
			? calculatedBackgroundColor = juce::Colour(0, calculatedBackgroundColorValue, 0)
			: coloursEqual(this->thumbColor, juce::Colours::blue)
			? calculatedBackgroundColor = juce::Colour(0, 0, calculatedBackgroundColorValue)
			: calculatedBackgroundColor = juce::Colour(calculatedBackgroundColorValue, calculatedBackgroundColorValue, calculatedBackgroundColorValue);

		g.setColour(calculatedBackgroundColor);

		g.fillRect(juce::Rectangle<float>(x, static_cast<float>(height) / 2.f - (static_cast<float>(height) / 4.f), static_cast<float>(width) + getSliderThumbRadius(slider), static_cast<float>(height) / 2));
	};

	void drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override
	{
		g.setColour(this->thumbColor);
		g.fillRect(juce::Rectangle<float>(sliderPos, 0, static_cast<float>(height), static_cast<float>(height)));
	};

	bool coloursEqual(juce::Colour c1, juce::Colour c2)
	{
		return c1.getRed() == c2.getRed() && c1.getGreen() == c2.getGreen() && c1.getBlue() == c2.getBlue();
	}



private:
	juce::Colour thumbColor;

};


RGBAHorizontalSlider::RGBAHorizontalSlider() : thumbColor(juce::Colours::black)
{
	setSliderStyle(juce::Slider::LinearHorizontal);
	setLookAndFeel(new RGBAHorizontalSliderLookAndFeel(this->thumbColor));
}

RGBAHorizontalSlider::RGBAHorizontalSlider(juce::Colour thumbColor) : thumbColor(thumbColor)
{
	setSliderStyle(juce::Slider::LinearHorizontal);
	setLookAndFeel(new RGBAHorizontalSliderLookAndFeel(this->thumbColor));
}
