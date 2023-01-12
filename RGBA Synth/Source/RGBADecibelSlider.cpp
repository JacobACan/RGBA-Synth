/*
  ==============================================================================

    DecibelSlider.cpp
    Created: 28 Dec 2022 11:40:33am
    Author:  Jacob

  ==============================================================================
*/

#include "RGBADecibelSlider.h"

juce::String RGBADecibelSlider::getTextFromValue(double value)
{
    return juce::Decibels::toString(value);

}

double RGBADecibelSlider::getValueFromText(const juce::String& text)
{
    double minusInfDb = -100;

    auto decibelText = text.upToFirstOccurrenceOf("db", false, false).trim();

    return text.equalsIgnoreCase("-INF") ? minusInfDb //Case insensitive comparison with another string
                                        : decibelText.getDoubleValue(); 
}

double RGBADecibelSlider::getRGBvalue(RGBADecibelSlider& slider) {
    double maxValue = slider.getMaximum();
    double scale = 255 / maxValue;
    return slider.getValue()* scale;
}

double RGBADecibelSlider::getLevelValue(RGBADecibelSlider& slider) {
    double maxValue = slider.getMaximum();
    double scale = maxValue * 64;
    return slider.getValue() / scale; // Return a value that is a fraction of the value from 0 to 1
}






