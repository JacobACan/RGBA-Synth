#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    //variable initialization
    angleDelta = 0.f;
    currentAngle = 0.f;
    frequency = 1.f;
    level = 0;


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);

    //Set Initial Background Color
    backgroundColor = juce::Colour::fromFloatRGBA(0, 0, 0, 0);

    //red slider
    red.setRange(juce::Range<double>(0, 255), 1);
    red.setSliderStyle(juce::Slider::LinearBarVertical);
    red.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        repaint();
        if (red.getValue() > 1) 
        {
            frequency = (red.getValue() * 2);
            updateAngleDelta();
        }
    };

    //green slider
    green.setRange(juce::Range<double>(0, 255), 1);
    green.setSliderStyle(juce::Slider::LinearBarVertical);
    green.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        repaint();
    };

    //blue slider
    blue.setRange(juce::Range<double>(0, 255), 1);
    blue.setSliderStyle(juce::Slider::LinearBarVertical);
    blue.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        repaint();
    };

    //alpha decibel slider
    alpha.setRange(juce::Range<double>(0, 12), 1);
    alpha.setSliderStyle(juce::Slider::LinearBarVertical);
    alpha.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        targetLevel = RGBADecibelSlider::getLevelValue(alpha);
        repaint();
    };


    addAndMakeVisible(red);
    addAndMakeVisible(green);
    addAndMakeVisible(blue);
    addAndMakeVisible(alpha);




    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    updateAngleDelta();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto * leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto * rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);


    if (level != targetLevel || frequency != targetFrequency)
    {
        // If the buffer size is too small the increment amount will still make artifacts.  
        // Splitting the smoothing between multiple buffers could fix this.
        double levelIncrement = (targetLevel - level) / bufferToFill.numSamples;
        double frequencyIncrement = (targetFrequency - frequency) / bufferToFill.numSamples;

        for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
        {
            level += levelIncrement;
            frequency += frequencyIncrement;

            double sinWavSample = (double) std::sin(currentAngle);

            double writeSampleVal = sinWavSample * level;

            leftBuffer[sample] = writeSampleVal;
            rightBuffer[sample] = writeSampleVal;
            currentAngle += angleDelta;
        }

        level = targetLevel;
        frequency = targetFrequency;
    }
    else
    {
        for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
        {
            double sinWavSample = std::sin(currentAngle);

            double writeSampleVal;
            writeSampleVal = sinWavSample * level;

            leftBuffer[sample] = writeSampleVal;
            rightBuffer[sample] = writeSampleVal;
            currentAngle += angleDelta;
        }
    }

}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    int titleHeight = 50;
    int darkThreshold = 120;
    int lightThreshold = 210;

    g.fillAll (backgroundColor);
    g.setFont(juce::Font("Titillium Web", titleHeight, juce::Font::plain));

        (RGBADecibelSlider::getRGBvalue(alpha) > lightThreshold
            && red.getValue() > lightThreshold
            && green.getValue() > lightThreshold &&
            blue.getValue() > lightThreshold)
        ? g.setColour(juce::Colours::black) 
        : RGBADecibelSlider::getRGBvalue(alpha) > darkThreshold
        ? g.setColour(juce::Colours::white)
        : g.setColour(juce::Colours::black);

    g.drawText("RGBA Synth", 0, 0, getWidth(), getHeight(), juce::Justification::centredTop);
}

void MainComponent::resized()
{
    int fourthHeight = getHeight() / 4;
    int width = 50;
    int offset = getWidth() / 12 - width / 2;
    juce::Rectangle<int> sliderRect(getWidth() / 6 + offset, fourthHeight, width, getHeight() / 2);
    red.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 2 + offset, fourthHeight);
    green.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 3 + offset, fourthHeight);
    blue.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 4 + offset, fourthHeight);
    alpha.setBounds(sliderRect);
}

void MainComponent::updateAngleDelta() 
{
    float cyclesPerSample = frequency / currentSampleRate; // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}
