#include "AudioTriggerBandSelector.h"

#include "AudioSampler.h"

AudioTriggerBandSelector::AudioTriggerBandSelector(uint16_t minimumFrequency, uint16_t maximumFrequency)
{
	MinimumFrequencyValueIndex = AudioSampler_SingleHalfBufferSize * minimumFrequency / AudioSampler_Frequency;
	MaximumFrequencyValueIndex = AudioSampler_SingleHalfBufferSize * maximumFrequency / AudioSampler_Frequency;
}