#ifndef AUDIO_TRIGGER_H
#define AUDIO_TRIGGER_H

#include <Particle.h>

#include "AudioTriggerBandSelector.h"
#include "AudioSampler.h"
#include "AudioSpectrumCalculator.h"
#include "Messenger.h"

const uint16_t AudioTrigger_AmplitudeSpectrumSize = AudioSampler_SingleHalfBufferSize / 2;
const uint16_t AudioTrigger_NumberOfLatestAverages = 100;

class AudioTrigger
{
private:
	AudioSpectrumCalculator *_spectrumCalculator;
	Messenger *_messenger;
	bool _enabled;
	float _amplitudeSpectrumSquared[AudioTrigger_AmplitudeSpectrumSize];
	float _latestWholeAverages[AudioTrigger_NumberOfLatestAverages];
	float _latestBandsAverages[AudioTrigger_NumberOfLatestAverages];
	uint16_t _lastAverageIndex;
	AudioTriggerBandSelector _bandSelector;
	bool _averagesFilled;
	bool _isTriggered;

public:
	AudioTrigger(AudioSpectrumCalculator *spectrumCalculator, Messenger *messenger);

	void Setup();
	void Loop();

	void Enable();
	void Disable();

	bool IsTriggered();
};

#endif // AUDIO_TRIGGER_H