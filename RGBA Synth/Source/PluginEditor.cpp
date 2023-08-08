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
	swtLevelSlider(RGBAColours::red, "Red"),
	sawLevelSlider(RGBAColours::green, "Green"),
	sqrLevelSlider(RGBAColours::blue, "Blue"),
	targetLevelSlider(juce::Colours::white, "Alpha"),
	detuneAmountSlider(juce::Colours::lightpink),
	swtPhaseSlider(RGBAColours::red),
	sawPhaseSlider(RGBAColours::green),
	sqrPhaseSlider(RGBAColours::blue)
{
	setSize(958, 564);
	getLookAndFeel().setDefaultSansSerifTypeface(RGBAFonts::parkLane);

	addAndMakeVisible(waveDisplay);
	//addAndMakeVisible(keyboardComponent);

	// Alpha ==============================================
	addAndMakeVisible(targetLevelSlider);
	targetLevelAttatchment.reset(new SliderAttachment(editorApvts, "targetLevel", targetLevelSlider));
	addAndMakeVisible(targetLevelLabel);
	targetLevelLabel.setText("Alpha", juce::dontSendNotification);
	targetLevelLabel.setLookAndFeel(new RGBALinearSliderLookAndFeel(RGBAColours::black));
	targetLevelLabel.attachToComponent(&targetLevelSlider, false);

	// Red =================================================
	addAndMakeVisible(swtLevelSlider);
	swtLevelAttatchment.reset(new SliderAttachment(editorApvts, "swtLevel", swtLevelSlider));
	swtLevelLabel.setText("Red", juce::dontSendNotification);
	swtLevelLabel.setLookAndFeel(new RGBALinearSliderLookAndFeel(swtLevelSlider.thumbColor));
	swtLevelLabel.attachToComponent(&swtLevelSlider, false);

	// Green =================================================
	addAndMakeVisible(sawLevelSlider);
	sawLevelAttatchment.reset(new SliderAttachment(editorApvts, "sawLevel", sawLevelSlider));
	sawLevelLabel.setText("Green", juce::dontSendNotification);
	sawLevelLabel.setLookAndFeel(new RGBALinearSliderLookAndFeel(sawLevelSlider.thumbColor));
	sawLevelLabel.attachToComponent(&sawLevelSlider, false);

	// Blue =================================================
	addAndMakeVisible(sqrLevelSlider);
	sqrLevelAttatchment.reset(new SliderAttachment(editorApvts, "sqrLevel", sqrLevelSlider));
	sqrLevelLabel.setText("Blue", juce::dontSendNotification);
	sqrLevelLabel.setLookAndFeel(new RGBALinearSliderLookAndFeel(sqrLevelSlider.thumbColor));
	sqrLevelLabel.attachToComponent(&sqrLevelSlider, false);

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

RGBASynthAudioProcessorEditor::~RGBASynthAudioProcessorEditor() {}

//==============================================================================
void RGBASynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	//TODO : Make a RGBA_colours and RGBA_Fonts Class for easier access and separation of concerns


	/*juce::ColourGradient backgroundGradient1(RGBA_red, 0, 0, RGBA_green, getWidth() / 2, 0, false);
	juce::Rectangle<float> half1(0, 0, getWidth() / 2, getHeight());
	g.setGradientFill(backgroundGradient1);
	g.fillRect(half1);

	juce::ColourGradient backgroundGradient2(RGBA_green, getWidth() / 2, 0, RGBA_blue, getWidth(), 0, false);
	juce::Rectangle<float> half2(getWidth() / 2, 0, getWidth(), getHeight());
	g.setGradientFill(backgroundGradient2);
	g.fillRect(half2);*/

	const int width = getWidth();
	const int height = getHeight();

	g.setGradientFill(juce::ColourGradient(RGBAColours::tan, 0, 0, RGBAColours::tan.darker(), 0, height, false));
	g.setOpacity(1);
	g.fillRect(0, 0, width, height);

	constexpr int fontHeight = 175;

	const juce::Point<float> topLeftText(0, fontHeight / 4);
	const juce::Point<float> bottomRightText(width, height);
	juce::Rectangle<float> textBounds(topLeftText, bottomRightText);

	const juce::ColourGradient RGBA_redGreenGradient(RGBAColours::red, topLeftText, RGBAColours::green, bottomRightText, false);
	const juce::ColourGradient RGBA_greenBlueGradient(RGBAColours::green, topLeftText, RGBAColours::blue, bottomRightText, false);
	const juce::ColourGradient RGBA_blueRedGradient(RGBAColours::blue, topLeftText, RGBAColours::red, bottomRightText, false);
	constexpr int layerSpread = 2;

	g.setFont(juce::Font(fontHeight / 2));

	g.setGradientFill(RGBA_blueRedGradient);
	textBounds.setPosition(textBounds.getPosition().getX(), textBounds.getPosition().getY() - layerSpread * 4);
	g.drawText("Rgba Synth", textBounds, juce::Justification::centredTop);
	g.setGradientFill(RGBA_greenBlueGradient);
	textBounds.setPosition(textBounds.getPosition().getX() - layerSpread, textBounds.getPosition().getY() - layerSpread);
	g.drawText("Rgba Synth", textBounds, juce::Justification::centredTop);
	g.setGradientFill(RGBA_redGreenGradient);
	textBounds.setPosition(textBounds.getPosition().getX() - layerSpread, textBounds.getPosition().getY() - layerSpread);
	g.drawText("Rgba Synth", textBounds, juce::Justification::centredTop);
}

void RGBASynthAudioProcessorEditor::resized()
{
	int constexpr titleHeight = 135;

	int constexpr  sliderWidth = 30;
	int indentLeft = 40;
	int const  waveDisplayWidth = getWidth() * .52;
	int const keyboardWidth = waveDisplayWidth * .1;


	int const leftSideOfWaveDisplay = getWidth() * .2 + 40;
	int rightSideOfWaveDisplay = leftSideOfWaveDisplay + waveDisplayWidth;
	int const waveDisplayHeight = getHeight() * .5;
	int const bottomOfWaveDisplay = titleHeight + waveDisplayHeight;

	int const volumeSliderWidth = waveDisplayWidth * .05;
	int const rgbSliderWidth = waveDisplayWidth;
	int const sliderHeight = waveDisplayHeight;


	//RGBA & Detune Sliders
	juce::Rectangle<int> rgbSliderRect(indentLeft, titleHeight, sliderWidth, sliderHeight);
	swtLevelSlider.setBounds(rgbSliderRect);

	rgbSliderRect.setPosition(indentLeft + sliderWidth * 2, titleHeight);
	sawLevelSlider.setBounds(rgbSliderRect);

	rgbSliderRect.setPosition(indentLeft + sliderWidth * 4, titleHeight);
	sqrLevelSlider.setBounds(rgbSliderRect);


	rgbSliderRect.setPosition(rightSideOfWaveDisplay + sliderWidth, titleHeight);
	targetLevelSlider.setBounds(rgbSliderRect);

	rgbSliderRect.setPosition(rightSideOfWaveDisplay + sliderWidth * 3, titleHeight);
	detuneAmountSlider.setBounds(rgbSliderRect);

	//Phase Radial Sliders
	juce::Rectangle<int> radialSliderRect(indentLeft, titleHeight + sliderHeight, sliderWidth, sliderWidth);
	swtPhaseSlider.setBounds(radialSliderRect);

	radialSliderRect.setPosition(indentLeft + sliderWidth * 2, titleHeight + sliderHeight);
	sawPhaseSlider.setBounds(radialSliderRect);

	radialSliderRect.setPosition(indentLeft + sliderWidth * 4, titleHeight + sliderHeight);
	sqrPhaseSlider.setBounds(radialSliderRect);

	//Wave Display
	waveDisplay.setBounds(leftSideOfWaveDisplay, titleHeight, waveDisplayWidth, waveDisplayHeight);
}
