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

    float yPosAmplitudeResult = (std::sin(offsetX * widthToRadiansFactor)
        + (WaveGen::sqr(offsetX * widthToRadiansFactor) * sqrLevel)
        + (WaveGen::saw(offsetX * widthToRadiansFactor) * sawLevel)
        + (WaveGen::swt(offsetX * widthToRadiansFactor) * swtLevel))
        / maxWaveHeight + offsetAmplitudeY;
    float prevYResult = (yPosAmplitudeResult / amplitudeToRadians);;
    float prevXResult = offsetX;

    float opacityFactor = .3;

    juce::Colour red = juce::Colour::fromFloatRGBA(1, 0, 0, swtLevel * opacityFactor);
    juce::Colour green = juce::Colour::fromFloatRGBA(0, 1, 0, sawLevel * opacityFactor);
    juce::Colour blue = juce::Colour::fromFloatRGBA(0, 0, 1, sqrLevel * opacityFactor);

    auto redPath = new juce::Path();
    auto greenPath = new juce::Path();
    auto bluePath = new juce::Path();
    auto resultPath = new juce::Path();

    for (float i = 0; i <= resolution; i++)
    {
        float xPos = i * resolutionFactor + offsetX;

        float xPosRadiads = xPos * widthToRadiansFactor;

        int thickness = 1;
        int interval = 4;
        int z = 0;
        z += interval;


        yPosAmplitudeSwt = (WaveGen::swt(xPosRadiads) * swtLevel + offsetAmplitudeY);
        float yPos = (yPosAmplitudeSwt / amplitudeToRadians) ;
        g.setColour(red);
        juce::Line<float> swtLine (prevXSwt + z, prevYSwt + z, xPos + z, yPos + z);
        redPath->addLineSegment(swtLine, thickness);
        prevXSwt = xPos;
        prevYSwt = yPos;


        z += interval;
        yPosAmplitudeSaw = (WaveGen::saw(xPosRadiads) * sawLevel + offsetAmplitudeY);
        yPos = (yPosAmplitudeSaw / amplitudeToRadians) ;
        juce::Line<float> sawLine(prevXSaw + z, prevYSaw + z, xPos + z, yPos + z);
        greenPath->addLineSegment(sawLine, thickness);
        prevXSaw = xPos;
        prevYSaw = yPos;
        
        z += interval;
        yPosAmplitudeSqr = (WaveGen::sqr(xPosRadiads) * sqrLevel + offsetAmplitudeY);
        yPos = (yPosAmplitudeSqr / amplitudeToRadians);
        juce::Line<float> sqrLine(prevXSqr + z, prevYSqr + z, xPos + z, yPos + z);
        bluePath->addLineSegment(sqrLine, thickness);
        prevXSqr = xPos;
        prevYSqr = yPos;

        z += interval;
        yPosAmplitudeResult = (std::sin(xPosRadiads) 
            + (WaveGen::sqr(xPosRadiads) * sqrLevel)
            + (WaveGen::saw(xPosRadiads) * sawLevel)
            + (WaveGen::swt(xPosRadiads) * swtLevel)) 
                / maxWaveHeight + offsetAmplitudeY;
        yPos = (yPosAmplitudeResult / amplitudeToRadians);
        juce::Line<float> resultLine(prevXResult + z, prevYResult + z, xPos + z, yPos + z);
        resultPath->addLineSegment(resultLine, thickness);
        prevXResult = xPos;
        prevYResult = yPos;

        
    }
    juce::PathStrokeType strokeType(3, juce::PathStrokeType::curved);

    g.setColour(red);
    g.strokePath(*redPath, strokeType);

    g.setColour(green);
    g.strokePath(*greenPath, strokeType);

    g.setColour(blue);
    g.strokePath(*bluePath, strokeType);

    g.setColour(juce::Colours::white);
    g.strokePath(*resultPath, strokeType);

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