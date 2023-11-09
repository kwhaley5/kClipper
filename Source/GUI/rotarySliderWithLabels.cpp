/*
  ==============================================================================

    rotarySliderWithLabels.cpp
    Created: 6 Nov 2023 5:57:57pm
    Author:  kylew

  ==============================================================================
*/

#include "rotarySliderWithLabels.h"

void RotarySliderWithLabels::paint(juce::Graphics& g) {
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    auto bounds = getLocalBounds();
    g.setColour(Colours::white);
    //g.drawFittedText(getName(), bounds.removeFromTop(getTextBoxHeight() - 3), Justification::centredBottom, 1);


    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng,
        endAng,
        *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * .5f;

    g.setColour(Colour(64u, 194u, 230u));
    g.setFont(15);

    auto numChoices = labels.size();

    //labels for rotarty
    for (int i = 0; i < numChoices; i++) {

        auto pos = labels[i].pos;

        auto ang = jmap(pos, 0.f, 5.f, 0.f, 2* MathConstants<float>::pi);

        auto c = center.getPointOnCircumference(radius, ang);

        Rectangle<float> r;
        auto str = labels[i].label;
        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
        r.setCentre(c);
        r.setY(r.getY() + getTextHeight());

        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 1);
    }
}

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const {

    auto bounds = getLocalBounds();

    auto reduceWidth = juce::jmax(bounds.getWidth() * .15, (double)18);
    auto reduceHeight = juce::jmax(bounds.getHeight() * .15, (double)18);
    auto boundsShrink = bounds.reduced(reduceWidth, reduceHeight);

    return boundsShrink;
}


juce::String RotarySliderWithLabels::getDisplayString() const {

    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param))
        return choiceParam->getCurrentChoiceName();

    juce::String str;
    bool addk = false;

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param)) {

        float val = getValue();

        if (val > 999) {
            val /= 1000.f;
            addk = true;
        }

        str = juce::String(val, (addk ? 2 : 0));

    }
    else {

        jassertfalse; //this shouldn't happen!
    }

    if (suffix.isNotEmpty()) {

        str << " ";
        if (addk)
            str << "k";

        str << suffix;
    }

    return str;
}

void RotarySliderWithLabels::changeParam(juce::RangedAudioParameter* p)
{
    param = p;
    repaint();
}

juce::String getValString(const juce::RangedAudioParameter& param, bool isName, juce::String suffix)
{
    juce::String str;

    auto name = param.getName(20);
    auto val = param.getValue();

    if (isName)
    {
        str << name;
    }
    else
    {
        str << val << suffix;
    }

    return str;
}