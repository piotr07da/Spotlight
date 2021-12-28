/*
 * Project Spotlight
 * Description:
 * Author: Piotr Bejger
 * Date: 2021-12-27
 */

#include "../lib/sh1106/src/sh1106.h"
#include "../lib/AccelStepperSpark/src/AccelStepper.h"
#include "SpotManager.h"
#include "Controller.h"
#include "Button.h"

sh1106_lcd *_glcd;
Display *_display;
SpotManager *_spotManager;
Controller *_inputController;

AccelStepper stepper = AccelStepper(AccelStepper::DRIVER, A3, A4);

void setup()
{
  pinMode(D7, OUTPUT);

  _glcd = sh1106_lcd::getInstance();

  _display = new Display(_glcd);
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
