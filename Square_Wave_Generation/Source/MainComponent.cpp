#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    //variable initialization
    angleDelta = 0.0;
    currentAngle = 0.0;
    frequency = 500;
    mute = true;
    combine = false;

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);

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
    auto cyclesPerSample = frequency / sampleRate; // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto * leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto * rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    float level = .08;

    for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        float sinWavSample = std::sin(currentAngle);
        float sinWavSample2 = std::sin(sqrWavPitch * currentAngle);

        float sqrWavSample;
        sinWavSample2 > 0 ? sqrWavSample = 1 : sqrWavSample = -1;

        float writeSampleVal;
        
        if (combine == false) 
        {
            writeSampleVal = sinWavSample * level;
        } 
        else
        {
            sinWavSample > 1 
                ? writeSampleVal = (sinWavSample + (sqrWavSample * combineAmt)) / 2 * level
                : writeSampleVal = (sinWavSample - (sqrWavSample * combineAmt)) / 2 * level;
        }

        leftBuffer[sample] = writeSampleVal;
        rightBuffer[sample] = writeSampleVal;
        currentAngle += angleDelta;
    }
    
    if (mute) bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (backgroundColor);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    int fourthHeight = getHeight() / 4;
    int width = 30;
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

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), alpha.getValue());
    repaint();
}

void MainComponent::buttonClicked(juce::Button* btn) {
    if (btn->getButtonText() == "Un-Mute") 
    {
        mute ? mute = false : mute = true;
    }
    else if (btn->getButtonText() == "Combine")
    {
        combine ? combine = false : combine = true;
    }
    

}
