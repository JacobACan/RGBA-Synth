
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginRGBASynthProcessorEditor::PluginRGBASynthProcessorEditor(PluginRGBASynthProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    waveDisplay(),
    keyboardComponent(audioProcessor.keyboardState, juce::KeyboardComponentBase::verticalKeyboardFacingLeft)
{
    // TODO : Initialize Sliders with apvts values.
    setSize(958, 564);


    backgroundColor = juce::Colour::fromFloatRGBA(0, 0, 0, 0);

    //red slider
    red.setRange(juce::Range<double>(0, 255), 1);
    red.setSliderStyle(juce::Slider::LinearHorizontal);
    red.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));

        double swtLevel = red.getValue() / 255;

        audioProcessor.apvts.getRawParameterValue("swtLevel")->store(swtLevel);
        
        waveDisplay.setSwtLevel(swtLevel);
        repaint();
    };

    //green slider
    green.setRange(juce::Range<double>(0, 255), 1);
    green.setSliderStyle(juce::Slider::LinearHorizontal);
    green.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));

        double sawLevel = green.getValue() / 255;

        audioProcessor.apvts.getRawParameterValue("sawLevel")->store(sawLevel);

        waveDisplay.setSawLevel(sawLevel);
        repaint();
    };

    //blue slider
    blue.setRange(juce::Range<double>(0, 255), 1);
    blue.setSliderStyle(juce::Slider::LinearHorizontal);
    blue.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));

        double sqrLevel = blue.getValue() / 255;

        audioProcessor.apvts.getRawParameterValue("sqrLevel")->store(sqrLevel);

        waveDisplay.setSqrLevel(sqrLevel);
        repaint();
    };

    //alpha decibel slider
    alpha.setRange(juce::Range<double>(0, 12), 1);
    alpha.setSliderStyle(juce::Slider::LinearBarVertical);
    alpha.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        double targetLevel = RGBADecibelSlider::getLevelValue(alpha);

        audioProcessor.apvts.getRawParameterValue("targetLevel")->store(targetLevel);
        
        waveDisplay.setLevel(targetLevel);
        repaint();
    };
    addAndMakeVisible(red);
    addAndMakeVisible(green);
    addAndMakeVisible(blue);

    //Phase Shift 
    swtPhase.setRange(juce::Range<double>(0, juce::MathConstants<double>::twoPi), .01);
    swtPhase.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    swtPhase.onValueChange = [this] {
        WaveGen::setSwtPhase(swtPhase.getValue());
        waveDisplay.repaint();
    };

    sawPhase.setRange(juce::Range<double>(0, juce::MathConstants<double>::twoPi), .01);
    sawPhase.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sawPhase.onValueChange = [this] {
        WaveGen::setSawPhase(sawPhase.getValue());
        waveDisplay.repaint();
    };

    sqrPhase.setRange(juce::Range<double>(0, juce::MathConstants<double>::twoPi), .01);
    sqrPhase.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sqrPhase.onValueChange = [this] {
        WaveGen::setSqrPhase(sqrPhase.getValue());
        waveDisplay.repaint();
    };

    //Detune 
    detune.setRange(0, 1, .001);
    detune.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    detune.onValueChange = [this] {
        double detuneAmount = detune.getValue();

        audioProcessor.apvts.getRawParameterValue("detuneAmount")->store(detuneAmount);
    };

    //keyboardComponent
    addAndMakeVisible(keyboardComponent);

    //WaveDisplay
    addAndMakeVisible(waveDisplay);

    addAndMakeVisible(alpha);
    addAndMakeVisible(swtPhase);
    addAndMakeVisible(sawPhase);
    addAndMakeVisible(sqrPhase);
    addAndMakeVisible(detune);
}

PluginRGBASynthProcessorEditor::~PluginRGBASynthProcessorEditor()
{
}

//==============================================================================
void PluginRGBASynthProcessorEditor::paint(juce::Graphics& g)
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

    (RGBADecibelSlider::getRGBvalue(alpha) > lightThreshold
        && red.getValue() > lightThreshold
        && green.getValue() > lightThreshold &&
        blue.getValue() > lightThreshold)
        ? g.setColour(juce::Colours::black)
        : g.setColour(juce::Colours::white);

    g.drawText("RGBA Synth", 6, 16, getWidth(), getHeight(), juce::Justification::centredTop);
}

void PluginRGBASynthProcessorEditor::resized()
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
    red.setBounds(rgbSliderRect);

    rgbSliderRect.setPosition(leftSideOfWaveDisplay, bottomOfWaveDisplay + rgbSliderHieght);
    green.setBounds(rgbSliderRect);

    rgbSliderRect.setPosition(leftSideOfWaveDisplay, bottomOfWaveDisplay + 2 * rgbSliderHieght);
    blue.setBounds(rgbSliderRect);


    juce::Rectangle<int> alphaSliderRect(rightSideOfWaveDisplay -= volumeSliderWidth, titleHeight, volumeSliderWidth, waveDisplayHeight);
    alpha.setBounds(alphaSliderRect);

    juce::Rectangle<int> phaseRect(keyboardWidth, rgbSliderHieght);

    phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay);
    swtPhase.setBounds(phaseRect);

    phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay + rgbSliderHieght);
    sawPhase.setBounds(phaseRect);

    phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay + 2 * rgbSliderHieght);
    sqrPhase.setBounds(phaseRect);

    phaseRect.setPosition(leftSideOfWaveDisplay - keyboardWidth, bottomOfWaveDisplay + 3 * rgbSliderHieght);
    detune.setBounds(phaseRect);


    //Keyboard Component
    keyboardComponent.setBounds(leftSideOfWaveDisplay - keyboardWidth, titleHeight, keyboardWidth, waveDisplayHeight);


    waveDisplay.setBounds(leftSideOfWaveDisplay, titleHeight, waveDisplayWidth, waveDisplayHeight);
}
