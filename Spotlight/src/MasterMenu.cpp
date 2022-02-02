#include "MasterMenu.h"

#include "DiagLed.h"

MasterMenu::MasterMenu(int ldPin, int luPin, int rdPin, int ruPin, int rlPin, int rrPin, AudioTrigger *audioTrigger, Display *display, Motor *motor, Light *light, Runner *runner, SpotCollection *spots)
	: _ldButton("ld", ldPin, Button_DebounceDelay_SlowButton),
	  _luButton("lu", luPin, Button_DebounceDelay_SlowButton),
	  _rdButton("rd", rdPin, Button_DebounceDelay_SlowButton),
	  _ruButton("ru", ruPin, Button_DebounceDelay_SlowButton),
	  _rlButton("rl", rlPin, Button_DebounceDelay_SlowButton),
	  _rrButton("rr", rrPin, Button_DebounceDelay_SlowButton),
	  _audioTrigger(audioTrigger),
	  _display(display),
	  _motor(motor),
	  _light(light),
	  _runner(runner),
	  _spots(spots),
	  _spotsNavigator(_spots, _motor),
	  _globalPropertiesMenu(&_rdButton, &_ruButton, &_rlButton, _display, &_spotsNavigator, _spots),
	  _spotPropertiesMenu(&_ldButton, &_luButton, &_ruButton, &_rdButton, &_rlButton, &_rrButton, _display, _light, &_spotsNavigator),
	  _spotPropertyValueMenu(&_ldButton, &_luButton, &_rdButton, &_ruButton, &_rlButton, &_rrButton, _display, _motor, &_spotsNavigator),
	  _standbyMenu(&_rlButton, &_rrButton, _audioTrigger, _display, _motor, _light, _runner, _spots)
{
}

void MasterMenu::Setup()
{
	_ldButton.Setup();
	_luButton.Setup();
	_rdButton.Setup();
	_ruButton.Setup();
	_rlButton.Setup();
	_rrButton.Setup();

	_globalPropertiesMenu.AssingMasterMenuActivator(&_activator);
	_spotPropertiesMenu.AssignMasterMenuActivator(&_activator);
	_spotPropertiesMenu.AssignSpotPropertyValueMenu(&_spotPropertyValueMenu);
	_spotPropertyValueMenu.AssingSpotPropertiesMenu(&_spotPropertiesMenu);
	_standbyMenu.AssingMasterMenuActivator(&_activator);

	_welcome = true;
	_display->ShowWelcome();
	_currentSubmenu = MasterMenuSubmenu::GlobalPropertiesMenu;
}

void MasterMenu::Loop()
{
	_ldButton.Loop();
	_luButton.Loop();
	_rdButton.Loop();
	_ruButton.Loop();
	_rlButton.Loop();
	_rrButton.Loop();

	_globalPropertiesMenu.Loop();
	_spotPropertiesMenu.Loop();
	_spotPropertyValueMenu.Loop();
	_standbyMenu.Loop();

	if (_welcome)
	{
		if (_ldButton.IsClicked() || _luButton.IsClicked() || _rdButton.IsClicked() || _ruButton.IsClicked() || _rlButton.IsClicked() || _rrButton.IsClicked())
		{
			Activate();
		}
		return;
	}

	if (_activator.ActivateRequested())
	{
		Activate();
	}

	if (!_isActive)
	{
		return;
	}

	auto prevMenuButton = &_ruButton;
	auto nextMenuButton = &_rdButton;
	auto enterMenuButton = &_rrButton;

	if (prevMenuButton->IsClicked())
	{
		ChangeSubmenu(-1);
	}
	else if (nextMenuButton->IsClicked())
	{
		ChangeSubmenu(1);
	}
	else if (enterMenuButton->IsClicked())
	{
		Deactivate();
		switch (_currentSubmenu)
		{
		case MasterMenuSubmenu::GlobalPropertiesMenu:
			_globalPropertiesMenu.Activate();
			break;

		case MasterMenuSubmenu::SpotPropertiesMenu:
			_spotPropertiesMenu.Activate(SpotProperty::Position);
			break;

		case MasterMenuSubmenu::StandbyMenu:
			_standbyMenu.Activate();
			break;

		case MasterMenuSubmenu::HelpMenu:
			Activate(); // TODO
			break;
		}
	}
}

void MasterMenu::Activate()
{
	_ldButton.ResetDisabled();
	_luButton.ResetDisabled();
	_rdButton.ResetEnabled();
	_ruButton.ResetEnabled();
	_rlButton.ResetDisabled();
	_rrButton.ResetEnabled();

	_rdButton.ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_ruButton.ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_rrButton.ChangeDebounceDelay(Button_DebounceDelay_SlowButton);

	_display->ShowMasterMenu(_currentSubmenu);

	_light->LightDownGentle();

	_welcome = false;
	_isActive = true;
}

void MasterMenu::Deactivate()
{
	_isActive = false;
}

// PRIVATE METHODS

void MasterMenu::ChangeSubmenu(int direction)
{
	if (direction != -1 && direction != 1)
	{
		return;
	}

	_currentSubmenu = (MasterMenuSubmenu)((int)_currentSubmenu + direction);
	if (_currentSubmenu < MasterMenuSubmenu::FIRST)
	{
		_currentSubmenu = MasterMenuSubmenu::LAST;
	}
	else if (_currentSubmenu > MasterMenuSubmenu::LAST)
	{
		_currentSubmenu = MasterMenuSubmenu::FIRST;
	}

	_display->ShowMasterMenu(_currentSubmenu);
}