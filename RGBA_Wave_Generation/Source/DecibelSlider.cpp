/*
  ==============================================================================

    DecibelSlider.cpp
    Created: 28 Dec 2022 11:40:33am
    Author:  13308

  ==============================================================================
*/

#include "DecibelsSlider.h"

juce::String DecibelSlider::getTextFromValue(double value)
{
    return juce::Decibels::toString(value);
}




