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
#include "Adafruit_SH1106_headers.h"
#include "SpotManager.h"
#include "Controller.h"
#include "Button.h"
#include "Light.h"

void setup();
void loop();
#line 15 "c:/_git/Spotlight/src/Spotlight.ino"
AccelStepper _stepper = AccelStepper(AccelStepper::DRIVER, A2, A3);
Motor *_motor;
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display *_display;
SpotManager *_spotManager;
Controller *_controller;

Light *_light;

int _t0;
LightActivity _la;

void setup()
{
  pinMode(D7, OUTPUT);

  _motor = new Motor(&_stepper);
  _display = new Display(&_oled);
  _spotManager = new SpotManager();
  _controller = new Controller(D2, D3, D4, D5, A0, A1, _motor, _display, _spotManager);

  _light = new Light(WKP);
  _light->Setup();

  _t0 = millis();
  _la = (LightActivity)((int)LightActivity::FIRST - 1);

  _light->SetActivity(LightActivity::A_Strobe, 5000);

  _controller->Setup();
}

void loop()
{
  if (millis() - _t0 > 5000)
  {
    _t0 = millis();
    _la = (LightActivity)((int)_la + 1);
    if ((int)_la <= (int)LightActivity::LAST)
    {
      _light->SetActivity(_la, 5000);
    }
  }

  _controller->Loop();
  _light->Loop();
}
