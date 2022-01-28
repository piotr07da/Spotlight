#include "AudioTrigger.h"

#include <Particle.h>
#include <math.h>

#include "AudioSampler.h"
#include "DiagLed.h"

BandSelector::BandSelector(uint16_t minimumFrequency, uint16_t maximumFrequency)
{
	MinimumFrequencyValueIndex = AudioSampler_SingleHalfBufferSize * minimumFrequency / AudioSampler_Frequency;
	MaximumFrequencyValueIndex = AudioSampler_SingleHalfBufferSize * maximumFrequency / AudioSampler_Frequency;
}

AudioTrigger::AudioTrigger(AudioSpectrumCalculator *spectrumCalculator)
	: _spectrumCalculator(spectrumCalculator), _bandSelector(2318, 2322) // PACT TIMER - 2320Hz
{
}

void AudioTrigger::Setup()
{
	Particle.publish("diag", "_bandSelector:[" + String(_bandSelector.MinimumFrequencyValueIndex) + "," + String(_bandSelector.MaximumFrequencyValueIndex) + "]");
}

void AudioTrigger::Loop()
{
	if (_spectrumCalculator->SpectrumReady())
	{
		float *spectrum = _spectrumCalculator->Spectrum();

		double wholeSpectrumSum = 0;
		double bandsSpectrumSum = 0;

		for (uint16_t i = 0; i < AudioTrigger_AmplitudeSpectrumSize; ++i)
		{
			auto a = spectrum[2 * i] * spectrum[2 * i] + spectrum[2 * i + 1] * spectrum[2 * i + 1];
			_amplitudeSpectrumSquared[i] = a;
		}

		if (_amplitudeSpectrumSquared[_bandSelector.MinimumFrequencyValueIndex - 1] > _amplitudeSpectrumSquared[_bandSelector.MinimumFrequencyValueIndex] ||
			_amplitudeSpectrumSquared[_bandSelector.MaximumFrequencyValueIndex + 1] > _amplitudeSpectrumSquared[_bandSelector.MaximumFrequencyValueIndex])
		{
			return;
		}

		for (uint16_t i = 0; i < AudioTrigger_AmplitudeSpectrumSize; ++i)
		{
			auto a = _amplitudeSpectrumSquared[i];

			wholeSpectrumSum += a;

			if (i >= _bandSelector.MinimumFrequencyValueIndex &&
				i <= _bandSelector.MaximumFrequencyValueIndex)
			{
				bandsSpectrumSum += a;
			}
		}

		auto wholeSpectrumAvg = wholeSpectrumSum / AudioTrigger_AmplitudeSpectrumSize;
		auto bandsSpectrumAvg = bandsSpectrumSum / (_bandSelector.MaximumFrequencyValueIndex - _bandSelector.MinimumFrequencyValueIndex + 1);

		_latestWholeAverages[_lastAverageIndex] = wholeSpectrumAvg;
		_latestBandsAverages[_lastAverageIndex] = bandsSpectrumAvg;

		if (_averagesFilled)
		{
			auto oldestAverageIndex = _lastAverageIndex + 1;
			if (oldestAverageIndex == AudioTrigger_NumberOfLatestAverages)
			{
				oldestAverageIndex = 0;
			}

			if (bandsSpectrumAvg > _latestWholeAverages[oldestAverageIndex] &&
				bandsSpectrumAvg > 10000 * _latestBandsAverages[oldestAverageIndex])
			{
				Particle.publish("diag-audio-trig", "bsavg:" + String(bandsSpectrumAvg) + ", olwa:" + String(_latestWholeAverages[oldestAverageIndex]) + ", olba:" + String(_latestBandsAverages[oldestAverageIndex]));
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
