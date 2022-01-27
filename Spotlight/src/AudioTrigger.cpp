#include "AudioTrigger.h"

#include "FastFourierTransform.h"

AudioTrigger::AudioTrigger(AudioSampler *sampler)
	: _sampler(sampler)
{
}

void AudioTrigger::Loop()
{
	if (_sampler->DoubleHalfBufferReady())
	{
		uint16_t *doubleHalfBuffer = _sampler->DoubleHalfBuffer();

		u_int64_t sum = 0;
		for (int i = 0; i < AudioSampler_SingleHalfBufferSize; ++i)
		{
			sum += doubleHalfBuffer[i * 2];
		}

		uint16_t avg = sum / AudioSampler_SingleHalfBufferSize;

		for (int i = 0; i < AudioSampler_SingleHalfBufferSize; ++i)
		{
			auto v = doubleHalfBuffer[i * 2] - avg;
			_spectrum[i * 2] = v;
			_spectrum[i * 2 + 1] = 0;
		}

		FastFourierTransform::FFT(_spectrum, AudioSampler_SingleHalfBufferSize);
	}
}
