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
	  _globalPropertiesMenu(&_rdButton, &_ruButton, _display, _spots),
	  _spotPropertiesMenu(&_ruButton, &_rdButton, &_rrButton, _display),
	  _spotPropertyValueMenu(&_rdButton, &_ruButton, &_rlButton, &_rrButton, _display, _motor),
	  _standbyMenu(&_rlButton, &_rrButton, _audioTrigger, _display, _motor, _light, _runner, _spots)
{
}

void MasterMenu::Setup()
{
	_spotPropertiesMenu.AssignSpotPropertyValueMenu(&_spotPropertyValueMenu);
	_spotPropertyValueMenu.AssingSpotPropertiesMenu(&_spotPropertiesMenu);

	_globalPropertiesMenu.Activate(false);

	_display->ShowWelcome();

	_currentSpotIndex = -1;
}

void MasterMenu::Loop()
{
	_ldButton.Loop();
	_luButton.Loop();
	_rdButton.Loop();
	_ruButton.Loop();
	_rlButton.Loop();
	_rrButton.Loop();

	if (_ldButton.IsClicked())
	{
		if (_standbyMenu.IsActive())
		{
			_standbyMenu.Deactivate();
			ActivateSpotMenuForCurrentSpotAndPositionMotor();
			LightUp();
		}
		else if (AnySpotMenuIsActive())
		{
			if (_currentSpotIndex > 0)
			{
				--_currentSpotIndex;
				ActivateSpotMenuForCurrentSpotAndPositionMotor();
			}
			else if (_currentSpotIndex == 0)
			{
				DeactivateAllSpotMenus();
				_globalPropertiesMenu.Activate(true);
				PositionMotorOnZero();
				LightDown();
			}
		}
	}
	else if (_luButton.IsClicked())
	{
		if (_globalPropertiesMenu.IsActive() && _spots->GetCount() > 0)
		{
			_currentSpotIndex = 0;
			_globalPropertiesMenu.Deactivate();
			_spotPropertiesMenu.Activate(_spots->GetByIndex(0), SpotProperty::Position);
			PositionMotorOnCurrentSpot();
			LightUp();
		}
		else if (AnySpotMenuIsActive())
		{
			if (_currentSpotIndex < _spots->GetCount() - 1)
			{
				++_currentSpotIndex;
				ActivateSpotMenuForCurrentSpotAndPositionMotor();
			}
			else
			{
				DeactivateAllSpotMenus();
				_standbyMenu.Activate();
				LightDown();
			}
		}
	}

	_globalPropertiesMenu.Loop();
	_spotPropertiesMenu.Loop();
	_spotPropertyValueMenu.Loop();
	_standbyMenu.Loop();
}

bool MasterMenu::AnySpotMenuIsActive()
{
	return _spotPropertiesMenu.IsActive() || _spotPropertyValueMenu.IsActive();
}

void MasterMenu::ActivateSpotMenuForCurrentSpotAndPositionMotor()
{
	ActivateSpotMenuForCurrentSpot();
	PositionMotorOnCurrentSpot();
}

void MasterMenu::ActivateSpotMenuForCurrentSpot()
{
	if (_spotPropertiesMenu.IsActive())
	{
		_spotPropertiesMenu.Activate(_spots->GetByIndex(_currentSpotIndex), _spotPropertiesMenu.GetCurrentProperty());
	}
	else if (_spotPropertyValueMenu.IsActive())
	{
		_spotPropertyValueMenu.Activate(_spots->GetByIndex(_currentSpotIndex), _spotPropertyValueMenu.GetCurrentProperty());
	}
	else
	{
		_spotPropertiesMenu.Activate(_spots->GetByIndex(_currentSpotIndex), SpotProperty::Position);
	}
}

void MasterMenu::DeactivateAllSpotMenus()
{
	_spotPropertiesMenu.Deactivate();
	_spotPropertyValueMenu.Deactivate();
}

void MasterMenu::PositionMotorOnCurrentSpot()
{
	_motor->MoveToWithSpeed(_spots->GetByIndex(_currentSpotIndex)->Position, Motor_MaxSpeed);
}

void MasterMenu::PositionMotorOnZero()
{
	_motor->MoveToWithSpeed(0, Motor_MaxSpeed);
}

void MasterMenu::LightUp()
{
	_light->SetActivity(LightActivity::A_01, 400);
}

void MasterMenu::LightDown()
{
	_light->SetActivity(LightActivity::A_10, 400);
}