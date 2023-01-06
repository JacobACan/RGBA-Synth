#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : angleDelta(0),
currentAngle(0),
frequency(1),
level(0),
noteOn(false),
keyboardComponent(keyboardState, juce::KeyboardComponentBase::horizontalKeyboard)
{

    setSize(800, 600);

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

   //keyboardState
    keyboardState.addListener(this);

   //keyboardComponent
    addAndMakeVisible(keyboardComponent);


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

    if (noteOn) 
    {
        //Create Sound
        if (level != targetLevel || frequency != targetFrequency)
        {
            // If the buffer size is too small the increment amount will still make artifacts.  
            // Splitting the smoothing between multiple buffers could fix this.
            double levelIncrement = (targetLevel - level) / bufferToFill.numSamples;

            for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
            {
                level += levelIncrement;

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
    else 
    {
        bufferToFill.clearActiveBufferRegion();
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

    //Sliders
    juce::Rectangle<int> sliderRect(getWidth() / 6 + offset, fourthHeight, width, getHeight() / 2);
    red.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 2 + offset, fourthHeight);
    green.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 3 + offset, fourthHeight);
    blue.setBounds(sliderRect);

    sliderRect.setPosition(getWidth() / 6 * 4 + offset, fourthHeight);
    alpha.setBounds(sliderRect);


    //Keyboard Component
    keyboardComponent.setBounds(getWidth() / 6, getHeight() * 3 / 4, getWidth() * 4 / 6, fourthHeight);
}

// ===============================================================================
void MainComponent::updateAngleDelta() 
{
    float cyclesPerSample = frequency / currentSampleRate; // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

//================================================================================
void MainComponent::handleNoteOn(juce::MidiKeyboardState* source,
    int midiChannel, int midiNoteNumber, float velocity) {
    
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    updateAngleDelta();
    noteOn = true;
}
void MainComponent::handleNoteOff(juce::MidiKeyboardState* source,
    int midiChannel, int midiNoteNumber, float velocity) {

    noteOn = false;
}
