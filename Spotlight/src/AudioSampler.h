#ifndef AUDIO_SAMPLER_H
#define AUDIO_SAMPLER_H

#include <Particle.h>

#include "AdcDma.h"

static const int AudioSampler_Frequency = 10240;		   // 10.24kHz
static const int AudioSampler_DoubleFullBufferSize = 2048; // has to be power of 2 for FFT
static const int AudioSampler_DoubleHalfBufferSize = AudioSampler_DoubleFullBufferSize / 2;
static const int AudioSampler_SingleFullBufferSize = AudioSampler_DoubleFullBufferSize / 2;
static const int AudioSampler_SingleHalfBufferSize = AudioSampler_DoubleHalfBufferSize / 2;

class AudioSampler
{
private:
	AdcDma _adcDma;
	uint16_t _doubleFullBuffer[AudioSampler_DoubleFullBufferSize];
	uint16_t *_doubleHalfBuffer;
	bool _doubleHalfBufferReady;

public:
	AudioSampler(int pin);

	void Setup();
	void Loop();

	uint16_t *DoubleHalfBuffer();
	bool DoubleHalfBufferReady();
};

#endif // AUDIO_SAMPLER_H