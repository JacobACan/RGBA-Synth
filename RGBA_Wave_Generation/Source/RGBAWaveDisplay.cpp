/*
  ==============================================================================

    WaveDisplay.cpp
    Created: 6 Jan 2023 7:35:10pm
    Author:  Jacob

  ==============================================================================
*/

#include "RGBAWaveDisplay.h"

RGBAWaveDisplay::RGBAWaveDisplay() : swtLevel (0),
                             sawLevel (0),
                             sqrLevel (0),
                             level (1), 
                             maxWaveHeight (1)
{
}

RGBAWaveDisplay::~RGBAWaveDisplay() {

}



void RGBAWaveDisplay::paint(juce::Graphics& g)
{
    juce::Rectangle<float> bounds(juce::Point<float>(0, 0), juce::Point<float>(getWidth(), getHeight()));

    g.setColour(juce::Colours::white);

    float resolution = 64;
    float widthToRadiansFactor = juce::MathConstants<float>::twoPi / getWidth();
    float resolutionFactor = getWidth() / resolution;
    float amplitudeToRadians = juce::MathConstants<float>::twoPi / getHeight();

    float offsetAmplitudeY = 3;
    float offsetX = -8;

    float yPosAmplitudeSwt = (WaveGen::swt(offsetX * widthToRadiansFactor ) * swtLevel + offsetAmplitudeY);
    float prevXSwt = offsetX;
    float prevYSwt = yPosAmplitudeSwt / amplitudeToRadians;

    float yPosAmplitudeSaw = (WaveGen::saw(offsetX * widthToRadiansFactor ) * sawLevel + offsetAmplitudeY);
    float prevXSaw = offsetX;
    float prevYSaw = yPosAmplitudeSaw / amplitudeToRadians;

    float yPosAmplitudeSqr = (WaveGen::sqr(offsetX * widthToRadiansFactor) * sqrLevel + offsetAmplitudeY);
    float prevXSqr = offsetX;
    float prevYSqr = yPosAmplitudeSqr / amplitudeToRadians;

    float yPosAmplitudeResult = (std::sin(offsetX * widthToRadiansFactor) * level + offsetAmplitudeY);
    float prevXResult = offsetX;
    float prevYResult = yPosAmplitudeResult / amplitudeToRadians;

    juce::Colour red = juce::Colour::fromFloatRGBA(1, 0, 0, swtLevel);
    juce::Colour green = juce::Colour::fromFloatRGBA(0, 1, 0, sawLevel);
    juce::Colour blue = juce::Colour::fromFloatRGBA(0, 0, 1, sqrLevel);

    for (float i = 0; i <= resolution; i++)
    {
        float xPos = i * resolutionFactor + offsetX;

        float xPosRadiads = xPos * widthToRadiansFactor;

        int thickness = 2;
        int interval = 4;
        int z = 0;
        z += interval;


        yPosAmplitudeSwt = (WaveGen::swt(xPosRadiads) * swtLevel + offsetAmplitudeY);
        float yPos = (yPosAmplitudeSwt / amplitudeToRadians) ;
        g.setColour(red);
        g.drawLine(prevXSwt + z, prevYSwt + z, xPos + z, yPos + z, thickness);
        prevXSwt = xPos;
        prevYSwt = yPos;


        z += interval;
        yPosAmplitudeSaw = (WaveGen::saw(xPosRadiads) * sawLevel + offsetAmplitudeY);
        yPos = (yPosAmplitudeSaw / amplitudeToRadians) ;
        g.setColour(green);
        g.drawLine(prevXSaw + z, prevYSaw + z, xPos + z, yPos + z, thickness);
        prevXSaw = xPos;
        prevYSaw = yPos;
        
        z += interval;
        yPosAmplitudeSqr = (WaveGen::sqr(xPosRadiads) * sqrLevel + offsetAmplitudeY);
        yPos = (yPosAmplitudeSqr / amplitudeToRadians);
        g.setColour(blue);
        g.drawLine(prevXSqr + z, prevYSqr + z, xPos + z, yPos + z, thickness);
        prevXSqr = xPos;
        prevYSqr = yPos;

        g.setOpacity(1);
        z += interval;
        yPosAmplitudeResult = (std::sin(xPosRadiads) 
            + (WaveGen::sqr(xPosRadiads) * sqrLevel)
            + (WaveGen::saw(xPosRadiads) * sawLevel)
            + (WaveGen::swt(xPosRadiads) * swtLevel)) 
                / maxWaveHeight + offsetAmplitudeY;
        yPos = (yPosAmplitudeResult / amplitudeToRadians);
        g.setColour(juce::Colours::white);
        g.drawLine(prevXResult + z, prevYResult + z, xPos + z, yPos + z, 5);
        prevXResult = xPos;
        prevYResult = yPos;
    }

    g.drawRoundedRectangle(bounds, 10, 10);

}

void RGBAWaveDisplay::setSwtLevel(double newSwtLevel)
{
    swtLevel = newSwtLevel;
}

void RGBAWaveDisplay::setSawLevel(double newSawLevel)
{
    sawLevel = newSawLevel;
}

void RGBAWaveDisplay::setSqrLevel(double newSqrLevel)
{
    sqrLevel = newSqrLevel;
}

void RGBAWaveDisplay::setLevel(double newLevel)
{
    level = newLevel;
}

void RGBAWaveDisplay::setMaxWaveHeight(double newMaxWaveHeight)
{
    maxWaveHeight = newMaxWaveHeight;
}