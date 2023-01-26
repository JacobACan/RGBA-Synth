

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginRGBASynthProcessor::PluginRGBASynthProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif

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
    extraVoices(3)
{
    for (int numVoices = 0; numVoices < 4; numVoices++)
    {
        RGBASynth.addVoice(new RGBASin());
    }
    RGBASynth.addSound(new RGBASound1());

}

PluginRGBASynthProcessor::~PluginRGBASynthProcessor()
{
    
}

//==============================================================================
const juce::String PluginRGBASynthProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginRGBASynthProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginRGBASynthProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginRGBASynthProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginRGBASynthProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginRGBASynthProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int PluginRGBASynthProcessor::getCurrentProgram()
{
    return 0;
}

void PluginRGBASynthProcessor::setCurrentProgram(int index)
{
}

const juce::String PluginRGBASynthProcessor::getProgramName(int index)
{
    return {};
}

void PluginRGBASynthProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void PluginRGBASynthProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    RGBASynth.setCurrentPlaybackSampleRate(samplesPerBlock);
    currentSampleRate = sampleRate;
    midiCollector.reset(sampleRate);
    updateAngleDelta();
}

void PluginRGBASynthProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginRGBASynthProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void PluginRGBASynthProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


   /* auto* leftBuffer = buffer.getWritePointer(0, 0);
    auto* rightBuffer = buffer.getWritePointer(1, 0);*/

    

    midiCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());

    RGBASynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


    //if (notesOn > 0)
    //{
    //    //Create Sound
    //    if (level != targetLevel)
    //    {
    //        // If the buffer size is too small the increment amount will still make artifacts.  
    //        // Splitting the smoothing between multiple buffers could fix this.
    //        double levelIncrement = (targetLevel - level) / buffer.getNumSamples();

    //        for (auto sample = 0; sample < buffer.getNumSamples(); sample++)
    //        {
    //            level += levelIncrement;
    //            double writeSampleVal;

    //            // 4 Notes can be played at a time.
    //            noteSample1 = getNoteSample(noteNumber1, currentAngle);
    //            noteSample2 = getNoteSample(noteNumber2, currentAngle);
    //            noteSample3 = getNoteSample(noteNumber3, currentAngle);
    //            noteSample4 = getNoteSample(noteNumber4, currentAngle);

    //            

    //            writeSampleVal = (noteSample1 + noteSample2 + noteSample3 + noteSample4);
    //            writeSampleVal *= level;

    //            leftBuffer[sample] = writeSampleVal;
    //            rightBuffer[sample] = writeSampleVal;
    //            currentAngle += angleDelta;
    //        }

    //        level = targetLevel;
    //    }
    //    else
    //    {
    //        for (auto sample = 0; sample < buffer.getNumSamples(); sample++)
    //        {
    //            double writeSampleVal;

    //            // 4 Notes can be played at a time.
    //            noteSample1 = getNoteSample(noteNumber1, currentAngle);
    //            noteSample2 = getNoteSample(noteNumber2, currentAngle);
    //            noteSample3 = getNoteSample(noteNumber3, currentAngle);
    //            noteSample4 = getNoteSample(noteNumber4, currentAngle);

    //            

    //            writeSampleVal = (noteSample1 + noteSample2 + noteSample3 + noteSample4);
    //            writeSampleVal *= level;

    //            leftBuffer[sample] = writeSampleVal;
    //            rightBuffer[sample] = writeSampleVal;
    //            currentAngle += angleDelta;
    //            
    //        }
    //    }
    //}
    //else
    //{
    //    buffer.clear();
    //}
}

double PluginRGBASynthProcessor::getNoteSample(int noteNumber, double angle)
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

void PluginRGBASynthProcessor::updateAngleDelta()
{
    float cyclesPerSample = rootFrequency / currentSampleRate; // amount of wave in-between each sample
    angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi; // amount of wave in-between each sample multiplied by 2 pi (in radians)
}

//==============================================================================
bool PluginRGBASynthProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginRGBASynthProcessor::createEditor()
{
    return new PluginRGBASynthProcessorEditor(*this);
}

// Getters ==============================================================================
void PluginRGBASynthProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

int PluginRGBASynthProcessor::getNote1() { return noteNumber1; }
int PluginRGBASynthProcessor::getNote2() { return noteNumber2; }
int PluginRGBASynthProcessor::getNote3() { return noteNumber3; }
int PluginRGBASynthProcessor::getNote4() { return noteNumber4; }


// Setters =====================================================================
void PluginRGBASynthProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void PluginRGBASynthProcessor::setSwtLevel(double newSwtLevel) { swtLevel = newSwtLevel; }
void PluginRGBASynthProcessor::setSawLevel(double newSawLevel) { sawLevel = newSawLevel; }
void PluginRGBASynthProcessor::setSqrLevel(double newSqrLevel) { sqrLevel = newSqrLevel; }
void PluginRGBASynthProcessor::setTargetLevel(double newTargetLevel) { targetLevel = newTargetLevel; }
void PluginRGBASynthProcessor::setDetuneAmount(double newDetuneAmount) { detuneAmount = newDetuneAmount; }

void PluginRGBASynthProcessor::setNote1(int newNoteNumber) { noteNumber1 = newNoteNumber; }
void PluginRGBASynthProcessor::setNote2(int newNoteNumber) { noteNumber2 = newNoteNumber; }
void PluginRGBASynthProcessor::setNote3(int newNoteNumber) { noteNumber3 = newNoteNumber; }
void PluginRGBASynthProcessor::setNote4(int newNoteNumber) { noteNumber4 = newNoteNumber; }

void PluginRGBASynthProcessor::incrementNotesOn() { notesOn += 1; }
void PluginRGBASynthProcessor::decrementNotesOn() { notesOn -= 1; }

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginRGBASynthProcessor();
}




