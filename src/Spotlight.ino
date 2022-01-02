/*
 * Project Spotlight
 * Description:
 * Author: Piotr Bejger
 * Date: 2021-12-27
 */

#include "SpotManager.h"
#include "Adafruit_SH1106_headers.h"

#include "Controller.h"
#include "Button.h"
#include "Light.h"
#include "Display.h"
#include "Runner.h"

SpotManager *_spotManager;
Motor *_motor;
Light *_light;
Controller *_controller;
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display *_display;
Runner *_runner;

void setup()
{
    pinMode(D7, OUTPUT);

    _spotManager = new SpotManager();
    _motor = new Motor(A2, A3);
    _light = new Light(WKP);
    _controller = new Controller(D2, D3, D4, D5, A0, A1, _spotManager, _motor);
    _display = new Display(&_oled, _controller, _spotManager, NULL);
    _runner = new Runner(_spotManager, _motor, _light);

    _controller->ModeChanged.Subscribe(
        [](ControllerMode mode)
        { _display->OnControllerModeChanged(mode); });

    _controller->StartRequested.Subscribe(
        []()
        { _display->OnStartRequested(); });

    _controller->StartRequested.Subscribe(
        []()
        { _runner->OnStartRequested(); });

    _controller->StopRequested.Subscribe(
        []()
        { _runner->OnStopRequested(); });

    _spotManager->NumberOfSpotsChanged.Subscribe(
        []()
        { _display->OnNumberOfSpotsChanged(); });

    _spotManager->SpotChanged.Subscribe(
        []()
        { _display->OnSpotChanged(); });

    _spotManager->SettingChanged.Subscribe(
        [](SpotSetting setting)
        { _display->OnSettingChanged(setting); });

    _spotManager->SettingValueChanged.Subscribe(
        []()
        { _display->OnSettingValueChanged(); });

    _motor->Setup();
    _light->Setup();
    _controller->Setup();
    _display->Setup();
    //_runner->Setup();

    Particle.publish("diag", String(_motor->GetCurrentPosition()));
    _motor->MoveToInTime(200, 5);
}

// int x;
// int cps[5];

void loop()
{
    _motor->Loop();
    _light->Loop();
    //_controller->Loop();
    _display->Loop();
    //_runner->Loop();

    if (_motor->HasFinished())
    {
        Particle.publish("diag", String(_motor->GetCurrentPosition()));
    }

    // if (_motor->HasFinished())
    // {
    //     int p = _motor->GetCurrentPosition();
    //     cps[x] = p;

    //     ++x;

    //     if (x == 5)
    //     {
    //         Particle.publish("diag", "R: " + String(cps[0]) + ", " + String(cps[1]) + ", " + String(cps[2]) + ", " + String(cps[3]) + ", " + String(cps[4]));
    //     }
    // }

    // if (x == 0)
    // {
    //     if (!_motor->IsRunning())
    //     {
    //         _motor->MoveToInTime(55, .3f);
    //     }
    // }
    // else if (x == 1)
    // {
    //     if (!_motor->IsRunning())
    //     {
    //         _motor->MoveToInTime(-50, .6f);
    //     }
    // }
    // else if (x == 2)
    // {
    //     if (!_motor->IsRunning())
    //     {
    //         _motor->MoveToWithSpeed(57, 500);
    //     }
    // }
    // else if (x == 3)
    // {
    //     if (!_motor->IsRunning())
    //     {
    //         _motor->MoveToWithSpeed(-54, 500);
    //     }
    // }
    // else if (x == 4)
    // {
    //     if (!_motor->IsRunning())
    //     {
    //         _motor->MoveToWithSpeed(0, 500);
    //     }
    // }
}
