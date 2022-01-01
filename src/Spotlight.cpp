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

#include <AccelStepper.h>
#include "SpotManager.h"
#include "Adafruit_SH1106_headers.h"

#include "Controller.h"
#include "Button.h"
#include "Light.h"
#include "Display.h"

/*
SpotManager *_spotManager;
AccelStepper _stepper = AccelStepper(AccelStepper::DRIVER, A2, A3);
Motor *_motor;
Light *_light;
Controller *_controller;
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display *_display;
*/

void setup();
void loop();
#line 28 "c:/_git/Spotlight/src/Spotlight.ino"
void setup()
{
    pinMode(D7, OUTPUT);
    digitalWrite(D7, HIGH);

    /*
        _spotManager = new SpotManager();
        _motor = new Motor(&_stepper);
        _light = new Light(WKP);
        _controller = new Controller(D2, D3, D4, D5, A0, A1, _spotManager, _motor);
        _display = new Display(&_oled, _controller, _spotManager, NULL);

        _controller->ModeChanged.Subscribe(
            [](ControllerMode mode)
            { _display->OnControllerModeChanged(mode); });

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
        */
}

void loop()
{
    digitalWrite(D7, HIGH);

    /*
    _controller->Loop();
    _display->Loop();
    _motor->Loop();
    _light->Loop();
    */
}
