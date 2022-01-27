#include "AudioSpectrumCalculator.h"

#include <math.h>

#include "FastFourierTransform.h"

AudioSpectrumCalculator::AudioSpectrumCalculator(AudioSampler *sampler)
	: _sampler(sampler)
{
}

void AudioSpectrumCalculator::Setup()
{
}

void AudioSpectrumCalculator::Loop()
{
	_spectrumReady = false;

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

		_spectrumReady = true;
	}
}

float *AudioSpectrumCalculator::Spectrum()
{
	return _spectrum;
}

bool AudioSpectrumCalculator::SpectrumReady()
{
	return _spectrumReady;
}
