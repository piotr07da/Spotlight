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

void setup();
void loop();
#line 14 "c:/_git/Spotlight/src/Spotlight.ino"
AccelStepper _stepper = AccelStepper(AccelStepper::DRIVER, A2, A3);
Motor *_motor;
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display *_display;
SpotManager *_spotManager;
Controller *_inputController;

void setup()
{
  pinMode(D7, OUTPUT);

  _motor = new Motor(&_stepper);
  _display = new Display(&_oled);
  _spotManager = new SpotManager();
  _inputController = new Controller(D2, D3, D4, D5, A0, A1, _motor, _display, _spotManager);

  _inputController->Setup();

  // pinMode(A2, OUTPUT);
  // pinMode(A3, OUTPUT);
  // digitalWrite(A3, HIGH);
}

void loop()
{

  // digitalWrite(A2, HIGH);
  // delayMicroseconds(500000);
  // digitalWrite(A2, LOW);
  // delayMicroseconds(500000);

  _inputController->Loop();
}
