#ifndef MESSENGER_H
#define MESSENGER_H

class Messenger
{
private:
	TCPServer *_server;
	TCPClient _client;

	bool EnsureConnected();

public:
	Messenger(TCPServer *server);

	void SendSamplesBatch(uint16_t *data, uint16_t size);
	void SendAmplitudeSpectrum(float *data, uint16_t size);
};

#endif // MESSENGER_H