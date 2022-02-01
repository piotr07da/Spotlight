#include "AudioSpectrumCalculator.h"

#include <math.h>

#include "FastFourierTransform.h"
#include "DiagLed.h"

AudioSpectrumCalculator::AudioSpectrumCalculator(AudioSampler *sampler, Messenger *messenger)
	: _sampler(sampler),
	  _messenger(messenger)
{
}

void AudioSpectrumCalculator::Setup()
{
}

void AudioSpectrumCalculator::Loop()
{
	if (!_enabled)
	{
		return;
	}

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

		if (_fftStart == 0)
		{
			_fftStart = micros();
		}

		FastFourierTransform::FFT(_spectrum, AudioSampler_SingleHalfBufferSize);

		if (_fftStart > 0)
		{
			_messenger->SendDiag("FFT Microseconds: " + String(micros() - _fftStart));
			_fftStart = -1;
		}

		_spectrumReady = true;
	}
}

void AudioSpectrumCalculator::Enable()
{
	_enabled = true;
	_spectrumReady = false;
	// DiagLed::Toggle();
}

void AudioSpectrumCalculator::Disable()
{
	_enabled = false;
	_spectrumReady = false;
	// DiagLed::Toggle();
}

float *AudioSpectrumCalculator::Spectrum()
{
	return _spectrum;
}

bool AudioSpectrumCalculator::SpectrumReady()
{
	return _spectrumReady;
}
