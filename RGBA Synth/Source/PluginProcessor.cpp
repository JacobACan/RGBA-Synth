

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
    apvts(*this, nullptr, "Parameters",createParameterLayout()),
    numVoices(8)
{
    for (int i = 0; i < numVoices; i++)
    {
        RGBASynth.addVoice(new RGBAVoice());

    }
    RGBASynth.addSound(new RGBASound());
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
    RGBASynth.setCurrentPlaybackSampleRate(sampleRate);
    RGBASynth.updateVoiceParameters(apvts);

    midiCollector.reset(sampleRate);
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
    buffer.clear();
    midiCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    RGBASynth.updateVoiceParameters(apvts);
    RGBASynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


    midiMessages.clear();
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
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

// Setters =====================================================================
void PluginRGBASynthProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout 
PluginRGBASynthProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("targetLevel", "targetLevel", juce::Range<float>(0, 1), .125));
    layout.add(std::make_unique<juce::AudioParameterFloat>("swtLevel", "swtLevel", juce::Range<float>(0, 1), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("sqrLevel", "sqrLevel", juce::Range<float>(0, 1), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("sawLevel", "sawLevel", juce::Range<float>(0, 1), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("detuneAmount", "detuneAmount", juce::Range<float>(0, 1), 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>("swtPhase", "swtPhase", juce::Range<float>(0, 1), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("sawPhase", "sawPhase", juce::Range<float>(0, 1), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("sqrPhase", "sqrPhase", juce::Range<float>(0, 1), 0));

    return layout;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ 
    return new PluginRGBASynthProcessor();
}