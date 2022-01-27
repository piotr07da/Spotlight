#include "AudioTrigger.h"

#include "FastFourierTransform.h"

AudioTrigger::AudioTrigger(AudioSpectrumCalculator *spectrumCalculator)
	: _spectrumCalculator(spectrumCalculator)
{
}

void AudioTrigger::Setup()
{
}

void AudioTrigger::Loop()
{
	if (_spectrumCalculator->SpectrumReady())
	{
	}
}
