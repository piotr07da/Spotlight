#ifndef AUDIO_TRIGGER_H
#define AUDIO_TRIGGER_H

#include "AudioSampler.h"

class AudioTrigger
{
private:
	AudioSampler *_sampler;
	float _spectrum[AudioSampler_SingleHalfBufferSize * 2];

public:
	AudioTrigger(AudioSampler *sampler);

	void Loop();
	
	
};

#endif // AUDIO_TRIGGER_H