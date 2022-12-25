#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    angleDelta = 440 * juce::MathConstants<double>::twoPi;

    //Buttons
    btn.setButtonText("Mute");
    addAndMakeVisible(btn);
    btn.addListener(this);

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
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    auto cyclesPerSample = frequency / sampleRate; // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi
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
        float writeSampleVal = sinWavSample * level;
        leftBuffer[sample] = writeSampleVal;
        rightBuffer[sample] = writeSampleVal;
        currentAngle += angleDelta;
    }
    
    
    
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    if (mute) bufferToFill.clearActiveBufferRegion();
}

void MainComponent::updateAngleDelta() {
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
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
    btn.setBounds(getWidth() / 4, getHeight() / 4, getWidth() / 2, getHeight() / 2);
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::buttonClicked(juce::Button*) {
    mute ? mute = false : mute = true;
    juce::String msg;
    msg << "Mute : " << (int)mute;
    juce::Logger::getCurrentLogger()->writeToLog(msg);

}
