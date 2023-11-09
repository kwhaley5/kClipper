/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/kLevelMeter.h"
#include "GUI/kLookAndFeel.h"
#include "GUI/rotarySliderWithLabels.h"

//==============================================================================
/**
*/
class KClipperAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    KClipperAudioProcessorEditor (KClipperAudioProcessor&);
    ~KClipperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void setRotarySlider(juce::Slider&);
    void setHorzSlider(juce::Slider&);

    void timerCallback() override;

private:
    
    Laf lnf;
    juce::Image logo;
    juce::Font newFont;

    KClipperAudioProcessor& audioProcessor;

    std::array<LevelMeter, 2> meter;
    std::array<LevelMeter, 2> outMeter;

    std::unique_ptr<RotarySliderWithLabels> inGain,
        oversampling,
        outGain,
        clipCeiling,
        clipType;

    juce::ToggleButton bypass{ "Bypass" };

    using Att = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<Att> inGainAT, oversamplingAT, outGainAT, clipCelingAT, clipTypeAT;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KClipperAudioProcessorEditor)
};
