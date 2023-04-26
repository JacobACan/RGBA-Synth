/*
  ==============================================================================

    RGBAHorizontalSlider.h
    Created: 24 Apr 2023 12:17:24pm
    Author:  13308

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RGBAHorizontalSlider : public juce::Slider {
public :
    RGBAHorizontalSlider();
private :
    juce::Colour thumbColor;
};