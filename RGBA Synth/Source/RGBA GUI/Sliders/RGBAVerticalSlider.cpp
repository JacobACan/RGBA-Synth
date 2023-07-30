/*
  ==============================================================================

	RGBAVerticalSlider.cpp
	Created: 24 Apr 2023 12:17:24pm
	Author:  13308

  ==============================================================================
*/

#include "RGBAVerticalSlider.h"

class RGBALinearSliderLookAndFeel : public juce::LookAndFeel_V4 {
public:

	RGBALinearSliderLookAndFeel(juce::Colour thumbColor) : thumbColor(thumbColor)
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
		if (style == juce::Slider::SliderStyle::LinearHorizontal)
		{
			DBG("No Horizontal Linear Slider Made for RGBA Linear Slider");
		}
		else if (style == juce::Slider::SliderStyle::LinearVertical)
		{
			constexpr float maxrange = 255;
			const float minimumSliderPos = (0 + getSliderThumbRadius(slider));
			const float totalsliderdistance = (float)height;
			const float calculatedBackgroundColorValue = maxrange - ((maxrange) / (totalsliderdistance)) * (sliderPos - minimumSliderPos);
			juce::Colour calculatedbackgroundcolor;

			coloursEqual(this->thumbColor, juce::Colours::red)
				? calculatedbackgroundcolor = juce::Colour(calculatedBackgroundColorValue, 0, 0)
				: coloursEqual(this->thumbColor, juce::Colours::green)
				? calculatedbackgroundcolor = juce::Colour(0, calculatedBackgroundColorValue, 0)
				: coloursEqual(this->thumbColor, juce::Colours::blue)
				? calculatedbackgroundcolor = juce::Colour(0, 0, calculatedBackgroundColorValue)
				: calculatedbackgroundcolor = juce::Colour(calculatedBackgroundColorValue, calculatedBackgroundColorValue, calculatedBackgroundColorValue);

			juce::ColourGradient calcColourGradient(juce::Colours::black, 0, 0, calculatedbackgroundcolor, 0, sliderPos + 50, false);
			g.setGradientFill(calcColourGradient);
			constexpr int padding = 5;
			const juce::Rectangle<float> sliderBackgroundColorBounds(0 + width / 4 + padding, 0 + getSliderThumbRadius(slider), width / 2 - padding, height - 2);
			g.fillRect(sliderBackgroundColorBounds);

			const juce::Image sliderBackground = juce::ImageCache::getFromMemory(BinaryData::RGBA_Synth_Vertical_Slider_Background_png, BinaryData::RGBA_Synth_Vertical_Slider_Background_pngSize);
			const juce::Rectangle<float> sliderBounds(0, 0 + getSliderThumbRadius(slider), width, height);
			g.drawImage(sliderBackground, sliderBounds, juce::RectanglePlacement::stretchToFit);
		}
	};

	void drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
	{
		if (style == juce::Slider::SliderStyle::LinearHorizontal)
		{
			DBG("No Horizontal Linear Slider Made for RGBA Linear Slider");
		}
		else if (style == juce::Slider::SliderStyle::LinearVertical)
		{
			const juce::Rectangle<float> sliderBounds(0, sliderPos - getSliderThumbRadius(slider), static_cast<float>(width), static_cast<float>(width) * (2.f / 3.f));

			const juce::ColourGradient knobGradient(this->thumbColor, 0, 0, juce::Colours::black, width, static_cast<float>(height) + static_cast<float>(height) / 2, false);
			g.setGradientFill(knobGradient);
			g.fillRoundedRectangle(sliderBounds, 10);

			const juce::Image sliderKnob = juce::ImageCache::getFromMemory(BinaryData::RGBA_Synth_Red_Slider_Knob_png, BinaryData::RGBA_Synth_Red_Slider_Knob_pngSize);
			g.drawImage(sliderKnob, sliderBounds, juce::RectanglePlacement::fillDestination);
		}

	};

	bool coloursEqual(juce::Colour c1, juce::Colour c2)
	{
		return c1.getRed() == c2.getRed() && c1.getGreen() == c2.getGreen() && c1.getBlue() == c2.getBlue();
	}
private:
	juce::Colour thumbColor;

};


RGBAVerticalSlider::RGBAVerticalSlider() : thumbColor(juce::Colours::white)
{
	setSliderStyle(juce::Slider::LinearVertical);
	setTextBoxStyle(juce::Slider::NoTextBox, true, getWidth(), getWidth() * 2 / 3);
	setLookAndFeel(new RGBALinearSliderLookAndFeel(this->thumbColor));
}

RGBAVerticalSlider::RGBAVerticalSlider(juce::Colour thumbColor) : thumbColor(thumbColor)
{
	setSliderStyle(juce::Slider::LinearVertical);
	setTextBoxStyle(juce::Slider::NoTextBox, true, getWidth(), getWidth() * 2 / 3);
	setLookAndFeel(new RGBALinearSliderLookAndFeel(this->thumbColor));
}
