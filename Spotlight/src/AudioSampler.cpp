#include "AudioSampler.h"

#include "DiagLed.h"

AudioSampler::AudioSampler(int pin)
	: _adcDma(pin, _doubleFullBuffer, AudioSampler_DoubleFullBufferSize, AudioSampler_Frequency)
{
}

void AudioSampler::Setup()
{
	_adcDma.Start();
}

void AudioSampler::Loop()
{
	_doubleHalfBufferReady = false;

	if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_HTIF0)) // HTIF - half transfer interrupt flag
	{
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_HTIF0);
		_doubleHalfBuffer = _doubleFullBuffer;
		_doubleHalfBufferReady = true;
	}

	if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0)) // TCIF - transfer complete interrupt flag
	{
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
		_doubleHalfBuffer = _doubleFullBuffer + AudioSampler_DoubleHalfBufferSize;
		_doubleHalfBufferReady = true;
	}
}

uint16_t *AudioSampler::DoubleHalfBuffer()
{
	return _doubleHalfBuffer;
}

bool AudioSampler::DoubleHalfBufferReady()
{
	return _doubleHalfBufferReady;
}