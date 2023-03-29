/*
  ==============================================================================

    WaveDisplay.cpp
    Created: 6 Jan 2023 7:35:10pm
    Author:  Jacob

  ==============================================================================
*/

#include "../RGBA Processing/WaveGen.h"
#include "RGBAWaveDisplay.h"

RGBAWaveDisplay::RGBAWaveDisplay(juce::AudioProcessorValueTreeState& apvts) 
    : waveDisplayApvts(apvts)
{
    targetLevel = waveDisplayApvts.getRawParameterValue("targetLevel");

    swtLevel = waveDisplayApvts.getRawParameterValue("swtLevel");
    sawLevel = waveDisplayApvts.getRawParameterValue("sawLevel");
    sqrLevel = waveDisplayApvts.getRawParameterValue("sqrLevel");
}

RGBAWaveDisplay::~RGBAWaveDisplay() {

}



void RGBAWaveDisplay::paint(juce::Graphics& g)
{
    juce::Rectangle<float> bounds(juce::Point<float>(0, 0), juce::Point<float>(getWidth(), getHeight()));

    g.setColour(juce::Colours::white);

    maxWaveHeight = 1 + swtLevel->load() + sawLevel->load() + sqrLevel->load();

    float resolution = 128;
    float widthToRadiansFactor = juce::MathConstants<float>::twoPi / getWidth();
    float resolutionFactor = getWidth() / resolution;
    float amplitudeToRadians = juce::MathConstants<float>::twoPi / getHeight();

    float offsetAmplitudeY = 3;
    float offsetX = -8;

    float yPosAmplitudeSwt = (WaveGen::swt(offsetX * widthToRadiansFactor) * swtLevel->load() + offsetAmplitudeY);
    float prevXSwt = offsetX;
    float prevYSwt = yPosAmplitudeSwt / amplitudeToRadians;

    float yPosAmplitudeSaw = (WaveGen::saw(offsetX * widthToRadiansFactor) * sawLevel->load() + offsetAmplitudeY);
    float prevXSaw = offsetX;
    float prevYSaw = yPosAmplitudeSaw / amplitudeToRadians;

    float yPosAmplitudeSqr = (WaveGen::sqr(offsetX * widthToRadiansFactor) * sqrLevel->load() + offsetAmplitudeY);
    float prevXSqr = offsetX;
    float prevYSqr = yPosAmplitudeSqr / amplitudeToRadians;

    float yPosAmplitudeResult = (std::sin(offsetX * widthToRadiansFactor)
        + (WaveGen::sqr(offsetX * widthToRadiansFactor) * sqrLevel->load())
        + (WaveGen::saw(offsetX * widthToRadiansFactor) * sawLevel->load())
        + (WaveGen::swt(offsetX * widthToRadiansFactor) * swtLevel->load()))
        / maxWaveHeight + offsetAmplitudeY;
    float prevYResult = (yPosAmplitudeResult / amplitudeToRadians);;
    float prevXResult = offsetX;

    float opacityFactor = .5;
    int thickness = 1;
    int zInterval = 4;

    juce::Colour red = juce::Colour::fromFloatRGBA(1, 0, 0, swtLevel->load() * opacityFactor);
    juce::Colour green = juce::Colour::fromFloatRGBA(0, 1, 0, sawLevel->load() * opacityFactor);
    juce::Colour blue = juce::Colour::fromFloatRGBA(0, 0, 1, sqrLevel->load() * opacityFactor);

    juce::Path redPath = juce::Path();
    juce::Path greenPath = juce::Path();
    juce::Path bluePath = juce::Path();
    juce::Path resultPath = juce::Path();

    for (float i = 0; i <= resolution; i++)
    {
        float xPos = i * resolutionFactor + offsetX;

        float xPosRadiads = xPos * widthToRadiansFactor;

        int z = 0;
        z += zInterval;


        yPosAmplitudeSwt = (WaveGen::swt(xPosRadiads) * swtLevel->load() + offsetAmplitudeY);
        float yPos = (yPosAmplitudeSwt / amplitudeToRadians);
        g.setColour(red);
        juce::Line<float> swtLine(prevXSwt + z, prevYSwt + z, xPos + z, yPos + z);
        redPath.addLineSegment(swtLine, thickness);
        prevXSwt = xPos;
        prevYSwt = yPos;


        z += zInterval;
        yPosAmplitudeSaw = (WaveGen::saw(xPosRadiads) * sawLevel->load() + offsetAmplitudeY);
        yPos = (yPosAmplitudeSaw / amplitudeToRadians);
        juce::Line<float> sawLine(prevXSaw + z, prevYSaw + z, xPos + z, yPos + z);
        greenPath.addLineSegment(sawLine, thickness);
        prevXSaw = xPos;
        prevYSaw = yPos;

        z += zInterval;
        yPosAmplitudeSqr = (WaveGen::sqr(xPosRadiads) * sqrLevel->load() + offsetAmplitudeY);
        yPos = (yPosAmplitudeSqr / amplitudeToRadians);
        juce::Line<float> sqrLine(prevXSqr + z, prevYSqr + z, xPos + z, yPos + z);
        bluePath.addLineSegment(sqrLine, thickness);
        prevXSqr = xPos;
        prevYSqr = yPos;

        z += zInterval;
        yPosAmplitudeResult = (std::sin(xPosRadiads)
            + (WaveGen::sqr(xPosRadiads) * sqrLevel->load())
            + (WaveGen::saw(xPosRadiads) * sawLevel->load())
            + (WaveGen::swt(xPosRadiads) * swtLevel->load()))
            / maxWaveHeight + offsetAmplitudeY;
        yPos = (yPosAmplitudeResult / amplitudeToRadians);
        juce::Line<float> resultLine(prevXResult + z, prevYResult + z, xPos + z, yPos + z);
        resultPath.addLineSegment(resultLine, thickness);
        prevXResult = xPos;
        prevYResult = yPos;


    }
    juce::PathStrokeType strokeType(3, juce::PathStrokeType::curved);

    g.setColour(red);
    g.strokePath(redPath, strokeType);

    g.setColour(green);
    g.strokePath(greenPath, strokeType);

    g.setColour(blue);
    g.strokePath(bluePath, strokeType);

    g.setColour(juce::Colours::white);
    g.strokePath(resultPath, strokeType);

    g.drawRoundedRectangle(bounds, 10, 10);

}
