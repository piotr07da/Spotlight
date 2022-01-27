#ifndef FAST_FOURIER_TRANSFORM_H
#define FAST_FOURIER_TRANSFORM_H

#include <Particle.h>

class FastFourierTransform
{
private:
	static void _FFT(float *data, uint16_t nn, int8_t isign);

public:
	static void FFT(float *data, uint16_t size);
	static void IFFT(float *data, uint16_t size);
};

#endif // FAST_FOURIER_TRANSFORM_H