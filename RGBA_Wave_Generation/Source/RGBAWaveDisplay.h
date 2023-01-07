/*
  ==============================================================================

    WaveDisplay.h
    Created: 6 Jan 2023 7:34:50pm
    Author:  Jacob

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WaveGen.h"

class RGBAWaveDisplay : public juce::Component
{
public:
    RGBAWaveDisplay();
    ~RGBAWaveDisplay() override;

    //==========================================================================
    void paint(juce::Graphics& g) override;

    void setSwtLevel(double newSwtLevel);
    void setSawLevel(double newSawLevel);
    void setSqrLevel(double newSqrLevel);
    void setLevel(double newLevel);
    void setMaxWaveHeight(double newMaxWaveHeight);

private:
    int quality;

    double level;

    double swtLevel;
    double sawLevel;
    double sqrLevel;
    double maxWaveHeight;
};