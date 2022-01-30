#include "MasterMenu.h"

MasterMenu::MasterMenu(int ldPin, int luPin, int rdPin, int ruPin, int rlPin, int rrPin, Display *display, Motor *motor, SpotCollection *spotCollection)
	: _ldButton("ld", ldPin, Button_DebounceDelay_SlowButton),
	  _luButton("lu", luPin, Button_DebounceDelay_SlowButton),
	  _rdButton("rd", rdPin, Button_DebounceDelay_SlowButton),
	  _ruButton("ru", ruPin, Button_DebounceDelay_SlowButton),
	  _rlButton("rl", rlPin, Button_DebounceDelay_SlowButton),
	  _rrButton("rr", rrPin, Button_DebounceDelay_SlowButton),
	  _display(display),
	  _motor(motor),
	  _spotPropertiesMenu(&_ruButton, &_rdButton, &_rrButton, _display),
	  _spotPropertyValueMenu(&_rdButton, &_ruButton, &_rlButton, &_rrButton, _display, _motor),
	  _spotCollection(spotCollection)
{
}

void MasterMenu::Setup()
{
	_spotPropertiesMenu.AssignSpotPropertyValueMenu(&_spotPropertyValueMenu);
	_spotPropertyValueMenu.AssingSpotPropertiesMenu(&_spotPropertiesMenu);

	_spotPropertiesMenu.Activate(_spotCollection->GetByIndex(0), SpotSetting::Position);
}

void MasterMenu::Loop()
{
	_spotPropertiesMenu.Loop();
	_spotPropertyValueMenu.Loop();
}