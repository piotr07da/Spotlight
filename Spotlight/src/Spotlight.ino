/*
 * Project Spotlight
 * Description:
 * Author: Piotr Bejger
 * Date: 2021-12-27
 */

#include "SpotManager.h"

#include <math.h>

#include "Controller.h"
#include "Button.h"
#include "Light.h"
#include "Display.h"
#include "Runner.h"
#include "DiagLed.h"
#include "AudioSampler.h"
#include "AudioSpectrumCalculator.h"
#include "AudioTrigger.h"
#include "Messenger.h"
#include "Complex.h"

SpotManager _spotManager;
Motor _motor(A2, A3);
Light _light(D2);
Controller _controller(A0, A1, D3, D4, D5, D6, &_spotManager, &_motor, &_light);
Display _display(&_spotManager);
Runner _runner(&_spotManager, &_motor, &_light);
AudioSampler _audioSampler(A5);
AudioSpectrumCalculator _audioSpectrumCalculator(&_audioSampler);
AudioTrigger _audioTrigger(&_audioSpectrumCalculator);
Messenger _messenger(33334);

void setup()
{
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
    _audioSampler.Setup();
    _audioSpectrumCalculator.Setup();
    _audioTrigger.Setup();
    _messenger.Setup();

    DiagLed::Toggle();
}

void loop()
{
    _motor.Loop();
    _light.Loop();
    _controller.Loop();
    _display.Loop();
    _runner.Loop();
    _audioSampler.Loop();
    _audioSpectrumCalculator.Loop();
    _audioTrigger.Loop();
    _messenger.Loop();

    if (_audioSampler.DoubleHalfBufferReady())
    {
        _messenger.SendSamplesBatch(_audioSampler.DoubleHalfBuffer(), AudioSampler_SingleHalfBufferSize);
    }

    if (_audioSpectrumCalculator.SpectrumReady())
    {
        _messenger.SendAmplitudeSpectrum(_audioSpectrumCalculator.Spectrum(), AudioSampler_SingleHalfBufferSize);
    }
}
