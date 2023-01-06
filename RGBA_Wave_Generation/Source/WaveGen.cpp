/*
  ==============================================================================

    WaveGen.cpp
    Created: 6 Jan 2023 10:49:23am
    Author:  13308

  ==============================================================================
*/

#include "WaveGen.h"

#include "cmath"

#include <JuceHeader.h>

double WaveGen::sqr(double angle) {
    auto sample = 0;
    angle = std::fmod(angle, juce::MathConstants<double>::twoPi);

    angle < juce::MathConstants<float>::pi ? sample = 1 : sample = -1;

    return sample;
}

double WaveGen::saw(double angle) {
    double sample = 0;
    angle = std::fmod(angle, juce::MathConstants<double>::twoPi);

    angle < juce::MathConstants<float>::pi
        ? sample = ((2 * angle) / juce::MathConstants<double>::pi) - 1
        : sample = ((-2 * angle) / juce::MathConstants<double>::pi) + 3;

    return sample;
}


double WaveGen::swt(double angle) {
    double sample = 0;
    angle = std::fmod(angle, juce::MathConstants<double>::twoPi);

    sample = (angle / juce::MathConstants<double>::pi) - 1;

    return sample;
}