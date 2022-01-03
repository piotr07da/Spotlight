/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/_git/Spotlight/src/Spotlight.ino"
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

void setup();
void loop();
#line 17 "c:/_git/Spotlight/src/Spotlight.ino"
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
    _runner->Setup();
}

// int x = -1;

void loop()
{
    _controller->Loop();
    _display->Loop();
    _runner->Loop();
    _motor->Loop();
    _light->Loop();

    // if (_motor->HasFinished() || x == -1)
    // {
    //     ++x;

    //     if (x % 3 == 0)
    //     {
    //         if (!_motor->IsRunning())
    //         {
    //             delay(1000);
    //             _motor->MoveToInTime(100, .2f);
    //         }
    //     }
    //     else if (x % 3 == 1)
    //     {
    //         if (!_motor->IsRunning())
    //         {
    //             delay(100);
    //             _motor->MoveToInTime(-100, .4f);
    //         }
    //     }
    //     else if (x % 3 == 2)
    //     {
    //         if (!_motor->IsRunning())
    //         {
    //             delay(100);
    //             _motor->MoveToWithSpeed(0, 500);
    //         }
    //     }
    // }
}
