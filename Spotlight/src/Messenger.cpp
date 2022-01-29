#include "Messenger.h"

#include <math.h>

#include "DiagLed.h"
#include "AudioTriggerBandSelector.h"

Messenger::Messenger(int port)
	: _server(port)
{
}

void Messenger::Setup()
{
	_server.begin();
}

void Messenger::Loop()
{
}

void Messenger::SendSamplesBatch(uint16_t *data, uint16_t size)
{
	if (EnsureConnected())
	{
		_client.write("BEGIN SAMPLES_BATCH;t:" + String(millis()) + ";");
		String s = "";
		for (int i = 0; i < size; ++i)
		{
			s += String(data[i * 2]) + ";";
			if (i % 50 == 49 || i == size - 1)
			{
				_client.write(s);
				s = "";
			}
		}
		_client.write("END;");
	}
}

void Messenger::SendAmplitudeSpectrum(float *data, uint16_t size)
{
	if (EnsureConnected())
	{
		_client.write("BEGIN AMPLITUDE_SPECTRUM;t:" + String(millis()) + ";");
		String s = "";
		for (int i = 0; i < size; ++i)
		{
			s += String(sqrt(data[2 * i] * data[2 * i] + data[2 * i + 1] * data[2 * i + 1])) + ";";
			if (i % 50 == 49 || i == size - 1)
			{
				_client.write(s);
				s = "";
			}
		}
		_client.write("END;");
	}
}

void Messenger::SendAudioTriggerInfo(AudioTriggerBandSelector *bandSelector, float bandsSpectrumAverage, float oldestWholeAverage, float oldestBandsAverage, float *amplitudeSpectrumSquared, uint16_t amplitudeSpectrumSize)
{
	if (EnsureConnected())
	{
		_client.write("BEGIN AUDIO_TRIGGER;t:" + String(millis()) + ";bandSelector;" + String(bandSelector->MinimumFrequencyValueIndex) + ";" + String(bandSelector->MaximumFrequencyValueIndex) + ";");
		_client.write("bandsSpectrumAverage;" + String(bandsSpectrumAverage) + ";oldestWholeAverage;" + String(oldestWholeAverage) + ";oldestBandsAverage;" + String(oldestBandsAverage) + ";");
		String s = "amplitudeSpectrum;";
		for (int i = 0; i < amplitudeSpectrumSize; ++i)
		{
			s += String(amplitudeSpectrumSquared[i]) + ";";
			if (i % 50 == 49 || i == amplitudeSpectrumSize - 1)
			{
				_client.write(s);
				s = "";
			}
		}
		_client.write("END;");
	}
}

bool Messenger::EnsureConnected()
{
	if (_client.connected())
	{
		return true;
	}
	else
	{
		_client = _server.available();
		if (_client.connected())
		{
			_client.write("BEGIN WELCOME;<header>;Spotlight here!;END;");
			return true;
		}
	}

	return false;
}
