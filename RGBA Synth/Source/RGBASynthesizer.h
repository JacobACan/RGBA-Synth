/*
  ==============================================================================

    RGBASynthesizer.h
    Created: 16 Jan 2023 12:38:55pm
    Author:  Jacob

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RGBASynthSounds.h"

class RGBASynthesizer : public juce::Synthesiser
{
public:

    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) override;
    void renderVoices(juce::AudioBuffer<double>& outPutBuffer, int startSample, int numSamples) override;


private:

    //Sounds this synth can make
    

};