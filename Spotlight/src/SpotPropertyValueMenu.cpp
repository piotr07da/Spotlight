#include "SpotPropertyValueMenu.h"

#include "DiagLed.h"

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

SpotPropertyValueMenu::SpotPropertyValueMenu(Button *prevSpotButton, Button *nextSpotButton, Button *decreasePropertyValueButton, Button *increasePropertyValueButton, Button *escapeButton, Button *enterButton, Display *display, Motor *motor, MenuSpotsNavigator *spotsNavigator)
	: _prevSpotButton(prevSpotButton),
	  _nextSpotButton(nextSpotButton),
	  _decreasePropertyValueButton(decreasePropertyValueButton),
	  _increasePropertyValueButton(increasePropertyValueButton),
	  _escapeButton(escapeButton),
	  _enterButton(enterButton),
	  _display(display),
	  _motor(motor),
	  _spotsNavigator(spotsNavigator)
{
}

void SpotPropertyValueMenu::Loop()
{
	if (!_isActive)
	{
		return;
	}

	if (_prevSpotButton->IsClicked())
	{
		_spotsNavigator->Prev();
		Show();
	}
	else if (_nextSpotButton->IsClicked())
	{
		_spotsNavigator->Next();
		Show();
	}
	else if (_decreasePropertyValueButton->IsClicked())
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
		_spotPropertiesMenu->Activate(_currentProperty);
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
			Show();
			_motor->MoveToWithSpeed(_spotsNavigator->GetCurrent()->Position, Motor_MaxSpeed);
		}
	}
}

void SpotPropertyValueMenu::AssingSpotPropertiesMenu(SpotPropertiesMenu *spotPropertiesMenu)
{
	_spotPropertiesMenu = spotPropertiesMenu;
}

bool SpotPropertyValueMenu::IsActive()
{
	return _isActive;
}

SpotProperty SpotPropertyValueMenu::GetCurrentProperty()
{
	return _currentProperty;
}

void SpotPropertyValueMenu::Activate(SpotProperty currentProperty)
{
	_currentProperty = currentProperty;

	_prevSpotButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_nextSpotButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_decreasePropertyValueButton->ResetEnabled();
	_increasePropertyValueButton->ResetEnabled();
	_escapeButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_enterButton->ResetEnabled(Button_DebounceDelay_SlowButton);

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

	Show();

	_isActive = true;
}

void SpotPropertyValueMenu::Deactivate()
{
	_isActive = false;
}

// PRIVATE METHODS

void SpotPropertyValueMenu::ChangePropertyValue(int direction)
{
	if (direction != -1 && direction != 1)
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

	auto spot = _spotsNavigator->GetCurrent();

	switch (_currentProperty)
	{
	case SpotProperty::Position:
	{
		spot->Position += direction;
		break;
	}
	case SpotProperty::SpotTime:
	{
		spot->SpotTime += _valueDelta * direction;
		break;
	}
	case SpotProperty::SpotActivity:
	{
		spot->SpotActivity = FindActivity(_spotActivites, _spotActivitiesCount, spot->SpotActivity, direction);
		break;
	}
	case SpotProperty::TravelTime:
	{
		spot->TravelTime += _valueDelta * direction;
		break;
	}
	case SpotProperty::TravelActivity:
	{
		spot->TravelActivity = FindActivity(_travelActivites, _travelActivitiesCount, spot->TravelActivity, direction);
		break;
	}
	}

	if (_currentProperty == SpotProperty::Position)
	{
		_motor->MoveToWithSpeed(spot->Position, SpotPropertyValueMenu_ButtonSyncMotorSpeed);
	}
	else
	{
		Show();
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

void SpotPropertyValueMenu::Show()
{
	_display->ShowSpotProperty(*_spotsNavigator->GetCurrent(), _currentProperty);
}