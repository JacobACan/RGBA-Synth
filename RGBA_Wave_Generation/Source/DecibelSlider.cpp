/*
  ==============================================================================

    DecibelSlider.cpp
    Created: 28 Dec 2022 11:40:33am
    Author:  Jacob

  ==============================================================================
*/

#include "DecibelsSlider.h"

juce::String DecibelSlider::getTextFromValue(double value)
{
    return juce::Decibels::toString(value);

}

double DecibelSlider::getValueFromText(const juce::String& text)
{
    double minusInfDb = -100;

    auto decibelText = text.upToFirstOccurrenceOf("db", false, false).trim();

    return text.equalsIgnoreCase("-INF") ? minusInfDb //Case insensitive comparison with another string
                                        : decibelText.getDoubleValue(); 
}






