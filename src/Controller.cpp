#include "Controller.h"

#include "Particle.h"

Controller::Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, int settingValueSensitivityPin, Display *display, SpotManager *spotManager)
{
	_previousSpotBtn = new Button(previousSpotPin);
	_nextSpotBtn = new Button(nextSpotPin);
	_previousSettingBtn = new Button(previousSettingPin);
	_nextSettingBtn = new Button(nextSettingPin);
	_decreaseSettingValueBtn = new Button(decreaseSettingValuePin);
	_increaseSettingValueBtn = new Button(increaseSettingValuePin);
	_settingValueSensitivityPin = settingValueSensitivityPin;
	_display = display;
	_spotManager = spotManager;

	_mode = ControllerMode::GlobalSettings;
}

void Controller::Setup()
{
	_previousSpotBtn->Setup();
	_nextSpotBtn->Setup();
	_previousSettingBtn->Setup();
	_nextSettingBtn->Setup();
	_decreaseSettingValueBtn->Setup();
	_increaseSettingValueBtn->Setup();
	pinMode(_settingValueSensitivityPin, PinMode::INPUT);

	_display->ShowWelcome();
}

void Controller::Loop()
{
	_previousSpotBtn->Loop();
	_nextSpotBtn->Loop();
	_previousSettingBtn->Loop();
	_nextSettingBtn->Loop();
	_decreaseSettingValueBtn->Loop();
	_increaseSettingValueBtn->Loop();

	bool refreshDisplay = false;

	switch (_mode)
	{
	case ControllerMode::GlobalSettings:
		if (_nextSpotBtn->GetState(true))
		{
			_spotManager->NextSpot();

			if (_spotManager->GetCurrentSpotIndex() > -1)
			{
				_mode = ControllerMode::SpotSettings;
			}

			refreshDisplay = true;
		}
		else if (_decreaseSettingValueBtn->GetState(true) && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->DecreaseActiveSpotCount();

			refreshDisplay = true;
		}
		else if (_increaseSettingValueBtn->GetState(true) && _spotManager->GetActiveSpotCount() < SpotManager_MaxSpotCount - 1)
		{
			_spotManager->IncreaseActiveSpotCount();

			refreshDisplay = true;
		}
		break;

	case ControllerMode::SpotSettings:
		if (_previousSpotBtn->GetState(true))
		{
			_spotManager->PreviousSpot();

			if (_spotManager->GetCurrentSpotIndex() == -1)
			{
				_mode = ControllerMode::GlobalSettings;
			}

			refreshDisplay = true;
		}
		else if (_nextSpotBtn->GetState(true))
		{
			_spotManager->NextSpot();

			refreshDisplay = true;
		}
		else if (_previousSettingBtn->GetState(true))
		{
			_spotManager->PreviousSetting();

			refreshDisplay = true;
		}
		else if (_nextSettingBtn->GetState(true))
		{
			_spotManager->NextSetting();

			refreshDisplay = true;
		}
		else if (_decreaseSettingValueBtn->GetState(true))
		{
			_spotManager->DecreaseSettingValue(1);

			refreshDisplay = true;
		}
		else if (_increaseSettingValueBtn->GetState(true))
		{
			_spotManager->IncreaseSettingValue(1);

			refreshDisplay = true;
		}
		break;
	}

	if (refreshDisplay)
	{
		switch (_mode)
		{
		case ControllerMode::GlobalSettings:
			_display->ShowGlobalSettings(_spotManager->GetActiveSpotCount());
			break;
		case ControllerMode::SpotSettings:
			_display->ShowSpotSetting(_spotManager->GetCurrentSpotIndex(), *_spotManager->GetCurrentSpot(), _spotManager->GetCurrentSetting());
			break;
		}
	}
}
