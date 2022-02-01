#ifndef MESSENGER_H
#define MESSENGER_H

#include <Particle.h>

#include "AudioTriggerBandSelector.h"

class Messenger
{
private:
	TCPServer _server;
	TCPClient _client;

	bool EnsureConnected();

public:
	Messenger(int port);

	void Setup();
	void Loop();

	void SendDiag(String message);
	void SendSamplesBatch(uint16_t *data, uint16_t size);
	void SendAmplitudeSpectrum(float *data, uint16_t size);
	void SendAudioTriggerInfo(AudioTriggerBandSelector *bandSelector, double wholeSpectrumAvg, double bandsSpectrumAvg, float *amplitudeSpectrumSquared, uint16_t amplitudeSpectrumSize);
};

#endif // MESSENGER_H