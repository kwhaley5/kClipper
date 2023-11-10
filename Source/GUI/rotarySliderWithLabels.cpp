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

    /*g.setColour(Colours::red);
    g.drawRect(getLocalBounds());
    g.setColour(Colours::yellow);
    g.drawRect(sliderBounds);*/

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
    auto radius = sliderBounds.getHeight() * .5f;

    g.setColour(Colours::white);
    g.setFont(14);

    auto numChoices = labels.size();

    for(int i = 0; i < numChoices; ++i)
    {
        Rectangle<float> r;
        Point<float> c;
        float ang;

        auto pos = labels[i].pos;
        auto str = labels[i].label;
        auto strWidth = g.getCurrentFont().getStringWidth(str);

        //if (strWidth > 90) { strWidth = 90; };
        
        r.setSize(strWidth, getTextHeight());

        if (pos == 1)
        {
            c = center.getPointOnCircumference(radius + getTextHeight() * .75, 0);
            r.setCentre(c);
        }
        else if (pos == 2)
        {
            r.setX(bounds.getWidth() - r.getWidth() - 5);
            r.setY(bounds.getHeight() / 2 - getTextHeight() / 2);
            
        }
        else if (pos == 3)
        {
            c = center.getPointOnCircumference(radius * 1, MathConstants<float>::pi);
            r.setCentre(c);
        }
        else 
        {
            r.setX(bounds.getX() + 5);
            r.setY(bounds.getHeight() / 2 - getTextHeight() / 2);
        }

        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 2, 1);
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
    auto range = param.getNormalisableRange();
    auto normVal = juce::jmap(val, 0.f, 1.f, range.start, range.end);
    normVal = floor(normVal * 100);
    normVal /= 100;

    if (isName)
    {
        str << name;
    }
    else
    {
        str << normVal << suffix;
    }

    return str;
}