#ifndef AUDIO_SPECTRUM_CALCULATOR_H
#define AUDIO_SPECTRUM_CALCULATOR_H

#include <Particle.h>

#include "AudioSampler.h"

class AudioSpectrumCalculator
{
private:
	AudioSampler *_sampler;
	bool _enabled;
	float _spectrum[AudioSampler_SingleHalfBufferSize * 2];
	bool _spectrumReady;

public:
	AudioSpectrumCalculator(AudioSampler *sampler);

	void Setup();
	void Loop();

	void Enable();
	void Disable();
	float *Spectrum();
	bool SpectrumReady();
};

#endif // AUDIO_SPECTRUM_CALCULATOR_H