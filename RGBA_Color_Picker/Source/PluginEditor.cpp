/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    setResizable(true, true);

    //Set Initial Background Color
    backgroundColor = juce::Colour::fromRGB(0, 0, 0);
    
    //red slider
    red.setRange(juce::Range<double>(0, 255), 1);
    red.setSliderStyle(juce::Slider::LinearBarVertical);
    red.addListener(this);

    //green slider
    green.setRange(juce::Range<double>(0, 255), 1);
    green.setSliderStyle(juce::Slider::LinearBarVertical);
    green.addListener(this);

    //blue slider
    blue.setRange(juce::Range<double>(0, 255), 1);
    blue.setSliderStyle(juce::Slider::LinearBarVertical);
    blue.addListener(this);

    //blue slider
    alpha.setRange(juce::Range<double>(0, 255), 1);
    alpha.setSliderStyle(juce::Slider::LinearBarVertical);
    alpha.addListener(this);


    addAndMakeVisible(red);
    addAndMakeVisible(green);
    addAndMakeVisible(blue);
    addAndMakeVisible(alpha);

}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (backgroundColor);

    g.setColour (juce::Colours::white);
    g.setFont(juce::Font("Rubik Vinyl Regular", 60.f, juce::Font::plain));
    g.drawFittedText ("Audio Vision", getLocalBounds(), juce::Justification::centredTop, 1);

}

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), alpha.getValue());
    repaint();
}

void NewProjectAudioProcessorEditor::resized()
{
    int fourthHeight = getHeight() / 4;
    int width = 30;
    int offset = getWidth() / 12 - width/2;
    juce::Rectangle<int> sliderRect(getWidth() / 6 + offset, fourthHeight, width, getHeight()/2);
    red.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 2 + offset, fourthHeight);
    green.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 3 + offset, fourthHeight);
    blue.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 4 + offset, fourthHeight);
    alpha.setBounds(sliderRect);
}
