/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// TODO
// Fix weird gain boost for each algo

//==============================================================================
KClipperAudioProcessorEditor::KClipperAudioProcessorEditor (KClipperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    clipCelingAT(audioProcessor.apvts, "threshold", clipCeiling), bypassAT(audioProcessor.apvts, "bypass", bypass)
{
    setLookAndFeel(&lnf);

    addAndMakeVisible(meter[0]);
    addAndMakeVisible(meter[1]);
    addAndMakeVisible(outMeter[0]);
    addAndMakeVisible(outMeter[1]);

    attachRSWL();

    addAndMakeVisible(*inGain);
    addAndMakeVisible(*oversampling);
    addAndMakeVisible(*clipType);
    addAndMakeVisible(*outGain);

    setVertSlider(clipCeiling);
    addAndMakeVisible(bypass);

    setSize(600, 300);
}

KClipperAudioProcessorEditor::~KClipperAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void KClipperAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);

    auto bounds = getLocalBounds();

    auto inputMeter = bounds.removeFromLeft(bounds.getWidth() * .05);
    auto meterLSide = inputMeter.removeFromLeft(inputMeter.getWidth() * .5);

    auto outputMeter = bounds.removeFromRight(bounds.getWidth() * .053);
    auto outMeterLSide = outputMeter.removeFromLeft(outputMeter.getWidth() * .5);

    auto noMeter = bounds;

    bounds = getLocalBounds();

    auto center = bounds.reduced(bounds.getWidth() * .1, bounds.getHeight() * .15);
    //g.drawRect(center);
    center.translate(0, -15);
    auto centerHold = center;

    g.setColour(juce::Colour(64u, 194u, 230u));
    auto info = bounds.removeFromTop(bounds.getHeight() * .09);
    g.drawLine(info.getBottomLeft().getX(), info.getBottomLeft().getY(), info.getBottomRight().getX(), info.getBottomRight().getY(), .5); //Not sure why this is not the same color. 
    info.removeFromLeft(info.getWidth() * .01);
    auto logoSpace = info.removeFromLeft(info.getWidth() * .3);
    auto textLeft = logoSpace.removeFromLeft(logoSpace.getWidth() * .25);
    auto textRight = logoSpace.removeFromRight(logoSpace.getWidth() * .66);
    info.removeFromLeft(info.getWidth() * .12);

    auto bottom = bounds.removeFromBottom(bounds.getHeight() * .2);
    bottom.translate(0, 12);
    auto botLeft = bottom.removeFromLeft(bottom.getWidth() * .25);
    botLeft.translate(-15, 0);
    auto botRight = bottom.removeFromRight(bottom.getWidth() * .33);
    botRight.translate(15, 0);

    auto osArea = bottom;
    bottom.reduce(bottom.getWidth() * .3, 0);

    auto horzArea = bounds.removeFromRight(bounds.getWidth() * .2);

    //g.drawRect(horzArea);

    logo = juce::ImageCache::getFromMemory(BinaryData::KITIK_LOGO_NO_BKGD_png, BinaryData::KITIK_LOGO_NO_BKGD_pngSize);
    g.drawImage(logo, logoSpace.toFloat(), juce::RectanglePlacement::fillDestination);

    auto newFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::OFFSHORE_TTF, BinaryData::OFFSHORE_TTFSize));
    g.setColour(juce::Colours::whitesmoke);
    g.setFont(newFont);
    g.setFont(25.f);
    g.drawFittedText("KiTiK", textLeft, juce::Justification::centredRight, 1);
    g.drawFittedText("Clipper", textRight, juce::Justification::centredLeft, 1);

    g.setFont(juce::Font::Font());
    g.drawFittedText("kwaudioproduction.com", info, juce::Justification::centredLeft, 1);
}

void KClipperAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    auto inputMeter = bounds.removeFromLeft(bounds.getWidth() * .05);
    auto meterLSide = inputMeter.removeFromLeft(inputMeter.getWidth() * .5);
    meter[0].setBounds(meterLSide);
    meter[1].setBounds(inputMeter);

    auto outputMeter = bounds.removeFromRight(bounds.getWidth() * .053);
    auto outMeterLSide = outputMeter.removeFromLeft(outputMeter.getWidth() * .5);
    outMeter[0].setBounds(outMeterLSide);
    outMeter[1].setBounds(outputMeter);

    bounds = getLocalBounds();

    auto center = bounds.reduced(bounds.getWidth() * .01, bounds.getHeight() * .15);
    clipType->setBounds(center);

    auto bottom = bounds.removeFromBottom(bounds.getHeight() * .2);
    bottom.translate(0, 12);
    auto botLeft = bottom.removeFromLeft(bottom.getWidth() * .25);
    botLeft.translate(-15, 0);
    auto botRight = bottom.removeFromRight(bottom.getWidth() * .33);
    botRight.translate(15, 0);

    auto osArea = bottom;
    bottom.reduce(bottom.getWidth() * .15, 0);
    inGain->setBounds(botLeft);
    outGain->setBounds(botRight);
    oversampling->setBounds(bottom);

    auto info = bounds.removeFromTop(bounds.getHeight() * .125);
    auto bypassArea = info.removeFromRight(info.getWidth() * .12);
    bypass.setBounds(bypassArea);

    auto vertArea = bounds.removeFromRight(bounds.getWidth() * .3);
    vertArea.removeFromTop(vertArea.getHeight() * .1);
    vertArea.removeFromBottom(vertArea.getHeight() * .11);
    clipCeiling.setBounds(vertArea);

}

void KClipperAudioProcessorEditor::setRotarySlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
    slider.setComponentID("Filter");
    addAndMakeVisible(slider);
}

void KClipperAudioProcessorEditor::setVertSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
    slider.setComponentID("Filter");
    addAndMakeVisible(slider);
}

void KClipperAudioProcessorEditor::attachRSWL()
{
    auto& apvts = audioProcessor.apvts;
    empty.clear();

    auto& gainInParam = getParam(apvts, "inValue");
    auto& oversamplingParam = getParam(apvts, "oversampleSelect");
    auto& clipTypeParam = getParam(apvts, "clipSelect");
    auto& gainOutParam = getParam(apvts, "outValue");

    inGain = std::make_unique<RotarySliderWithLabels>(&gainInParam, "dB", "In Gain");
    oversampling = std::make_unique<RotarySliderWithLabels>(&oversamplingParam, "dB", "OverSampling");
    clipType = std::make_unique<RotarySliderWithLabels>(&clipTypeParam, "dB", "Clipping Type");
    outGain = std::make_unique<RotarySliderWithLabels>(&gainOutParam, "dB", "Out Gain");

    makeAttachment(inGainAT, apvts, "inValue", *inGain);
    makeAttachment(oversamplingAT, apvts, "oversampleSelect", *oversampling);
    makeAttachment(clipTypeAT, apvts, "clipSelect", *clipType);
    makeAttachment(outGainAT, apvts, "outValue", *outGain);

    addLabelPairs(inGain->labels, 1, 2, gainInParam, " dB", empty);
    addLabelPairs(oversampling->labels, 4, 2, oversamplingParam, "", oversamplingText);
    addLabelPairs(clipType->labels, 1, 3, clipTypeParam, "", clipText);
    addLabelPairs(outGain->labels, 1, 4, gainOutParam, " dB", empty);

    inGain.get()->onValueChange = [this, &gainInParam]()
        {
            addLabelPairs(inGain->labels, 1, 2, gainInParam, " dB", empty);
        };
    oversampling.get()->onValueChange = [this, &oversamplingParam]()
        {
            addLabelPairs(oversampling->labels, 4, 2, oversamplingParam, "", oversamplingText);
        };
    clipType.get()->onValueChange = [this, &clipTypeParam]()
        {
            addLabelPairs(clipType->labels, 1, 3, clipTypeParam, "", clipText);
        };
    outGain.get()->onValueChange = [this, &gainOutParam]()
        {
            addLabelPairs(outGain->labels, 1, 4, gainOutParam, " dB", empty);
        };
}

void KClipperAudioProcessorEditor::timerCallback()
{
    for (auto channel = 0; channel < audioProcessor.getTotalNumInputChannels(); channel++) {
        meter[channel].setLevel(audioProcessor.levelMeterData.getRMS(channel));
        meter[channel].repaint();

        outMeter[channel].setLevel(audioProcessor.levelMeterData.getOutRMS(channel));
        outMeter[channel].repaint();
    }
}
