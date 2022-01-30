#include "MasterMenu.h"

#include "DiagLed.h"

MasterMenu::MasterMenu(int ldPin, int luPin, int rdPin, int ruPin, int rlPin, int rrPin, Display *display, Motor *motor, SpotCollection *spots)
	: _ldButton("ld", ldPin, Button_DebounceDelay_SlowButton),
	  _luButton("lu", luPin, Button_DebounceDelay_SlowButton),
	  _rdButton("rd", rdPin, Button_DebounceDelay_SlowButton),
	  _ruButton("ru", ruPin, Button_DebounceDelay_SlowButton),
	  _rlButton("rl", rlPin, Button_DebounceDelay_SlowButton),
	  _rrButton("rr", rrPin, Button_DebounceDelay_SlowButton),
	  _display(display),
	  _motor(motor),
	  _spots(spots),
	  _globalPropertiesMenu(&_rdButton, &_ruButton, _display, _spots),
	  _spotPropertiesMenu(&_ruButton, &_rdButton, &_rrButton, _display),
	  _spotPropertyValueMenu(&_rdButton, &_ruButton, &_rlButton, &_rrButton, _display, _motor)
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
		if (AnySpotMenuIsActive())
		{
			if (_currentSpotIndex > 0)
			{
				--_currentSpotIndex;
				ActivateSpotMenuForCurrentSpot();
			}
			else if (_currentSpotIndex == 0)
			{
				--_currentSpotIndex;
				DeactivateAllSpotMenus();
				_globalPropertiesMenu.Activate(true);
			}
		}
	}
	else if (_luButton.IsClicked())
	{
		if (_globalPropertiesMenu.IsActive() && _spots->GetCount() > 0)
		{
			++_currentSpotIndex;
			_globalPropertiesMenu.Deactivate();
			_spotPropertiesMenu.Activate(_spots->GetByIndex(0), SpotSetting::Position);
		}
		else if (AnySpotMenuIsActive() && _currentSpotIndex < _spots->GetCount() - 1)
		{
			++_currentSpotIndex;
			ActivateSpotMenuForCurrentSpot();
		}
		else
		{
		}
	}

	_globalPropertiesMenu.Loop();
	_spotPropertiesMenu.Loop();
	_spotPropertyValueMenu.Loop();
}

bool MasterMenu::AnySpotMenuIsActive()
{
	return _spotPropertiesMenu.IsActive() || _spotPropertyValueMenu.IsActive();
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
		_spotPropertiesMenu.Activate(_spots->GetByIndex(_currentSpotIndex), SpotSetting::Position);
	}
}

void MasterMenu::DeactivateAllSpotMenus()
{
	_spotPropertiesMenu.Deactivate();
	_spotPropertyValueMenu.Deactivate();
}