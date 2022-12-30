/*
  ==============================================================================

    DecibelsSlider.h
    Created: 28 Dec 2022 11:41:42am
    Author:  Jacob

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DecibelSlider : public juce::Slider
{
public:
    juce::String getTextFromValue(double value) override;
    double getValueFromText(const juce::String& text) override;
    
private:

};
