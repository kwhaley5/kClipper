/*
  ==============================================================================

    kLevelMeterData.h
    Created: 2 Nov 2023 1:43:50pm
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct LevelMeterData
{
	void process(bool inOrOut, int channel, juce::AudioBuffer<float>& buffer)
	{
		if (inOrOut) //true is in rms, false is out rms
		{
			rmsIn[channel] = juce::Decibels::gainToDecibels(buffer.getRMSLevel(channel, 0, buffer.getNumSamples()));
			if (rmsIn[channel] < -60) { rmsIn[channel] = -60; }
		}
		else
		{
			rmsOut[channel] = juce::Decibels::gainToDecibels(buffer.getRMSLevel(channel, 0, buffer.getNumSamples()));
			if (rmsOut[channel] < -60) { rmsOut[channel] = -60; }
		}
	};
	float getRMS(int channel) { return rmsIn[channel]; };
	float getOutRMS(int channel) { return rmsOut[channel]; };

private:

	std::array<std::atomic<float>, 2> rmsIn;
	std::array<std::atomic<float>, 2> rmsOut;
};
