#include "SpotPropertyValueMenu.h"

LightActivity SpotPropertyValueMenu::_spotActivites[7] = {
	LightActivity::A_1,
	LightActivity::A_010,
	LightActivity::A_01,
	LightActivity::A_10,
	LightActivity::A_0,
	LightActivity::A_101,
	LightActivity::A_Strobe,
};
int SpotPropertyValueMenu::_spotActivitiesCount = 7;
LightActivity SpotPropertyValueMenu::_travelActivites[7] = {
	LightActivity::A_0,
	LightActivity::A_101,
	LightActivity::A_10,
	LightActivity::A_01,
	LightActivity::A_1,
	LightActivity::A_010,
	LightActivity::A_Strobe,
};
int SpotPropertyValueMenu::_travelActivitiesCount = 7;

SpotPropertyValueMenu::SpotPropertyValueMenu(Button *decreasePropertyValueButton, Button *increasePropertyValueButton, Button *escapeButton, Button *enterButton, Display *display, Motor *motor)
	: _decreasePropertyValueButton(decreasePropertyValueButton),
	  _increasePropertyValueButton(increasePropertyValueButton),
	  _escapeButton(escapeButton),
	  _enterButton(enterButton),
	  _display(display),
	  _motor(motor)

{
}

void SpotPropertyValueMenu::Loop()
{
	if (!_isActive)
	{
		return;
	}

	if (_decreasePropertyValueButton->IsClicked())
	{
		ChangePropertyValue(-1);
	}
	else if (_increasePropertyValueButton->IsClicked())
	{
		ChangePropertyValue(1);
	}
	else if (_escapeButton->IsClicked())
	{
		Deactivate();
		_spotPropertiesMenu->Activate(_spot, _currentProperty);
	}
	else if (_enterButton->IsClicked())
	{
		// TODO SpotPropertyValuePropertiesMenu
	}
	else if (_decreasePropertyValueButton->IsReleased() || _increasePropertyValueButton->IsReleased())
	{
		_valueDelta = 0;
		_valueChangeCounter = 0;

		if (_currentProperty == SpotProperty::Position)
		{
			_display->ShowSpotProperty(*_spot, _currentProperty);
			_motor->MoveToWithSpeed(_spot->Position, Motor_MaxSpeed);
		}
	}
}

bool SpotPropertyValueMenu::IsActive()
{
	return _isActive;
}

SpotProperty SpotPropertyValueMenu::GetCurrentProperty()
{
	return _currentProperty;
}

void SpotPropertyValueMenu::AssingSpotPropertiesMenu(SpotPropertiesMenu *spotPropertiesMenu)
{
	_spotPropertiesMenu = spotPropertiesMenu;
}

void SpotPropertyValueMenu::Activate(Spot *spot, SpotProperty currentProperty)
{
	_spot = spot;
	_currentProperty = currentProperty;

	_decreasePropertyValueButton->ResetEnabled();
	_increasePropertyValueButton->ResetEnabled();
	_escapeButton->ResetEnabled();
	_enterButton->ResetEnabled();

	switch (currentProperty)
	{
	case SpotProperty::Position:
		_decreasePropertyValueButton->ChangeDebounceDelay(100);
		_increasePropertyValueButton->ChangeDebounceDelay(100);
		break;
	case SpotProperty::SpotTime:
	case SpotProperty::TravelTime:
		_decreasePropertyValueButton->ChangeDebounceDelay(Button_DebounceDelay_FastButton);
		_increasePropertyValueButton->ChangeDebounceDelay(Button_DebounceDelay_FastButton);
		break;
	default:
		_decreasePropertyValueButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
		_increasePropertyValueButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
		break;
	}

	_display->ShowSpotProperty(*_spot, _currentProperty);

	_isActive = true;
}

void SpotPropertyValueMenu::Deactivate()
{
	_isActive = false;
}

void SpotPropertyValueMenu::ChangePropertyValue(int sign)
{
	if (sign > 0)
	{
		sign = 1;
	}
	else if (sign < 0)
	{
		sign = -1;
	}
	else
	{
		return;
	}

	if (_currentProperty == SpotProperty::SpotTime || _currentProperty == SpotProperty::TravelTime)
	{
		if (_valueChangeCounter == 0)
		{
			_valueDelta = 1;
		}
		else if (_valueChangeCounter == 10)
		{
			_valueDelta = 2;
		}
		else if (_valueChangeCounter == 15)
		{
			_valueDelta = 5;
		}
		else if (_valueChangeCounter == 19)
		{
			_valueDelta = 10;
		}
		else if (_valueChangeCounter == 25)
		{
			_valueDelta = 100;
		}
	}

	switch (_currentProperty)
	{
	case SpotProperty::Position:
	{
		_spot->Position += 1 * sign;
		break;
	}
	case SpotProperty::SpotTime:
	{
		_spot->SpotTime += _valueDelta * sign;
		break;
	}
	case SpotProperty::SpotActivity:
	{
		_spot->SpotActivity = FindActivity(_spotActivites, _spotActivitiesCount, _spot->SpotActivity, sign);
		break;
	}
	case SpotProperty::TravelTime:
	{
		_spot->TravelTime -= _valueDelta;
		break;
	}
	case SpotProperty::TravelActivity:
	{
		_spot->TravelActivity = FindActivity(_travelActivites, _travelActivitiesCount, _spot->TravelActivity, sign);
		break;
	}
	}

	if (_currentProperty == SpotProperty::Position)
	{
		_motor->MoveToWithSpeed(_spot->Position, SpotPropertyValueMenu_ButtonSyncMotorSpeed);
	}
	else
	{
		_display->ShowSpotProperty(*_spot, _currentProperty);
	}

	++_valueChangeCounter;
}

LightActivity SpotPropertyValueMenu::FindActivity(LightActivity *activities, int activitiesCount, LightActivity currentActivity, int indexChangeValue)
{
	if (indexChangeValue > 0)
	{
		indexChangeValue = 1;
	}
	else if (indexChangeValue < 0)
	{
		indexChangeValue = -1;
	}
	else
	{
		return currentActivity;
	}

	int currentActivityIndex = -1;
	for (int i = 0; i < activitiesCount; ++i)
	{
		if (activities[i] == currentActivity)
		{
			currentActivityIndex = i;
			break;
		}
	}

	if (currentActivityIndex == -1)
	{
		return currentActivity;
	}

	currentActivityIndex += indexChangeValue;

	if (currentActivityIndex >= activitiesCount)
	{
		currentActivityIndex = 0;
	}
	else if (currentActivityIndex < 0)
	{
		currentActivityIndex = activitiesCount - 1;
	}

	return activities[currentActivityIndex];
}
