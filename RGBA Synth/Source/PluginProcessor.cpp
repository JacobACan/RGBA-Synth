/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RGBASynthAudioProcessor::RGBASynthAudioProcessor()
    : parameters(*this, nullptr, juce::Identifier("APVTS"), createParameterLayout()),
    numVoices(8),
#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for (int i = 0; i < numVoices; i++)
    {
        RGBASynth.addVoice(new RGBAVoice());
    }
    RGBASynth.addSound(new RGBASound());
}

juce::AudioProcessorValueTreeState::ParameterLayout 
RGBASynthAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    params.add(std::make_unique<juce::AudioParameterFloat>("targetLevel", "targetLevel", juce::Range<float>(0.f, 1.f), 0.125f));
    params.add(std::make_unique<juce::AudioParameterFloat>("swtLevel", "swtLevel", juce::Range<float>(0.f, 1.f), 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("sawLevel", "sawLevel", juce::Range<float>(0.f, 1.f), 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("sqrLevel", "sqrLevel", juce::Range<float>(0.f, 1.f), 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("detuneAmount", "detuneAmount", juce::Range<float>(0.f, 1.f), 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("swtPhase", "swtPhase", juce::Range<float>(0.f, juce::MathConstants<double>::twoPi), 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("sawPhase", "sawPhase", juce::Range<float>(0.f, juce::MathConstants<double>::twoPi), 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("sqrPhase", "sqrPhase", juce::Range<float>(0.f, juce::MathConstants<double>::twoPi), 0));

    return params;
}

RGBASynthAudioProcessor::~RGBASynthAudioProcessor()
{
}

//==============================================================================
const juce::String RGBASynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RGBASynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RGBASynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RGBASynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RGBASynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RGBASynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RGBASynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RGBASynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RGBASynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void RGBASynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RGBASynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    RGBASynth.setCurrentPlaybackSampleRate(sampleRate);
    RGBASynth.updateVoiceParameters(parameters);
    midiCollector.reset(sampleRate);
}

void RGBASynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RGBASynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void RGBASynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    midiCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    RGBASynth.updateVoiceParameters(parameters);

    RGBASynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    midiMessages.clear();
}

//==============================================================================
bool RGBASynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RGBASynthAudioProcessor::createEditor()
{
    return new RGBASynthAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void RGBASynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RGBASynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RGBASynthAudioProcessor();
}
