#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    //Set size of Main Component
    setSize (800, 600);

    //Buttons
    muteBtn.setButtonText("Mute");
    addAndMakeVisible(muteBtn);
    muteBtn.addListener(this);

    combineBtn.setButtonText("Combine");
    addAndMakeVisible(combineBtn);
    combineBtn.addListener(this);

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

    float level = .125;

    for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        float sinWavSample = std::sin(currentAngle);
        float sqrWavSample;
        sinWavSample > 0 ? sqrWavSample = 1 : sqrWavSample = -1;

        float writeSampleVal;
        
        if (combine == false) 
        {
            writeSampleVal = sinWavSample * level;
        } 
        else
        {
            sinWavSample > 1 
                ? writeSampleVal = (sinWavSample + sqrWavSample) / 2 * level 
                : writeSampleVal = (sinWavSample - sqrWavSample) / 2 * level;
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
    muteBtn.setBounds(getWidth() / 4, getHeight() / 4, 70, 20);
    combineBtn.setBounds(getWidth() / 4 * 2, getHeight() / 4, 70, 20);
}

void MainComponent::buttonClicked(juce::Button* btn) {
    if (btn->getButtonText() == "Mute") 
    {
        mute ? mute = false : mute = true;
    }
    else if (btn->getButtonText() == "Combine")
    {
        combine ? combine = false : combine = true;
    }
    

}
