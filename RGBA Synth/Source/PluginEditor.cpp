/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RGBASynthAudioProcessorEditor::RGBASynthAudioProcessorEditor (RGBASynthAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor(p),
    editorApvts(apvts),
    waveDisplay(apvts),
    keyboardComponent(p.keyboardState, juce::KeyboardComponentBase::verticalKeyboardFacingLeft)

{
    setSize(958, 564);
    backgroundColor = juce::Colour::fromFloatRGBA(0, 0, 0, 0);

    addAndMakeVisible(waveDisplay);
    addAndMakeVisible(keyboardComponent);

    // Alpha ==============================================
    targetLevelSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    addAndMakeVisible(targetLevelSlider);
    targetLevelSlider.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
        repaint();
    };
    targetLevelAttatchment.reset(new SliderAttachment(editorApvts, "targetLevel", targetLevelSlider));

    // Red =================================================
    swtLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(swtLevelSlider);
    swtLevelSlider.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
        repaint();
    };
    swtLevelAttatchment.reset(new SliderAttachment(editorApvts, "swtLevel", swtLevelSlider));

    // Green =================================================
    sawLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(sawLevelSlider);
    sawLevelSlider.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
        repaint();
    };
    sawLevelAttatchment.reset(new SliderAttachment(editorApvts, "sawLevel", sawLevelSlider));

    // Blue =================================================
    sqrLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(sqrLevelSlider);
    sqrLevelSlider.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(swtLevelSlider.getValue() * 255, sawLevelSlider.getValue() * 255, sqrLevelSlider.getValue() * 255, targetLevelSlider.getValue() * 255);
        repaint();
    };
    sqrLevelAttatchment.reset(new SliderAttachment(editorApvts, "sqrLevel", sqrLevelSlider));

    // Detune ================================================
    detuneAmountSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    addAndMakeVisible(detuneAmountSlider);
    detuneAmountAttatchment.reset(new SliderAttachment(editorApvts, "detuneAmount", detuneAmountSlider));
    
    //Phase
    swtPhaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    addAndMakeVisible(swtPhaseSlider);
    swtPhaseSlider.onValueChange = [this] {
        WaveGen::setSwtPhase(swtPhaseSlider.getValue());
        waveDisplay.repaint();
    };
    swtPhaseAttatchment.reset(new SliderAttachment(editorApvts, "swtPhase", swtPhaseSlider));

    sawPhaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    addAndMakeVisible(sawPhaseSlider);
    sawPhaseSlider.onValueChange = [this] {
        WaveGen::setSawPhase(sawPhaseSlider.getValue());
        waveDisplay.repaint();
    };
    sawPhaseAttatchment.reset(new SliderAttachment(editorApvts, "sawPhase", sawPhaseSlider));

    sqrPhaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
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
void RGBASynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    int titleHeight = 40;
    int lightThreshold = 210;

    g.fillAll(backgroundColor);
    g.setFont(juce::Font("Press Start 2P", titleHeight, juce::Font::plain));

    g.setColour(juce::Colours::red);
    g.drawText("RGBA Synth", 0, 10, getWidth(), getHeight(), juce::Justification::centredTop);
    g.setColour(juce::Colours::green);
    g.drawText("RGBA Synth", 2, 12, getWidth(), getHeight(), juce::Justification::centredTop);
    g.setColour(juce::Colours::blue);
    g.drawText("RGBA Synth", 4, 14, getWidth(), getHeight(), juce::Justification::centredTop);

    g.drawText("RGBA Synth", 6, 16, getWidth(), getHeight(), juce::Justification::centredTop);

    (targetLevelSlider.getValue() *255 > lightThreshold
        && swtLevelSlider.getValue() > lightThreshold
        && sawLevelSlider.getValue() > lightThreshold &&
        sqrLevelSlider.getValue() > lightThreshold)
        ? g.setColour(juce::Colours::black)
        : g.setColour(juce::Colours::white);

    g.drawText("RGBA Synth", 6, 16, getWidth(), getHeight(), juce::Justification::centredTop);
}

void RGBASynthAudioProcessorEditor::resized()
{
    int titleHeight = 80;

    int waveDisplayWidth = getWidth() * .8;
    int keyboardWidth = waveDisplayWidth * .1;

    int leftSideOfWaveDisplay = getWidth() * .1 + keyboardWidth / 2;
    int rightSideOfWaveDisplay = leftSideOfWaveDisplay + waveDisplayWidth;
    int waveDisplayHeight = getHeight() * .5;
    int bottomOfWaveDisplay = titleHeight + waveDisplayHeight;

    int volumeSliderWidth = waveDisplayWidth * .05;
    int rgbSliderWidth = waveDisplayWidth;
    int rgbSliderHieght = 40;


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
