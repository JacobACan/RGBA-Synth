

#pragma once

#include <JuceHeader.h>
#include "RGBASynthesizer.h"
#include "RGBASynthSounds.h"
#include "RGBASynthVoices.h"


//==============================================================================
/**
*/
class PluginRGBASynthProcessor  : public juce::AudioProcessor,
                                  public juce::MidiKeyboardState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    PluginRGBASynthProcessor();
    ~PluginRGBASynthProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================

    //==============================================================================
    void handleNoteOn(juce::MidiKeyboardState* source,
        int midiChannel, int midiNoteNumber, float velocity);

    void handleNoteOff(juce::MidiKeyboardState* source,
        int midiChannel, int midiNoteNumber, float velocity);

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;

    void setStateInformation (const void* data, int sizeInBytes) override;

    // TODO : Move these levels to audio processor value tree state
    void setSwtLevel(double newSwtLevel);
    void setSawLevel(double newSawLevel);
    void setSqrLevel(double newSqeLevel);
    void setTargetLevel(double newTargetLevel);
    void setDetuneAmount(double newDetuneLevel);

    juce::MidiKeyboardState keyboardState;

private:
    //==============================================================================

    //Functions
    void updateAngleDelta();
    double getNoteSample(int noteNumber, double angle);

    //Noise Generation Variables


    //TODO : Move these variables to an audio processor value tree state
    double currentSampleRate;

    double currentAngle;
    float angleDelta;

    double level;
    double targetLevel;

    double rootFrequency;
    int notesOn;
    int noteNumber1;
    int noteNumber2;
    int noteNumber3;
    int noteNumber4;
    double noteSample1;
    double noteSample2;
    double noteSample3;
    double noteSample4;

    double swtLevel;
    double sawLevel;
    double sqrLevel;

    double detuneAmount;
    int extraVoices;

    RGBASynthesizer RGBASynth;

    juce::MidiMessageCollector midiCollector;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginRGBASynthProcessor)
};
