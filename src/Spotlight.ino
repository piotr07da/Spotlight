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

AccelStepper _stepper = AccelStepper(AccelStepper::DRIVER, A2, A3);
Motor *_motor;
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display *_display;
SpotManager *_spotManager;
Controller *_controller;

void setup()
{
  pinMode(D7, OUTPUT);

  _motor = new Motor(&_stepper);
  _display = new Display(&_oled);
  _spotManager = new SpotManager();
  _controller = new Controller(D2, D3, D4, D5, A0, A1, _motor, _display, _spotManager);

  _controller->Setup();
}

void loop()
{
  _controller->Loop();
}
