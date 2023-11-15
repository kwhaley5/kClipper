/*
  ==============================================================================

    rotarySliderWithLabels.h
    Created: 6 Nov 2023 5:57:57pm
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct RotarySliderWithLabels : juce::Slider {
    RotarySliderWithLabels(juce::RangedAudioParameter* rap, const juce::String& unitSuffix, const juce::String title) :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
        param(rap),
        suffix(unitSuffix)
    {
        setName(title);
    }

    struct LabelPos {
        float pos;
        juce::String label;

    };

    juce::Array<LabelPos> labels;

    void paint(juce::Graphics& g) override;
    juce::Rectangle<int>  getSliderBounds() const;
    int getTextHeight() const { return 14; };

    void changeParam(juce::RangedAudioParameter* p);

protected:
    juce::RangedAudioParameter* param;
    juce::String suffix;

};

template <
    typename Attachment,
    typename APVTS,
    typename ParamName,
    typename SliderType
>
void makeAttachment(std::unique_ptr<Attachment>& attachment,
    APVTS& apvts,
    const ParamName& name,
    SliderType& slider)
{
    attachment = std::make_unique<Attachment>(apvts,
        name,
        slider);
}

template <
    typename APVTS,
    typename Name
>
juce::RangedAudioParameter& getParam(APVTS& apvts, const Name& name)
{
    auto param = apvts.getParameter(name);

    return *param;
}

template <
    typename Labels,
    typename ParamType,
    typename SuffixType
>
void addLabelPairs(Labels& labels, const float posOne, const float posTwo, const ParamType& param, const SuffixType& suffix, std::vector<juce::String> array)
{
    labels.clear();
    labels.add({ posOne, getValString(param, true, suffix, array) });
    labels.add({ posTwo, getValString(param, false, suffix, array) });
}

juce::String getValString(const juce::RangedAudioParameter& param, bool getLow, juce::String suffix, std::vector<juce::String>);