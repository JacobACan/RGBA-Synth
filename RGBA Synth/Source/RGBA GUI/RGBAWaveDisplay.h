/*
  ==============================================================================

    WaveDisplay.h
    Created: 6 Jan 2023 7:34:50pm
    Author:  Jacob

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class RGBAWaveDisplay : public juce::Component
{
public:
    RGBAWaveDisplay(juce::AudioProcessorValueTreeState& apvts);
    ~RGBAWaveDisplay() override;

    //==========================================================================
    void paint(juce::Graphics& g) override;

    //==========================================================================

private:
    juce::AudioProcessorValueTreeState& waveDisplayApvts;

    std::atomic<float>* targetLevel = nullptr;

    std::atomic<float>* swtLevel = nullptr;
    std::atomic<float>* sawLevel = nullptr;
    std::atomic<float>* sqrLevel = nullptr;
    float maxWaveHeight = 0;
};