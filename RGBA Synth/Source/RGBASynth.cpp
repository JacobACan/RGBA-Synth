#include "RGBASynth.h"

//==============================================================================
RGBASynth::RGBASynth() : angleDelta(0),
currentAngle(0),
frequency(1),
level(0),
notesOn(0),
keyboardComponent(keyboardState, juce::KeyboardComponentBase::verticalKeyboardFacingLeft),
maxWaveHeight (1),
swtLevel(0),
sawLevel(0),
sqrLevel (0),
waveDisplay()
{

    setSize(735, 410);
    

    backgroundColor = juce::Colour::fromFloatRGBA(0, 0, 0, 0);

    //red slider
    red.setRange(juce::Range<double>(0, 255), 1);
    red.setSliderStyle(juce::Slider::LinearHorizontal);
    red.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        repaint();

        swtLevel = red.getValue() / 255;
        maxWaveHeight = swtLevel + sawLevel + sqrLevel + 1;

        waveDisplay.setMaxWaveHeight(maxWaveHeight);
        waveDisplay.setSwtLevel(swtLevel);
    };

    //green slider
    green.setRange(juce::Range<double>(0, 255), 1);
    green.setSliderStyle(juce::Slider::LinearHorizontal);
    green.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        repaint();

        sawLevel = green.getValue() / 255;
        maxWaveHeight = swtLevel + sawLevel + sqrLevel + 1;

        waveDisplay.setMaxWaveHeight(maxWaveHeight);
        waveDisplay.setSawLevel(sawLevel);
    };

    //blue slider
    blue.setRange(juce::Range<double>(0, 255), 1);
    blue.setSliderStyle(juce::Slider::LinearHorizontal);
    blue.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        repaint();

        sqrLevel = blue.getValue() / 255;
        maxWaveHeight = swtLevel + sawLevel + sqrLevel + 1;

        waveDisplay.setMaxWaveHeight(maxWaveHeight);
        waveDisplay.setSqrLevel(sqrLevel);
    };

    //alpha decibel slider
    alpha.setRange(juce::Range<double>(0, 12), 1);
    alpha.setSliderStyle(juce::Slider::LinearBarVertical);
    alpha.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        targetLevel = RGBADecibelSlider::getLevelValue(alpha);
        repaint();

        waveDisplay.setLevel(targetLevel);
    };

   //keyboardState
    keyboardState.addListener(this);

   //keyboardComponent
    addAndMakeVisible(keyboardComponent);

    //WaveDisplay
    addAndMakeVisible(waveDisplay);

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

RGBASynth::~RGBASynth()
{
    shutdownAudio();
}

//==============================================================================
void RGBASynth::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    updateAngleDelta();
}

void RGBASynth::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto * leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto * rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    if (notesOn > 0) 
    {
        //Create Sound
        if (level != targetLevel)
        {
            // If the buffer size is too small the increment amount will still make artifacts.  
            // Splitting the smoothing between multiple buffers could fix this.
            double levelIncrement = (targetLevel - level) / bufferToFill.numSamples;

            for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
            {
                level += levelIncrement;
                double sinWavSample = std::sin(currentAngle);
                double swtnWavSample = WaveGen::swt(currentAngle);
                double sawWavSample = WaveGen::saw(currentAngle);
                double sqrWavSample = WaveGen::sqr(currentAngle);

                double writeSampleVal;

                writeSampleVal = (sinWavSample + (swtnWavSample * swtLevel) + (sawWavSample * sawLevel) + (sqrWavSample * sqrLevel)) / maxWaveHeight;

                writeSampleVal *= level;

                leftBuffer[sample] = writeSampleVal;
                rightBuffer[sample] = writeSampleVal;
                currentAngle += angleDelta;
            }

            level = targetLevel;
        }
        else
        {
            for (auto sample = 0; sample < bufferToFill.numSamples; sample++)
            {
                double sinWavSample = std::sin(currentAngle);
                double swtnWavSample = WaveGen::swt(currentAngle);
                double sawWavSample = WaveGen::saw(currentAngle);
                double sqrWavSample = WaveGen::sqr(currentAngle);

                double writeSampleVal;

                writeSampleVal = (sinWavSample + (swtnWavSample * swtLevel) + (sawWavSample * sawLevel) + (sqrWavSample * sqrLevel)) / maxWaveHeight;

                writeSampleVal *= level;

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

void RGBASynth::releaseResources()
{
}

//==============================================================================
void RGBASynth::paint (juce::Graphics& g)
{
    int titleHeight = 50;
    int lightThreshold = 210;

    g.fillAll (backgroundColor);
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

void RGBASynth::resized()
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
    blue.setBounds(rgbSliderRect);

    rgbSliderRect.setPosition(leftSideOfWaveDisplay, bottomOfWaveDisplay += rgbSliderHieght);
    green.setBounds(rgbSliderRect);

    rgbSliderRect.setPosition(leftSideOfWaveDisplay, bottomOfWaveDisplay += rgbSliderHieght);
    red.setBounds(rgbSliderRect);


    juce::Rectangle<int> alphaSliderRect(rightSideOfWaveDisplay -= volumeSliderWidth, titleHeight, volumeSliderWidth, waveDisplayHeight);
    alpha.setBounds(alphaSliderRect);


    //Keyboard Component
    keyboardComponent.setBounds(leftSideOfWaveDisplay - keyboardWidth, titleHeight, keyboardWidth, waveDisplayHeight);


    waveDisplay.setBounds(leftSideOfWaveDisplay, titleHeight, waveDisplayWidth, waveDisplayHeight);
}

// ===============================================================================
void RGBASynth::updateAngleDelta()
{
    float cyclesPerSample = frequency / currentSampleRate; // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

//================================================================================
void RGBASynth::handleNoteOn(juce::MidiKeyboardState* source,
    int midiChannel, int midiNoteNumber, float velocity) {
    
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    updateAngleDelta();
    notesOn += 1;
}
void RGBASynth::handleNoteOff(juce::MidiKeyboardState* source,
    int midiChannel, int midiNoteNumber, float velocity) {
    
    notesOn -= 1;

}

//================================================================================
