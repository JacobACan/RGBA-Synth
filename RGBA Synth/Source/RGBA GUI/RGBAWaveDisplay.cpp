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
	: waveDisplayApvts(apvts),
	maxWaveHeight(0)
{
	targetLevel = waveDisplayApvts.getRawParameterValue("targetLevel");

	swtLevel = waveDisplayApvts.getRawParameterValue("swtLevel");
	sawLevel = waveDisplayApvts.getRawParameterValue("sawLevel");
	sqrLevel = waveDisplayApvts.getRawParameterValue("sqrLevel");

	startTimer(100);
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

	juce::Image waveDisplayBackground = juce::ImageCache::getFromMemory(BinaryData::RGBA_Synth_Display_png, BinaryData::RGBA_Synth_Display_pngSize);
	g.drawImage(waveDisplayBackground, bounds, juce::RectanglePlacement::stretchToFit);


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
	juce::PathStrokeType strokeType(.8, juce::PathStrokeType::curved);
	g.setOpacity(.8);

	juce::ColourGradient redLinearGradient(juce::Colours::red, 0.f, 0.f, juce::Colours::pink, getWidth(), 0.f, false);
	g.setGradientFill(redLinearGradient);
	g.strokePath(redPath, strokeType);

	juce::ColourGradient greenLinearGradient(juce::Colours::green, 0.f, 0.f, juce::Colours::greenyellow, getWidth(), getHeight(), false);
	g.setGradientFill(greenLinearGradient);
	g.strokePath(greenPath, strokeType);

	juce::ColourGradient blueLinearGradient(juce::Colours::blue, 0.f, 0.f, juce::Colours::blueviolet, getWidth(), getHeight(), false);
	g.setGradientFill(blueLinearGradient);
	g.strokePath(bluePath, strokeType);

	juce::ColourGradient whiteLinearGradient(juce::Colours::white, 0.f, 0.f, juce::Colours::wheat, getWidth(), getHeight(), false);
	g.setGradientFill(whiteLinearGradient);
	g.strokePath(resultPath, strokeType);

	juce::Image waveDisplayBorder = juce::ImageCache::getFromMemory(BinaryData::RGBA_Synth_Display_Border_png, BinaryData::RGBA_Synth_Display_Border_pngSize);
	g.drawImage(waveDisplayBorder, bounds, juce::RectanglePlacement::stretchToFit);
}

void RGBAWaveDisplay::updatePreviousValues()
{
	prevSawLevel = sawLevel->load();
	prevSwtLevel = swtLevel->load();
	prevSqrLevel = sqrLevel->load();
}

bool RGBAWaveDisplay::previousValuesDifferentFromCurrentValues()
{
	return swtLevel->load() != prevSwtLevel || sawLevel->load() != prevSawLevel || sqrLevel->load() != prevSqrLevel;
}




void RGBAWaveDisplay::timerCallback()
{
	if (previousValuesDifferentFromCurrentValues())
		repaint();


	updatePreviousValues();
}
