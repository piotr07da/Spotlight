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
#include "SpotCollection.h"
#include "Messenger.h"
#include "AudioSampler.h"
#include "AudioSpectrumCalculator.h"
#include "AudioTrigger.h"
#include "Display.h"
#include "Motor.h"
#include "Light.h"
#include "Runner.h"
#include "MasterMenu.h"

void setup();
void loop();
#line 22 "c:/_git/Spotlight/Spotlight/src/Spotlight.ino"
SpotCollection _spots;
Messenger _messenger(33334);
AudioSampler _audioSampler(A4);
AudioSpectrumCalculator _audioSpectrumCalculator(&_audioSampler, &_messenger);
AudioTrigger _audioTrigger(&_audioSpectrumCalculator, &_messenger);
Display _display;
Motor _motor(A2, A3);
Light _light(D2);
Runner _runner(&_spots, &_motor, &_light);
MasterMenu _masterMenu(A0, A1, D3, D4, D5, D6, &_audioTrigger, &_display, &_motor, &_light, &_runner, &_spots);

void setup()
{
    pinMode(D7, OUTPUT);

    _messenger.Setup();
    _audioSampler.Setup();
    _audioSpectrumCalculator.Setup();
    _audioTrigger.Setup();
    _motor.Setup();
    _light.Setup();
    _display.Setup();
    _masterMenu.Setup();
    _runner.Setup();

    // DiagLed::Toggle();
    //_audioTrigger.Enable();
}

void loop()
{
    _messenger.Loop();
    _audioSampler.Loop();
    _audioSpectrumCalculator.Loop();
    _audioTrigger.Loop();
    _motor.Loop();
    _light.Loop();
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
