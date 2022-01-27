#ifndef AUDIO_TRIGGER_H
#define AUDIO_TRIGGER_H

#include <Particle.h>

#include "AudioSpectrumCalculator.h"

class AudioTrigger
{
private:
	AudioSpectrumCalculator *_spectrumCalculator;

public:
	AudioTrigger(AudioSpectrumCalculator *spectrumCalculator);

	void Setup();
	void Loop();
};

#endif // AUDIO_TRIGGER_H