/*
  ==============================================================================

    clipper.cpp
    Created: 2 Nov 2023 1:42:41pm
    Author:  kylew

  ==============================================================================
*/

#include "Clipper.h"

void Clipper::process(juce::dsp::AudioBlock<float>& block, int channel)
{
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    auto* channelInput = context.getInputBlock().getChannelPointer(channel);
    auto* channelOutput = context.getOutputBlock().getChannelPointer(channel);

    auto len = context.getInputBlock().getNumSamples();


    switch (clipperMode)
    {
    case Clipper::hardClipper:
        for (int s = 0; s < len; ++s)
        {
            auto input = channelInput[s];
            auto gain = juce::Decibels::decibelsToGain(clipperThresh);
            channelInput[s] > gain ? channelOutput[s] = gain : channelOutput[s] = channelInput[s];
            channelInput[s] < -gain ? channelOutput[s] = -gain : channelOutput[s] = channelInput[s];
        }
        break;

    case Clipper::cubic:
        for (int s = 0; s < len; s++)
        {

            auto newLimit = juce::Decibels::decibelsToGain(clipperThresh);
            auto inverse = 1 / newLimit;
            auto resizeSamples = channelInput[s] * inverse;
            resizeSamples > 1 ? resizeSamples = 1 : resizeSamples = resizeSamples;
            resizeSamples < -1 ? resizeSamples = -1 : resizeSamples = resizeSamples;
            auto cubic = (resizeSamples - pow(resizeSamples, 3) / 3);
            channelOutput[s] = cubic * newLimit;
        }
        break;

    case Clipper::sin:
        for (int s = 0; s < len; ++s)
        {
            auto input = channelInput[s];
            auto newLimit = juce::Decibels::decibelsToGain(clipperThresh);
            auto inverse = 1 / newLimit;
            auto resizeSamples = channelInput[s] * inverse;
            resizeSamples > 1 ? resizeSamples = 1 : resizeSamples = resizeSamples;
            resizeSamples < -1 ? resizeSamples = -1 : resizeSamples = resizeSamples;
            auto sinosidal = std::sin(3 * juce::MathConstants<float>::pi * resizeSamples / 4);

            channelOutput[s] = sinosidal * newLimit;
        }
        break;

    case Clipper::hTangent:
        for (int s = 0; s < len; s++)
        {
            auto newLimit = juce::Decibels::decibelsToGain(clipperThresh);
            auto inverse = 1 / newLimit;
            auto resizeSamples = channelInput[s] * inverse;
            resizeSamples > 1 ? resizeSamples = 1 : resizeSamples = resizeSamples;
            resizeSamples < -1 ? resizeSamples = -1 : resizeSamples = resizeSamples;
            auto hyperTan = tanh(5 * resizeSamples) * (3 / juce::MathConstants<float>::pi);
            channelOutput[s] = hyperTan * newLimit;
        }
        break;

    case Clipper::arcTangent:
        for (int s = 0; s < len; ++s)
        {
            auto newLimit = juce::Decibels::decibelsToGain(clipperThresh);
            auto inverse = 1 / newLimit;
            auto resizeSamples = channelInput[s] * inverse;
            resizeSamples > 1 ? resizeSamples = 1 : resizeSamples = resizeSamples;
            resizeSamples < -1 ? resizeSamples = -1 : resizeSamples = resizeSamples;
            auto arcTan = atan(5 * resizeSamples) * (2 / juce::MathConstants<float>::pi);
            channelOutput[s] = arcTan * newLimit;
        }
        break;

    case Clipper::quintic:
        for (int s = 0; s < len; ++s)
        {
            auto newLimit = juce::Decibels::decibelsToGain(clipperThresh);
            auto inverse = 1 / newLimit;
            auto resizeSamples = channelInput[s] * inverse;
            resizeSamples > 1 ? resizeSamples = 1 : resizeSamples = resizeSamples;
            resizeSamples < -1 ? resizeSamples = -1 : resizeSamples = resizeSamples;
            auto quintic = resizeSamples - pow(resizeSamples, 5) / 5;
            channelOutput[s] = quintic * newLimit;
        }
        break;
    }


}

void Clipper::updateParams(int mode, float threshold)
{
    clipperMode = mode;
    clipperThresh = threshold;
}
