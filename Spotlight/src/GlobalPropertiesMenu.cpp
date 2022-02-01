#include "GlobalPropertiesMenu.h"

#include "DiagLed.h"

GlobalPropertiesMenu::GlobalPropertiesMenu(Button *remSpotButton, Button *addSpotButton, Button *escapeButton, Display *display, SpotCollection *spots)
	: _remSpotButton(remSpotButton),
	  _addSpotButton(addSpotButton),
	  _escapeButton(escapeButton),
	  _display(display),
	  _spots(spots)
{
	// At least two spots.
	AddDefaultSpot();
	AddDefaultSpot();
}

void GlobalPropertiesMenu::Loop()
{
	if (!_isActive)
	{
		return;
	}

	if (_remSpotButton->IsClicked())
	{
		// At least two spots.
		if (_spots->GetCount() > 2)
		{
			_spots->RemoveLast();
			_display->ShowGlobalProperties(_spots);
		}
	}
	else if (_addSpotButton->IsClicked())
	{
		AddDefaultSpot();
		_display->ShowGlobalProperties(_spots);
	}
	else if (_escapeButton->IsClicked())
	{
		Deactivate();
		_masterMenuActivator->Activate();
	}
}

void GlobalPropertiesMenu::AssingMasterMenuActivator(MasterMenuActivator *masterMenuActivator)
{
	_masterMenuActivator = masterMenuActivator;
}

bool GlobalPropertiesMenu::IsActive()
{
	return _isActive;
}

void GlobalPropertiesMenu::Activate()
{
	_remSpotButton->ResetEnabled();
	_addSpotButton->ResetEnabled();
	_escapeButton->ResetEnabled();

	_remSpotButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_addSpotButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_escapeButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);

	_display->ShowGlobalProperties(_spots);

	_isActive = true;
}

void GlobalPropertiesMenu::Deactivate()
{
	_isActive = false;
}

void GlobalPropertiesMenu::AddDefaultSpot()
{
	_spots->Add(0, 1000, LightActivity::A_1, 200, LightActivity::A_0);
}