/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RGBASynthAudioProcessorEditor::RGBASynthAudioProcessorEditor(RGBASynthAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
	: AudioProcessorEditor(p),
	editorApvts(apvts),
	waveDisplay(apvts),
	keyboardComponent(p.keyboardState, juce::KeyboardComponentBase::verticalKeyboardFacingLeft),
	swtLevelSlider(juce::Colours::red),
	sawLevelSlider(juce::Colours::green),
	sqrLevelSlider(juce::Colours::blue)

{
	setSize(958, 564);
	backgroundColor = juce::Colour::fromFloatRGBA(0, 0, 0, 0);
	static const juce::Typeface::Ptr pressStart2P{ juce::Typeface::createSystemTypefaceFor(BinaryData::PressStart2PRegular_ttf, BinaryData::PressStart2PRegular_ttfSize) };
	getLookAndFeel().setDefaultSansSerifTypeface(pressStart2P);



	addAndMakeVisible(waveDisplay);
	addAndMakeVisible(keyboardComponent);

	// Alpha ==============================================
	addAndMakeVisible(targetLevelSlider);
	targetLevelSlider.onValueChange = [this]
	{
		backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
		repaint();
	};
	targetLevelAttatchment.reset(new SliderAttachment(editorApvts, "targetLevel", targetLevelSlider));

	// Red =================================================
	addAndMakeVisible(swtLevelSlider);
	swtLevelSlider.onValueChange = [this]
	{
		backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
		repaint();
	};
	swtLevelAttatchment.reset(new SliderAttachment(editorApvts, "swtLevel", swtLevelSlider));

	// Green =================================================
	addAndMakeVisible(sawLevelSlider);
	sawLevelSlider.onValueChange = [this]
	{
		backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
		repaint();
	};
	sawLevelAttatchment.reset(new SliderAttachment(editorApvts, "sawLevel", sawLevelSlider));

	// Blue =================================================
	addAndMakeVisible(sqrLevelSlider);
	sqrLevelSlider.onValueChange = [this]
	{
		backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
		repaint();
	};
	sqrLevelAttatchment.reset(new SliderAttachment(editorApvts, "sqrLevel", sqrLevelSlider));

	// Detune ================================================
	addAndMakeVisible(detuneAmountSlider);
	detuneAmountAttatchment.reset(new SliderAttachment(editorApvts, "detuneAmount", detuneAmountSlider));

	//Phase
	addAndMakeVisible(swtPhaseSlider);
	swtPhaseSlider.onValueChange = [this] {
		WaveGen::setSwtPhase(swtPhaseSlider.getValue());
		waveDisplay.repaint();
	};
	swtPhaseAttatchment.reset(new SliderAttachment(editorApvts, "swtPhase", swtPhaseSlider));

	addAndMakeVisible(sawPhaseSlider);
	sawPhaseSlider.onValueChange = [this] {
		WaveGen::setSawPhase(sawPhaseSlider.getValue());
		waveDisplay.repaint();
	};
	sawPhaseAttatchment.reset(new SliderAttachment(editorApvts, "sawPhase", sawPhaseSlider));

	addAndMakeVisible(sqrPhaseSlider);
	sqrPhaseSlider.onValueChange = [this] {
		WaveGen::setSqrPhase(sqrPhaseSlider.getValue());
		waveDisplay.repaint();
	};
	sqrPhaseAttatchment.reset(new SliderAttachment(editorApvts, "sqrPhase", sqrPhaseSlider));


}

RGBASynthAudioProcessorEditor::~RGBASynthAudioProcessorEditor()
{

}

//==============================================================================
void RGBASynthAudioProcessorEditor::paint(juce::Graphics& g)
{

	int lightThreshold = 210;


	g.fillAll(backgroundColor);
	g.setFont(50.f);

	g.setColour(juce::Colours::red);
	g.drawText("RGBA Synth", 0, 10, getWidth(), getHeight(), juce::Justification::centredTop);
	g.setColour(juce::Colours::green);
	g.drawText("RGBA Synth", 2, 12, getWidth(), getHeight(), juce::Justification::centredTop);
	g.setColour(juce::Colours::blue);
	g.drawText("RGBA Synth", 4, 14, getWidth(), getHeight(), juce::Justification::centredTop);

	g.drawText("RGBA Synth", 6, 16, getWidth(), getHeight(), juce::Justification::centredTop);

	(targetLevelSlider.getValue() * 255 > lightThreshold
		&& swtLevelSlider.getValue() > lightThreshold
		&& sawLevelSlider.getValue() > lightThreshold &&
		sqrLevelSlider.getValue() > lightThreshold)
		? g.setColour(juce::Colours::black)
		: g.setColour(juce::Colours::white);

	g.drawText("RGBA Synth", 6, 16, getWidth(), getHeight(), juce::Justification::centredTop);
}

void RGBASynthAudioProcessorEditor::resized()
{
	int constexpr titleHeight = 80;

	int const  waveDisplayWidth = getWidth() * .8;
	int const keyboardWidth = waveDisplayWidth * .1;

	int const leftSideOfWaveDisplay = getWidth() * .1 + keyboardWidth / 2;
	int rightSideOfWaveDisplay = leftSideOfWaveDisplay + waveDisplayWidth;
	int const waveDisplayHeight = getHeight() * .5;
	int const bottomOfWaveDisplay = titleHeight + waveDisplayHeight;

	int const volumeSliderWidth = waveDisplayWidth * .05;
	int const rgbSliderWidth = waveDisplayWidth;
	int constexpr rgbSliderHieght = 40;


	//Sliders
	juce::Rectangle<int> rgbSliderRect(leftSideOfWaveDisplay, bottomOfWaveDisplay, waveDisplayWidth, rgbSliderHieght);
	swtLevelSlider.setBounds(rgbSliderRect);

	rgbSliderRect.setPosition(leftSideOfWaveDisplay, bottomOfWaveDisplay + rgbSliderHieght);
	sawLevelSlider.setBounds(rgbSliderRect);

	rgbSliderRect.setPosition(leftSideOfWaveDisplay, bottomOfWaveDisplay + 2 * rgbSliderHieght);
	sqrLevelSlider.setBounds(rgbSliderRect);


	juce::Rectangle<int> alphaSliderRect(rightSideOfWaveDisplay -= volumeSliderWidth, titleHeight, volumeSliderWidth, waveDisplayHeight);
	targetLevelSlider.setBounds(alphaSliderRect);

	juce::Rectangle<int> phaseRect(keyboardWidth, rgbSliderHieght);

	phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay);
	swtPhaseSlider.setBounds(phaseRect);

	phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay + rgbSliderHieght);
	sawPhaseSlider.setBounds(phaseRect);

	phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay + 2 * rgbSliderHieght);
	sqrPhaseSlider.setBounds(phaseRect);

	phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay + 3 * rgbSliderHieght);
	detuneAmountSlider.setBounds(phaseRect);


	//Keyboard Component
	keyboardComponent.setBounds(leftSideOfWaveDisplay - keyboardWidth, titleHeight, keyboardWidth, waveDisplayHeight);


	waveDisplay.setBounds(leftSideOfWaveDisplay, titleHeight, waveDisplayWidth, waveDisplayHeight);
}
