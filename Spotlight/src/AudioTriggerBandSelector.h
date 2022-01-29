#ifndef AUDIO_TRIGGER_BAND_SELECTOR_H
#define AUDIO_TRIGGER_BAND_SELECTOR_H

#include <Particle.h>

class AudioTriggerBandSelector
{
public:
	AudioTriggerBandSelector(uint16_t minimumFrequency, uint16_t maximumFrequency);
	uint16_t MinimumFrequencyValueIndex;
	uint16_t MaximumFrequencyValueIndex;
};

#endif // AUDIO_TRIGGER_BAND_SELECTOR_H