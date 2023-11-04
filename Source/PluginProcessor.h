/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/clipper.h"
#include "DSP/kLevelMeterData.h"

//==============================================================================
/**
*/
class KClipperAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    KClipperAudioProcessor();
    ~KClipperAudioProcessor() override;

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
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "parameters", createParameterLayout() };

    LevelMeterData levelMeterData;


private:

    Clipper clipper;

    std::array<juce::dsp::Oversampling<float>, 4> overSamplers
    { {
        {2, 0, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR},
        {2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR},
        {2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR},
        {2, 3, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR},
    } };

    juce::dsp::Gain<float> inGain;
    juce::dsp::Gain<float> outGain;

    juce::AudioParameterBool* bypass{ nullptr };
    juce::AudioParameterInt* clipSelect{ nullptr };
    juce::AudioParameterInt* oversampleSelect{ nullptr };
    juce::AudioParameterFloat* threshold{ nullptr };
    juce::AudioParameterFloat* inValue{ nullptr };
    juce::AudioParameterFloat* outValue{ nullptr };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KClipperAudioProcessor)
};
