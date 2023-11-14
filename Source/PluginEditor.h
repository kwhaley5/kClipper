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
    void setVertSlider(juce::Slider&);
    void attachRSWL();

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
        clipType;

    juce::Slider clipCeiling{ "Threshold" };

    juce::ToggleButton bypass{ "Bypass" };

    using Att = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<Att> inGainAT, oversamplingAT, outGainAT, clipTypeAT;
    Att clipCelingAT;

    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAT;

    std::vector<juce::String> oversamplingText{ "1x", "2x", "4x", "8x" };
    std::vector<juce::String> clipText{ "Hard Clip", "Cubic", "Sine", "Tanget", "ArcTangent", "Quintic"};
    std::vector<juce::String> empty;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KClipperAudioProcessorEditor)
};
