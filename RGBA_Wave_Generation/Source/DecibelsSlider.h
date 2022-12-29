/*
  ==============================================================================

    DecibelsSlider.h
    Created: 28 Dec 2022 11:41:42am
    Author:  13308

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DecibelSlider : public juce::Slider
{
public:
    juce::String getTextFromValue(double value) override;
    
private:

};
