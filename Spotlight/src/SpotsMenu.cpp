#include "SpotsMenu.h"

#include "DiagLed.h"

SpotsMenu::SpotsMenu(Button *prevSpotButton, Button *nextSpotButton, Button *escapeButton, Button *enterButton, Display *display, Light *light, MenuSpotsNavigator *spotsNavigator)
	: _prevSpotButton(prevSpotButton),
	  _nextSpotButton(nextSpotButton),
	  _escapeButton(escapeButton),
	  _enterButton(enterButton),
	  _display(display),
	  _light(light),
	  _spotsNavigator(spotsNavigator)
{
}

void SpotsMenu::Loop()
{
	if (!_isActive)
	{
		return;
	}

	if (_escapeButton->IsClicked())
	{
		Deactivate();
		_masterMenuActivator->Activate();
	}
	else if (_spotsNavigator->GetCount() > 0)
	{
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
		else if (_enterButton->IsClicked())
		{
			Deactivate();
			_spotPropertiesMenu->Activate(SpotProperty::Position);
		}
	}
}

void SpotsMenu::AssingMasterMenuActivator(MasterMenuActivator *masterMenuActivator)
{
	_masterMenuActivator = masterMenuActivator;
}

void SpotsMenu::AssingSpotPropertiesMenu(SpotPropertiesMenu *spotPropertiesMenu)
{
	_spotPropertiesMenu = spotPropertiesMenu;
}

void SpotsMenu::AssingSpotPropertyValueMenu(SpotPropertyValueMenu *spotPropertyValueMenu)
{
	_spotPropertyValueMenu = spotPropertyValueMenu;
}

void SpotsMenu::Activate()
{
	_prevSpotButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_nextSpotButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_escapeButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_enterButton->ResetEnabled(Button_DebounceDelay_SlowButton);

	Show();

	_light->LightUpGentle();

	_isActive = true;
}

void SpotsMenu::Deactivate()
{
	_isActive = false;
}

// PRIVATE METHODS

void SpotsMenu::Show()
{
	_display->ShowSpots(_spotsNavigator->GetAll(), _spotsNavigator->GetCurrentIndex());
}