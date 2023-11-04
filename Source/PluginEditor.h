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

    juce::Slider inGain{ "In Gain" },
        oversampling{ "Oversampling" },
        outGain{ "Out Gain" },
        clipCeiling{ "Clip Ceiling" },
        clipType{ "Clip Type" };

    juce::ToggleButton bypass{ "Bypass" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KClipperAudioProcessorEditor)
};
