/*
  ==============================================================================

    AlphaSlider.h
    Created: 23 Apr 2023 8:45:46am
    Author:  13308

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AlphaSlider : public juce::Slider {
public :
    juce::String getTextFromValue(double currentValue) override;

private :


};