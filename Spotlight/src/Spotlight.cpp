/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/_git/Spotlight/Spotlight/src/Spotlight.ino"
/*
 * Project Spotlight
 * Description:
 * Author: Piotr Bejger
 * Date: 2021-12-27
 */

#include "SpotManager.h"
#include "Adafruit_SH1106_headers.h"

#include <math.h>

#include "Controller.h"
#include "Button.h"
#include "Light.h"
#include "Display.h"
#include "Runner.h"
#include "AdcDma.h"
#include "DiagLed.h"
#include "Complex.h"

void setup();
void loop();
void SendSamplesBatchMessage();
void SendAmplitudeSpectrumMessage();
void CalculateSpectrum();
void fft1(Complex *a, int size);
void fft2(Complex *x, unsigned int size);
void fft3(float *data, uint16_t nn, uint8_t isign);
#line 22 "c:/_git/Spotlight/Spotlight/src/Spotlight.ino"
SpotManager _spotManager;
Motor _motor(A2, A3);
Light _light(D2);
Controller _controller(A0, A1, D3, D4, D5, D6, &_spotManager, &_motor, &_light);
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display _display(&_oled, &_spotManager);
Runner _runner(&_spotManager, &_motor, &_light);

const int AudioSamplesDoubleFullBufferSize = 2048;
const int AudioSamplesDoubleHalfBufferSize = AudioSamplesDoubleFullBufferSize / 2;
const int AudioSamplesSingleFullBufferSize = AudioSamplesDoubleFullBufferSize / 2;
const int AudioSamplesSingleHalfBufferSize = AudioSamplesDoubleHalfBufferSize / 2;
uint16_t _samplesDoubleFullBuffer[AudioSamplesDoubleFullBufferSize];
uint16_t *_samplesDoubleHalfBuffer;
Complex _spectrumSingleHalfBuffer[AudioSamplesSingleHalfBufferSize];
AdcDma _adcDma(A5, _samplesDoubleFullBuffer, AudioSamplesDoubleFullBufferSize, 20480);

TCPServer _server(33334);
TCPClient _client;

void setup()
{
    _server.begin();

    pinMode(D7, OUTPUT);

    pinMode(A5, INPUT);

    _controller.ModeChanged.Subscribe(
        [](ControllerMode mode)
        { _display.OnControllerModeChanged(mode); });

    _controller.StartRequested.Subscribe(
        []()
        { _display.OnStartRequested(); });

    _controller.StartRequested.Subscribe(
        []()
        { _runner.OnStartRequested(); });

    _controller.StopRequested.Subscribe(
        []()
        { _runner.OnStopRequested(); });

    _spotManager.NumberOfSpotsChanged.Subscribe(
        []()
        { _display.OnNumberOfSpotsChanged(); });

    _spotManager.SpotChanged.Subscribe(
        []()
        { _display.OnSpotChanged(); });

    _spotManager.SettingChanged.Subscribe(
        [](SpotSetting setting)
        { _display.OnSettingChanged(setting); });

    _spotManager.SettingValueChanged.Subscribe(
        []()
        { _display.OnSettingValueChanged(); });

    _motor.Setup();
    _light.Setup();
    _controller.Setup();
    _display.Setup();
    _runner.Setup();

    _adcDma.Start();

    // DiagLed::Toggle();
}

void loop()
{
    _controller.Loop();
    _display.Loop();
    _runner.Loop();
    _motor.Loop();
    _light.Loop();

    bool halfBufferReady = false;
    if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_HTIF0)) // HTIF - half transfer interrupt flag
    {
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_HTIF0);
        _samplesDoubleHalfBuffer = _samplesDoubleFullBuffer;
        halfBufferReady = true;
    }

    if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0)) // TCIF - transfer complete interrupt flag
    {
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
        _samplesDoubleHalfBuffer = _samplesDoubleFullBuffer + AudioSamplesDoubleHalfBufferSize;
        halfBufferReady = true;
    }

    if (halfBufferReady)
    {
        if (_client.connected())
        {
            SendSamplesBatchMessage();
            CalculateSpectrum();
            SendAmplitudeSpectrumMessage();
        }
        else
        {
            DiagLed::Toggle();
            _client = _server.available();
            if (_client.connected())
            {
                _client.write("BEGIN WELCOME;<header>;Spotlight here!;END;");
            }
        }
    }
}

float _spectrumSingleHalfBuffer2[AudioSamplesSingleHalfBufferSize * 2];

void SendSamplesBatchMessage()
{
    _client.write("BEGIN SAMPLES_BATCH;t:" + String(millis()) + ";");
    String s = "";
    for (int i = 0; i < AudioSamplesSingleHalfBufferSize; ++i)
    {
        s += String(_samplesDoubleHalfBuffer[i * 2]) + ";";
        if (i % 20 == 19 || i == AudioSamplesSingleHalfBufferSize - 1)
        {
            _client.write(s);
            s = "";
        }
    }
    _client.write("END;");
}

void SendAmplitudeSpectrumMessage()
{
    // _client.write("BEGIN AMPLITUDE_SPECTRUM;t:" + String(millis()) + ";");
    // String s = "";
    // for (int i = 0; i < AudioSamplesSingleHalfBufferSize; ++i)
    // {
    //     s += String(sqrt(_spectrumSingleHalfBuffer[i].Re * _spectrumSingleHalfBuffer[i].Re + _spectrumSingleHalfBuffer[i].Im * _spectrumSingleHalfBuffer[i].Im)) + ";";
    //     if (i % 20 == 19 || i == AudioSamplesSingleHalfBufferSize - 1)
    //     {
    //         _client.write(s);
    //         s = "";
    //     }
    // }
    // _client.write("END;");

    _client.write("BEGIN AMPLITUDE_SPECTRUM;t:" + String(millis()) + ";");
    String s = "";
    for (int i = 0; i < AudioSamplesSingleHalfBufferSize; ++i)
    {
        s += String(sqrt(_spectrumSingleHalfBuffer2[2 * i] * _spectrumSingleHalfBuffer2[2 * i] + _spectrumSingleHalfBuffer2[2 * i + 1] * _spectrumSingleHalfBuffer2[2 * i + 1])) + ";";
        if (i % 20 == 19 || i == AudioSamplesSingleHalfBufferSize - 1)
        {
            _client.write(s);
            s = "";
        }
    }
    _client.write("END;");
}

void CalculateSpectrum()
{
    u_int64_t sum = 0;
    for (int i = 0; i < AudioSamplesSingleHalfBufferSize; ++i)
    {
        sum += _samplesDoubleHalfBuffer[i * 2];
    }

    uint16_t avg = sum / AudioSamplesSingleHalfBufferSize;

    for (int i = 0; i < AudioSamplesSingleHalfBufferSize; ++i)
    {
        auto v = _samplesDoubleHalfBuffer[i * 2] - avg;
        _spectrumSingleHalfBuffer[i] = Complex(v);
        _spectrumSingleHalfBuffer2[i * 2] = v;
        _spectrumSingleHalfBuffer2[i * 2 + 1] = 0;
    }

    // fft2(_spectrumSingleHalfBuffer, AudioSamplesSingleHalfBufferSize);

    fft3(_spectrumSingleHalfBuffer2, AudioSamplesSingleHalfBufferSize, 1);
}

void fft1(Complex *a, int size)
{
    double PI = acos(-1);
    int n = size;

    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
        {
            Complex tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    for (int len = 2; len <= n; len <<= 1)
    {
        double ang = 2 * PI / len;
        Complex wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len)
        {
            Complex w(1);
            for (int j = 0; j < len / 2; j++)
            {
                Complex u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

void fft2(Complex *x, unsigned int size)
{
    // DFT
    unsigned int N = size, k = size, n;
    double thetaT = 3.14159265358979323846264338328L / N;
    Complex phiT(cos(thetaT), -sin(thetaT));
    Complex T;
    while (k > 1)
    {
        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        T = Complex(1.0L);
        for (unsigned int l = 0; l < k; l++)
        {
            for (unsigned int a = l; a < N; a += n)
            {
                unsigned int b = a + k;
                Complex t = x[a] - x[b];
                x[a] += x[b];
                x[b] = t * T;
            }
            T *= phiT;
        }
    }
    // Decimate
    unsigned int m = (unsigned int)log2(N);
    for (unsigned int a = 0; a < N; a++)
    {
        unsigned int b = a;
        // Reverse bits
        b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
        b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
        b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
        b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
        if (b > a)
        {
            Complex t = x[a];
            x[a] = x[b];
            x[b] = t;
        }
    }
    //// Normalize (This section make it not working correctly)
    // Complex f = 1.0 / sqrt(N);
    // for (unsigned int i = 0; i < N; i++)
    //	x[i] *= f;
}

void fft3(float *data, uint16_t nn, uint8_t isign)
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