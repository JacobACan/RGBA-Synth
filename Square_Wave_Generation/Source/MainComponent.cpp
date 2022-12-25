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

    //Set size of Main Component
    setSize (800, 600);

    //Buttons
    muteBtn.setButtonText("Un-Mute");
    addAndMakeVisible(muteBtn);
    muteBtn.addListener(this);

    combineBtn.setButtonText("Combine");
    addAndMakeVisible(combineBtn);
    combineBtn.addListener(this);

    combineAmtSld.setSliderStyle(juce::Slider::LinearBarVertical);
    addAndMakeVisible(combineAmtSld);
    combineAmtSld.addListener(this);
    combineAmtSld.setRange(.01, 1, .01);
    combineAmt = combineAmtSld.getValue();

    sqrPitchSld.setSliderStyle(juce::Slider::LinearBarVertical);
    addAndMakeVisible(sqrPitchSld);
    sqrPitchSld.addListener(this);
    sqrPitchSld.setRange(1, 5, .1);
    sqrWavPitch = sqrPitchSld.getValue();




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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    muteBtn.setBounds(getWidth() / 5, getHeight() / 4, 70, 20);
    combineBtn.setBounds(getWidth() / 5 * 2, getHeight() / 4, 70, 20);
    combineAmtSld.setBounds(getWidth() / 5 * 3, getHeight() / 4, 30, getHeight() / 2);
    sqrPitchSld.setBounds(getWidth() / 5 * 4, getHeight() / 4, 30, getHeight() / 2);
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    combineAmt = combineAmtSld.getValue();
    sqrWavPitch = sqrPitchSld.getValue();
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
