#include "SpotPropertiesMenu.h"

#include "DiagLed.h"

SpotPropertiesMenu::SpotPropertiesMenu(Button *prevSpotButton, Button *nextSpotButton, Button *prevPropertyButton, Button *nextPropertyButton, Button *escapeButton, Button *enterButton, Display *display, Light *light, MenuSpotsNavigator *spotsNavigator)
	: _prevSpotButton(prevSpotButton),
	  _nextSpotButton(nextSpotButton),
	  _prevPropertyButton(prevPropertyButton),
	  _nextPropertyButton(nextPropertyButton),
	  _escapeButton(escapeButton),
	  _enterButton(enterButton),
	  _display(display),
	  _light(light),
	  _spotsNavigator(spotsNavigator)
{
}

void SpotPropertiesMenu::Loop()
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
	else if (_prevPropertyButton->IsClicked())
	{
		ChangeProperty(-1);
	}
	else if (_nextPropertyButton->IsClicked())
	{
		ChangeProperty(1);
	}
	else if (_escapeButton->IsClicked())
	{
		Deactivate();
		_masterMenuActivator->Activate();
	}
	else if (_enterButton->IsClicked())
	{
		Deactivate();
		_spotPropertyValueMenu->Activate(_currentProperty);
	}
}

void SpotPropertiesMenu::AssignMasterMenuActivator(MasterMenuActivator *masterMenuActivator)
{
	_masterMenuActivator = masterMenuActivator;
}

void SpotPropertiesMenu::AssignSpotPropertyValueMenu(SpotPropertyValueMenu *spotPropertyValueMenu)
{
	_spotPropertyValueMenu = spotPropertyValueMenu;
}

bool SpotPropertiesMenu::IsActive()
{
	return _isActive;
}

SpotProperty SpotPropertiesMenu::GetCurrentProperty()
{
	return _currentProperty;
}

void SpotPropertiesMenu::Activate(SpotProperty currentProperty)
{
	_currentProperty = currentProperty;

	_prevSpotButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_nextSpotButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_prevPropertyButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_nextPropertyButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_escapeButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_enterButton->ResetEnabled(Button_DebounceDelay_SlowButton);

	Show();

	_light->LightUpGentle();

	_spotsNavigator->RepositionToCurrent();

	_isActive = true;
}

void SpotPropertiesMenu::Deactivate()
{
	_isActive = false;
}

// PRIVATE METHODS

void SpotPropertiesMenu::ChangeProperty(int direction)
{
	if (direction != -1 && direction != 1)
	{
		return;
	}

	_currentProperty = (SpotProperty)((int)_currentProperty + direction);
	if (_currentProperty < SpotProperty::FIRST)
	{
		_currentProperty = SpotProperty::LAST;
	}
	else if (_currentProperty > SpotProperty::LAST)
	{
		_currentProperty = SpotProperty::FIRST;
	}

	Show();
}

void SpotPropertiesMenu::Show()
{
	_display->ShowSpotProperties(*_spotsNavigator->GetCurrent(), _currentProperty);
}