#include "FastFourierTransform.h"

#include <math.h>

void FastFourierTransform::FFT(float *data, uint16_t size)
{
	_FFT(data, size, 1);
}

void FastFourierTransform::IFFT(float *data, uint16_t size)
{
	_FFT(data, size, -1);
}

void FastFourierTransform::_FFT(float *data, uint16_t nn, int8_t isign)
{
	uint16_t n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	double tempr, tempi;

	n = nn << 1;
	j = 1;

	// This is the bit-reversal section of the routine.
	for (i = 1; i < n; i += 2)
	{
		if (j > i)
		{
			// Exchange the two complex numbers.
			tempr = data[j - 1];
			data[j - 1] = data[i - 1];
			data[i - 1] = tempr;
			tempr = data[j];
			data[j] = data[i];
			data[i] = tempr;
		}
		m = nn;
		while (m >= 2 && j > m)
		{
			j -= m;
			m >>= 1;
		}
		j += m;
	}

	mmax = 2;
	while (n > mmax)
	{
		// Outer loop executed log2 nn times.

		istep = mmax << 1;

		// Initialize the trigonometric recurrence.
		theta = isign * (6.28318530717959 / mmax);
		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2)
		{
			// Here are the two nested inner loops.
			for (i = m; i <= n; i += istep)
			{
				// This is the Danielson-Lanczos formula.
				j = i + mmax;

				tempr = wr * data[j - 1] - wi * data[j];
				tempi = wr * data[j] + wi * data[j - 1];

				data[j - 1] = data[i - 1] - tempr;
				data[j] = data[i] - tempi;
				data[i - 1] += tempr;
				data[i] += tempi;
			}
			// Trigonometric recurrence.
			wr = (wtemp = wr) * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax = istep;
	}
}