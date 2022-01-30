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

#include <math.h>

#include "DiagLed.h"
#include "Messenger.h"
#include "SpotManager.h"
#include "SpotCollection.h"
#include "AudioSampler.h"
#include "AudioSpectrumCalculator.h"
#include "AudioTrigger.h"
#include "Motor.h"
#include "Light.h"
//#include "Controller.h"
#include "Display.h"
#include "MasterMenu.h"
#include "Runner.h"

void setup();
void loop();
#line 24 "c:/_git/Spotlight/Spotlight/src/Spotlight.ino"
Messenger _messenger(33334);
SpotManager _spotManager;
SpotCollection _spotCollection;
AudioSampler _audioSampler(A4);
AudioSpectrumCalculator _audioSpectrumCalculator(&_audioSampler);
AudioTrigger _audioTrigger(&_audioSpectrumCalculator, &_messenger);
Motor _motor(A2, A3);
Light _light(D2);
// Controller _controller(A0, A1, D3, D4, D5, D6, &_spotManager, &_audioTrigger, &_motor, &_light);
Display _display(&_spotManager);
MasterMenu _masterMenu(A0, A1, D3, D4, D5, D6, &_display, &_motor, &_spotCollection);
Runner _runner(&_spotManager, &_motor, &_light);

void setup()
{
    pinMode(D7, OUTPUT);

    // _controller.ModeChanged.Subscribe(
    //     [](ControllerMode mode)
    //     { _display.OnControllerModeChanged(mode); });

    // _controller.StartRequested.Subscribe(
    //     []()
    //     { _display.OnStartRequested(); });

    // _controller.StartRequested.Subscribe(
    //     []()
    //     { _runner.OnStartRequested(); });

    // _controller.StopRequested.Subscribe(
    //     []()
    //     { _runner.OnStopRequested(); });

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

    _messenger.Setup();
    _audioSampler.Setup();
    _audioSpectrumCalculator.Setup();
    _audioTrigger.Setup();
    _motor.Setup();
    _light.Setup();
    //_controller.Setup();
    _display.Setup();
    _masterMenu.Setup();
    _runner.Setup();

    // DiagLed::Toggle();
}

void loop()
{
    _messenger.Loop();
    _audioSampler.Loop();
    _audioSpectrumCalculator.Loop();
    _audioTrigger.Loop();
    _motor.Loop();
    _light.Loop();
    //_controller.Loop();
    _display.Loop();
    _masterMenu.Loop();
    _runner.Loop();

    // if (_audioSampler.DoubleHalfBufferReady())
    // {
    //     _messenger.SendSamplesBatch(_audioSampler.DoubleHalfBuffer(), AudioSampler_SingleHalfBufferSize);
    // }

    // if (_audioSpectrumCalculator.SpectrumReady())
    // {
    //     _messenger.SendAmplitudeSpectrum(_audioSpectrumCalculator.Spectrum(), AudioSampler_SingleHalfBufferSize);
    // }
}
