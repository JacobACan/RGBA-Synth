#include "RGBASynth.h"

//==============================================================================
RGBASynth::RGBASynth() : angleDelta(0),
rootFrequency(440),

notesOn(0),
noteSample1(0),
noteSample2(0),
noteSample3(0),
noteSample4(0),
noteNumber1(0),
noteNumber2(0),
noteNumber3(0),
noteNumber4(0),

currentAngle(0),
level(0),

swtLevel(0),
sawLevel(0),
sqrLevel(0),

detuneAmount(1),
extraVoices(3),

waveDisplay(),
keyboardComponent(keyboardState, juce::KeyboardComponentBase::verticalKeyboardFacingLeft)

{

    setSize(958, 564);
    

    backgroundColor = juce::Colour::fromFloatRGBA(0, 0, 0, 0);

    //red slider
    red.setRange(juce::Range<double>(0, 255), 1);
    red.setSliderStyle(juce::Slider::LinearHorizontal);
    red.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));

        swtLevel = red.getValue() / 255;

        waveDisplay.setSwtLevel(swtLevel);
        repaint();
    };

    //green slider
    green.setRange(juce::Range<double>(0, 255), 1);
    green.setSliderStyle(juce::Slider::LinearHorizontal);
    green.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));

        sawLevel = green.getValue() / 255;

        waveDisplay.setSawLevel(sawLevel);
        repaint();
    };

    //blue slider
    blue.setRange(juce::Range<double>(0, 255), 1);
    blue.setSliderStyle(juce::Slider::LinearHorizontal);
    blue.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));

        sqrLevel = blue.getValue() / 255;

        waveDisplay.setSqrLevel(sqrLevel);
        repaint();
    };

    //alpha decibel slider
    alpha.setRange(juce::Range<double>(0, 12), 1);
    alpha.setSliderStyle(juce::Slider::LinearBarVertical);
    alpha.onValueChange = [this]
    {
        backgroundColor = juce::Colour::fromRGBA(red.getValue(), green.getValue(), blue.getValue(), RGBADecibelSlider::getRGBvalue(alpha));
        targetLevel = RGBADecibelSlider::getLevelValue(alpha);

        waveDisplay.setLevel(targetLevel);
        repaint();
    };
    addAndMakeVisible(red);
    addAndMakeVisible(green);
    addAndMakeVisible(blue);

    //Phase Shift =======================================================================================
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

    //Detune =======
    detune.setRange(0, 1, .001);
    detune.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    detune.onValueChange = [this] {
        detuneAmount = detune.getValue();
    };

    addAndMakeVisible(swtPhase);
    addAndMakeVisible(sawPhase);
    addAndMakeVisible(sqrPhase);
    addAndMakeVisible(detune);



   //keyboardState
    keyboardState.addListener(this);

   //keyboardComponent
    addAndMakeVisible(keyboardComponent);

    //WaveDisplay
    addAndMakeVisible(waveDisplay);

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
    keyboardState.removeListener(this);
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
                double writeSampleVal;

                // 4 Notes can be played at a time.
                noteSample1 = getNoteSample(noteNumber1, currentAngle);
                noteSample2 = getNoteSample(noteNumber2, currentAngle);
                noteSample3 = getNoteSample(noteNumber3, currentAngle);
                noteSample4 = getNoteSample(noteNumber4, currentAngle);

                writeSampleVal = (noteSample1 + noteSample2 + noteSample3 + noteSample4);
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
                double writeSampleVal;

                // 4 Notes can be played at a time.
                noteSample1 = getNoteSample(noteNumber1, currentAngle);
                noteSample2 = getNoteSample(noteNumber2, currentAngle);
                noteSample3 = getNoteSample(noteNumber3, currentAngle);
                noteSample4 = getNoteSample(noteNumber4, currentAngle);

                writeSampleVal = (noteSample1 + noteSample2 + noteSample3 + noteSample4);
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

double RGBASynth::getNoteSample(int noteNumber, double angle)
{
    if (noteNumber == 0) return 0;

    int distanceFromA = noteNumber - 48;
    double angleForNote = angle * std::pow<double>(2, (double)distanceFromA / (double)12);

    double bassSinVoiceSample = std::sin(angleForNote / 2);


    double sinWavSample = std::sin(angleForNote);

    double swtWavSample = WaveGen::swt(angleForNote);
    double swtVoiceSample2 = WaveGen::swt(angleForNote / (1 + detuneAmount));
    double swtVoiceSample3 = WaveGen::swt(angleForNote * (1 + detuneAmount));

    double sawWavSample = WaveGen::saw(angleForNote);
    double sawVoiceSample2 = WaveGen::saw(angleForNote / (1 + detuneAmount));
    double sawVoiceSample3 = WaveGen::saw(angleForNote * (1 + detuneAmount));

    double sqrWavSample = WaveGen::sqr(angleForNote);
    double sqrVoiceSample2 = WaveGen::sqr(angleForNote / (1 + detuneAmount));
    double sqrVoiceSample3 = WaveGen::sqr(angleForNote * (1 + detuneAmount));

    double noteSampleVal;

    noteSampleVal = (
            sinWavSample
            + ((swtWavSample + swtVoiceSample2 + swtVoiceSample3) * swtLevel)
            + ((sawWavSample + sawVoiceSample2 + sawVoiceSample3) * sawLevel)
            + ((sqrWavSample + sqrVoiceSample2 + sqrVoiceSample3) * sqrLevel)

            + bassSinVoiceSample
        )
            / ((swtLevel * 3) + (sawLevel * 3) + (sqrLevel * 3) + 1); // Max combined in phase level reached by adding these wave samples.

    return noteSampleVal;
}

void RGBASynth::releaseResources()
{
}

//==============================================================================
void RGBASynth::paint (juce::Graphics& g)
{
    int titleHeight = 40;
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
    DBG("Get Width : " << getWidth());
    DBG("Get Height : " << getHeight());
}

// ===============================================================================
void RGBASynth::updateAngleDelta()
{
    float cyclesPerSample = rootFrequency / currentSampleRate; // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

//================================================================================
void RGBASynth::handleNoteOn(juce::MidiKeyboardState* source,
    int midiChannel, int midiNoteNumber, float velocity) {
    int correctNoteNumber = midiNoteNumber - 33;
    notesOn += 1;
    
    noteNumber1 == 0 ? noteNumber1 = correctNoteNumber
        : noteNumber2 == 0 ? noteNumber2 = correctNoteNumber
        : noteNumber3 == 0 ? noteNumber3 = correctNoteNumber
        : noteNumber4 == 0 ? noteNumber4 = correctNoteNumber
        : correctNoteNumber;

}
void RGBASynth::handleNoteOff(juce::MidiKeyboardState* source,
    int midiChannel, int midiNoteNumber, float velocity) {
    notesOn -= 1;
    
    int correctNoteNumber = midiNoteNumber - 33;

    noteNumber1 == correctNoteNumber ? noteNumber1 = 0
        : noteNumber2 == correctNoteNumber ? noteNumber2 = 0
        : noteNumber3 == correctNoteNumber ? noteNumber3 = 0
        : noteNumber4 == correctNoteNumber ? noteNumber4 = 0
        : correctNoteNumber;



}

//================================================================================