#ifndef ADC_DMA_H
#define ADC_DMA_H

#include "Particle.h"

class AdcDma
{
private:
	int _pin;
	uint16_t *_buffer;
	size_t _bufferSize;
	size_t _frequency;

public:
	AdcDma(int pin, uint16_t *buffer, size_t bufferSize, size_t frequency);
	virtual ~AdcDma();

	void Start();
	void Stop();
};

#endif // ADC_DMA_H
