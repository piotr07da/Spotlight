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
#include "AudioSampler.h"
#include "FastFourierTransform.h"
#include "AudioTrigger.h"
#include "Messenger.h"
#include "Complex.h"

SpotManager _spotManager;
Motor _motor(A2, A3);
Light _light(D2);
Controller _controller(A0, A1, D3, D4, D5, D6, &_spotManager, &_motor, &_light);
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display _display(&_oled, &_spotManager);
Runner _runner(&_spotManager, &_motor, &_light);
AudioSampler _audioSampler(A5);
FastFourierTransform _fft;
AudioTrigger _audioTrigger;


Complex _spectrumSingleHalfBuffer[AudioSamplesSingleHalfBufferSize];

TCPServer _server(33334);
Messenger _messenger(&_server);

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

    
}

float _spectrumSingleHalfBuffer2[AudioSamplesSingleHalfBufferSize * 2];

void CalculateSpectrum()
{
    
}

