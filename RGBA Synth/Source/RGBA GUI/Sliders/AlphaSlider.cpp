/*
  ==============================================================================

    AlphaSlider.cpp
    Created: 23 Apr 2023 8:45:46am
    Author:  13308

  ==============================================================================
*/

#include "AlphaSlider.h"



AlphaSlider::AlphaSlider()
{
    setSliderStyle(juce::Slider::LinearBarVertical);
}

juce::String AlphaSlider::getTextFromValue(double currentValue)
{
    double gainInDecibels = juce::Decibels::gainToDecibels<double>(currentValue);
    float roundingToOneDecimal = (int)(gainInDecibels * 10);
    gainInDecibels = roundingToOneDecimal / 10;
    return ((juce::String)gainInDecibels) << "db";
}
