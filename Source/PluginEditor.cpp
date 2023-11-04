/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KClipperAudioProcessorEditor::KClipperAudioProcessorEditor (KClipperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&lnf);

    addAndMakeVisible(meter[0]);
    addAndMakeVisible(meter[1]);
    addAndMakeVisible(outMeter[0]);
    addAndMakeVisible(outMeter[1]);

    setRotarySlider(inGain);
    setRotarySlider(outGain);
    setHorzSlider(clipCeiling);
    setRotarySlider(oversampling);
    setRotarySlider(clipType);

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

    auto center = bounds.reduced(bounds.getWidth() * .3, bounds.getHeight() * .2);
    center.translate(0, -25);
    auto centerHold = center;
    //g.drawRect(center);
    //g.drawRect(noMeter);

    g.setColour(juce::Colour(64u, 194u, 230u));
    auto info = bounds.removeFromTop(bounds.getHeight() * .09);
    g.drawLine(info.getBottomLeft().getX(), info.getBottomLeft().getY(), info.getBottomRight().getX(), info.getBottomRight().getY(), .5); //Not sure why this is not the same color. 
    info.removeFromLeft(info.getWidth() * .01);
    auto logoSpace = info.removeFromLeft(info.getWidth() * .3);
    auto textLeft = logoSpace.removeFromLeft(logoSpace.getWidth() * .25);
    auto textRight = logoSpace.removeFromRight(logoSpace.getWidth() * .66);
    info.removeFromLeft(info.getWidth() * .12);

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

    auto center = bounds.reduced(bounds.getWidth() * .3, bounds.getHeight() * .2);
    center.translate(0, -15);
    clipType.setBounds(center);

    auto bottom = bounds.removeFromBottom(bounds.getHeight() * .2);
    auto botLeft = bottom.removeFromLeft(bottom.getWidth() * .25);
    auto botRight = bottom.removeFromRight(bottom.getWidth() * .33);
    inGain.setBounds(botLeft);
    outGain.setBounds(botRight);
    oversampling.setBounds(bottom);

    auto info = bounds.removeFromTop(bounds.getHeight() * .125);
    auto bypassArea = info.removeFromRight(info.getWidth() * .12);
    bypass.setBounds(bypassArea);

    auto horzArea = bounds.removeFromBottom(bounds.getHeight() * .1);
    horzArea.removeFromLeft(horzArea.getWidth() * .2);
    horzArea.removeFromRight(horzArea.getWidth() * .25);
    clipCeiling.setBounds(horzArea);

}

void KClipperAudioProcessorEditor::setRotarySlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
    slider.setComponentID("Filter");
    addAndMakeVisible(slider);
}

void KClipperAudioProcessorEditor::setHorzSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
    //slider.setComponentID("Filter");
    addAndMakeVisible(slider);
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
