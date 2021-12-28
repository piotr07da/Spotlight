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

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <AccelStepper.h>
#include "SpotManager.h"
#include "Controller.h"
#include "Button.h"

void setup();
void loop();
#line 15 "c:/_git/Spotlight/src/Spotlight.ino"
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display *_display;
SpotManager *_spotManager;
Controller *_inputController;

AccelStepper stepper = AccelStepper(AccelStepper::DRIVER, A3, A4);

void setup()
{
  pinMode(D7, OUTPUT);

  _display = new Display(&_oled);
  _spotManager = new SpotManager();
  _inputController = new Controller(D2, D3, D4, D5, A0, A1, A2, _display, _spotManager);

  _inputController->Setup();

  stepper.setMaxSpeed(90000);
  stepper.setAcceleration(1000);
  stepper.moveTo(32000);
}

void loop()
{
  _inputController->Loop();
  // stepper.run();
}
