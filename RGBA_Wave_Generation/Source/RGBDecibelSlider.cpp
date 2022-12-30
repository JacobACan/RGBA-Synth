/*
  ==============================================================================

    DecibelSlider.cpp
    Created: 28 Dec 2022 11:40:33am
    Author:  Jacob

  ==============================================================================
*/

#include "RGBDecibelsSlider.h"

juce::String RGBDecibelSlider::getTextFromValue(double value)
{
    return juce::Decibels::toString(value);

}

double RGBDecibelSlider::getValueFromText(const juce::String& text)
{
    double minusInfDb = -100;

    auto decibelText = text.upToFirstOccurrenceOf("db", false, false).trim();

    return text.equalsIgnoreCase("-INF") ? minusInfDb //Case insensitive comparison with another string
                                        : decibelText.getDoubleValue(); 
}

double RGBDecibelSlider::getRGBvalue(RGBDecibelSlider& slider) {
    double maxValue = slider.getMaximum();
    double scale = 255 / maxValue;
    return slider.getValue()* scale;
}

double RGBDecibelSlider::getLevelValue(RGBDecibelSlider& slider) {
    double maxValue = slider.getMaximum();
    double scale = maxValue * 64;
    return slider.getValue() / scale; // Return a value that is a fraction of the value from 0 to 1
}







