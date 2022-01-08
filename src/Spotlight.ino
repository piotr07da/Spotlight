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
    _light = new Light(D2);
    _controller = new Controller(A0, A1, D3, D4, D5, D6, _spotManager, _motor, _light);
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

void loop()
{
    _controller->Loop();
    _display->Loop();
    _runner->Loop();
    _motor->Loop();
    _light->Loop();
}
