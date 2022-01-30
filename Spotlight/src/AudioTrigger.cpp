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
			_amplitudeSpectrumSquared[i] = a;

			wholeSpectrumSum += a;
		}

		auto wholeSpectrumAvg = wholeSpectrumSum / AudioTrigger_AmplitudeSpectrumSize;

		double bandSpectrumSum = 0;

		for (uint16_t i = _bandSelector.MinimumFrequencyValueIndex; i <= _bandSelector.MaximumFrequencyValueIndex; ++i)
		{
			bandSpectrumSum += _amplitudeSpectrumSquared[i];
		}

		auto bandSpectrumAvg = bandSpectrumSum / (_bandSelector.MaximumFrequencyValueIndex - _bandSelector.MinimumFrequencyValueIndex + 1);

		if (_amplitudeSpectrumSquared[_bandSelector.MinimumFrequencyValueIndex - 1] > bandSpectrumAvg ||
			_amplitudeSpectrumSquared[_bandSelector.MaximumFrequencyValueIndex + 1] > bandSpectrumAvg)
		{
			return;
		}

		auto bandsSpectrumAvg = bandSpectrumAvg;

		_latestWholeAverages[_lastAverageIndex] = wholeSpectrumAvg;
		_latestBandsAverages[_lastAverageIndex] = bandSpectrumAvg;

		if (_averagesFilled)
		{
			auto oldestAverageIndex = _lastAverageIndex + 1;
			if (oldestAverageIndex == AudioTrigger_NumberOfLatestAverages)
			{
				oldestAverageIndex = 0;
			}

			if (bandsSpectrumAvg > _latestWholeAverages[oldestAverageIndex] &&
				bandsSpectrumAvg > 300 * _latestBandsAverages[oldestAverageIndex])
			{
				//_messenger->SendAudioTriggerInfo(&_bandSelector, bandsSpectrumAvg, _latestWholeAverages[oldestAverageIndex], _latestBandsAverages[oldestAverageIndex], _amplitudeSpectrumSquared, AudioTrigger_AmplitudeSpectrumSize);
				_isTriggered = true;
				DiagLed::Toggle();
			}
		}

		++_lastAverageIndex;
		if (_lastAverageIndex == AudioTrigger_NumberOfLatestAverages)
		{
			_lastAverageIndex = 0;
			_averagesFilled = true;
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