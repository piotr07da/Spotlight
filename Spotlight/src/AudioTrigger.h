#ifndef AUDIO_TRIGGER_H
#define AUDIO_TRIGGER_H

#include <Particle.h>

#include "AudioSampler.h"
#include "AudioSpectrumCalculator.h"

const uint16_t AudioTrigger_AmplitudeSpectrumSize = AudioSampler_SingleHalfBufferSize / 2;
const uint16_t AudioTrigger_NumberOfLatestAverages = 100;

class BandSelector
{
public:
	BandSelector(uint16_t minimumFrequency, uint16_t maximumFrequency);
	uint16_t MinimumFrequencyValueIndex;
	uint16_t MaximumFrequencyValueIndex;
};

class AudioTrigger
{
private:
	AudioSpectrumCalculator *_spectrumCalculator;
	float _amplitudeSpectrumSquared[AudioTrigger_AmplitudeSpectrumSize];
	float _latestWholeAverages[AudioTrigger_NumberOfLatestAverages];
	float _latestBandsAverages[AudioTrigger_NumberOfLatestAverages];
	uint16_t _lastAverageIndex;
	BandSelector _bandSelector;
	bool _averagesFilled;

public:
	AudioTrigger(AudioSpectrumCalculator *spectrumCalculator);

	void Setup();
	void Loop();
};

#endif // AUDIO_TRIGGER_H