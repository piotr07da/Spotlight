#include "SpotPropertiesMenu.h"

#include "DiagLed.h"

SpotPropertiesMenu::SpotPropertiesMenu(Button *prevPropertyButton, Button *nextPropertyButton, Button *enterButton, Display *display)
	: _prevPropertyButton(prevPropertyButton),
	  _nextPropertyButton(nextPropertyButton),
	  _enterButton(enterButton),
	  _display(display)
{
}

void SpotPropertiesMenu::Loop()
{
	if (!_isActive)
	{
		return;
	}

	if (_prevPropertyButton->IsClicked())
	{
		if (_currentProperty > SpotSetting::FIRST)
		{
			_currentProperty = (SpotSetting)((int)_currentProperty - 1);
		}
		else
		{
			_currentProperty = SpotSetting::LAST;
		}

		_display->ShowSpotProperties(*_spot, _currentProperty);
	}
	else if (_nextPropertyButton->IsClicked())
	{
		if (_currentProperty < SpotSetting::LAST)
		{
			_currentProperty = (SpotSetting)((int)_currentProperty + 1);
		}
		else
		{
			_currentProperty = SpotSetting::FIRST;
		}

		_display->ShowSpotProperties(*_spot, _currentProperty);
	}
	else if (_enterButton->IsClicked())
	{
		Deactivate();
		_spotPropertyValueMenu->Activate(_spot, _currentProperty);
	}
}

bool SpotPropertiesMenu::IsActive()
{
	return _isActive;
}

SpotSetting SpotPropertiesMenu::GetCurrentProperty()
{
	return _currentProperty;
}

void SpotPropertiesMenu::AssignSpotPropertyValueMenu(SpotPropertyValueMenu *spotPropertyValueMenu)
{
	_spotPropertyValueMenu = spotPropertyValueMenu;
}

void SpotPropertiesMenu::Activate(Spot *spot, SpotSetting currentProperty)
{
	_spot = spot;
	_currentProperty = currentProperty;

	_prevPropertyButton->ResetEnabled();
	_nextPropertyButton->ResetEnabled();
	_enterButton->ResetEnabled();

	_prevPropertyButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_nextPropertyButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_enterButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);

	_display->ShowSpotProperties(*_spot, _currentProperty);

	_isActive = true;
}

void SpotPropertiesMenu::Deactivate()
{
	_isActive = false;
}