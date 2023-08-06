/*
  ==============================================================================

	RGBAVerticalSlider.cpp
	Created: 24 Apr 2023 12:17:24pm
	Author:  13308

  ==============================================================================
*/

#include "RGBAVerticalSlider.h"




RGBALinearSliderLookAndFeel::RGBALinearSliderLookAndFeel() : thumbColor(RGBAColours::black)
{

}


RGBALinearSliderLookAndFeel::RGBALinearSliderLookAndFeel(juce::Colour thumbColor) : thumbColor(thumbColor)
{

};

void 	RGBALinearSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
	float sliderPos, float minSliderPos, float maxSliderPos,
	const juce::Slider::SliderStyle sliderStyle, juce::Slider& slider) {


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

void RGBALinearSliderLookAndFeel::drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider)
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
		const int calculatedSliderColorRed = (thumbColor.getRed() / maxrange) * calculatedBackgroundColorValue;
		const int calculatedSliderColorGreen = (thumbColor.getGreen() / maxrange) * calculatedBackgroundColorValue;
		const int calculatedSliderColorBlue = (thumbColor.getBlue() / maxrange) * calculatedBackgroundColorValue;
		juce::Colour calculatedbackgroundcolor = juce::Colour(calculatedSliderColorRed, calculatedSliderColorGreen, calculatedSliderColorBlue);

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

void RGBALinearSliderLookAndFeel::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider)
{

	if (style == juce::Slider::SliderStyle::LinearHorizontal)
	{
		DBG("No Horizontal Linear Slider Made for RGBA Linear Slider");
	}
	else if (style == juce::Slider::SliderStyle::LinearVertical)
	{
		const juce::Rectangle<float> sliderBounds(0, sliderPos - getSliderThumbRadius(slider) / y - y, static_cast<float>(width), static_cast<float>(width) * (2.f / 3.f));

		const juce::ColourGradient knobGradient(this->thumbColor, 0, 0, juce::Colours::black, width, static_cast<float>(height) + static_cast<float>(height) / 2, false);
		g.setGradientFill(knobGradient);
		g.fillRoundedRectangle(sliderBounds, 10);

		const juce::Image sliderKnob = juce::ImageCache::getFromMemory(BinaryData::RGBA_Synth_Red_Slider_Knob_png, BinaryData::RGBA_Synth_Red_Slider_Knob_pngSize);
		g.drawImage(sliderKnob, sliderBounds, juce::RectanglePlacement::fillDestination);
	}

};


void RGBALinearSliderLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& sliderLabel)
{
	int textHeight = 30;
	juce::ColourGradient letterGradient(thumbColor, 0, 0, thumbColor.brighter(), sliderLabel.getWidth(), sliderLabel.getHeight(), false);
	g.setGradientFill(letterGradient);
	g.setFont(sliderLabel.getHeight());
	g.drawText(sliderLabel.getText().substring(0, 1), 0, 0, sliderLabel.getWidth(), sliderLabel.getHeight(), juce::Justification::centredBottom);
	juce::ColourGradient letterGradient2(thumbColor.darker(), 0, 0, thumbColor.darker().darker(), sliderLabel.getWidth(), sliderLabel.getHeight(), false);
	g.setGradientFill(letterGradient2);
	g.drawText(sliderLabel.getText().substring(0, 1), 1, 1, sliderLabel.getWidth(), sliderLabel.getHeight(), juce::Justification::centredBottom);
};




RGBAVerticalSlider::RGBAVerticalSlider() : thumbColor(juce::Colours::white), sliderName("")
{
	setSliderStyle(LinearVertical);
	setLookAndFeel(new RGBALinearSliderLookAndFeel(this->thumbColor));
}



RGBAVerticalSlider::RGBAVerticalSlider(juce::Colour thumbColor, juce::String sliderName)
	: thumbColor(thumbColor),
	sliderName(sliderName)

{
	setTextBoxStyle(TextBoxBelow, true, getWidth(), getHeight());
	setSliderStyle(LinearVertical);
	setLookAndFeel(new RGBALinearSliderLookAndFeel(this->thumbColor));
}


void RGBAVerticalSlider::paint(juce::Graphics& g)
{
	Slider::paint(g);
}


