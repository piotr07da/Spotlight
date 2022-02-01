#include "AudioTrigger.h"

#include <Particle.h>
#include <math.h>

#include "AudioSampler.h"
#include "DiagLed.h"

AudioTrigger::AudioTrigger(AudioSpectrumCalculator *spectrumCalculator, Messenger *messenger)
	: _spectrumCalculator(spectrumCalculator),
	  _messenger(messenger),
	  _bandSelector(2320, 2340) // PACT TIMER freq
{
}

void AudioTrigger::Setup()
{
}

void AudioTrigger::Loop()
{
	if (!_enabled)
	{
		return;
	}

	if (_spectrumCalculator->SpectrumReady())
	{
		float *spectrum = _spectrumCalculator->Spectrum();

		double wholeSpectrumSum = 0;

		for (uint16_t i = 0; i < AudioTrigger_AmplitudeSpectrumSize; ++i)
		{
			auto a = spectrum[2 * i] * spectrum[2 * i] + spectrum[2 * i + 1] * spectrum[2 * i + 1];
			a /= 1000; // To keep numbers smaller.
			_amplitudeSpectrumSquared[i] = a;

			wholeSpectrumSum += a;
		}

		auto wholeSpectrumAvg = wholeSpectrumSum / AudioTrigger_AmplitudeSpectrumSize;

		double bandsSpectrumSum = 0;

		for (uint16_t i = _bandSelector.MinimumFrequencyValueIndex; i <= _bandSelector.MaximumFrequencyValueIndex; ++i)
		{
			bandsSpectrumSum += _amplitudeSpectrumSquared[i];
		}

		auto bandsSpectrumAvg = bandsSpectrumSum / (_bandSelector.MaximumFrequencyValueIndex - _bandSelector.MinimumFrequencyValueIndex + 1);

		if (_amplitudeSpectrumSquared[_bandSelector.MinimumFrequencyValueIndex - 1] > bandsSpectrumAvg ||
			_amplitudeSpectrumSquared[_bandSelector.MaximumFrequencyValueIndex + 1] > bandsSpectrumAvg)
		{
			return;
		}

		if (bandsSpectrumAvg > wholeSpectrumAvg * 70)
		{
			_messenger->SendAudioTriggerInfo(&_bandSelector, wholeSpectrumAvg, bandsSpectrumAvg, _amplitudeSpectrumSquared, AudioTrigger_AmplitudeSpectrumSize);
			_isTriggered = true;
			DiagLed::Toggle();
		}
	}
}

void AudioTrigger::Enable()
{
	_spectrumCalculator->Enable();
	_enabled = true;
}

void AudioTrigger::Disable()
{
	_spectrumCalculator->Disable();
	_enabled = false;
}

bool AudioTrigger::IsTriggered()
{
	auto isTriggered = _isTriggered;
	if (_isTriggered)
	{
		_isTriggered = false;
	}
	return isTriggered;
}