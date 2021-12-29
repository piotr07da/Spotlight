#include "Controller.h"

#include "Particle.h"

Controller::Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, int settingValueSensitivityPin, Display *display, SpotManager *spotManager)
{
	_previousSpotBtn = new Button(previousSpotPin, Button_DebounceDelay_SlowButton);
	_nextSpotBtn = new Button(nextSpotPin, Button_DebounceDelay_SlowButton);
	_previousSettingBtn = new Button(previousSettingPin, Button_DebounceDelay_SlowButton);
	_nextSettingBtn = new Button(nextSettingPin, Button_DebounceDelay_SlowButton);
	_decreaseSettingValueBtn = new Button(decreaseSettingValuePin, Button_DebounceDelay_SlowButton);
	_increaseSettingValueBtn = new Button(increaseSettingValuePin, Button_DebounceDelay_SlowButton);
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

	bool stateChanged = false;

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

			stateChanged = true;
		}
		else if (_decreaseSettingValueBtn->GetState(true) && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->DecreaseActiveSpotCount();

			stateChanged = true;
		}
		else if (_increaseSettingValueBtn->GetState(true) && _spotManager->GetActiveSpotCount() < SpotManager_MaxSpotCount - 1)
		{
			_spotManager->IncreaseActiveSpotCount();

			stateChanged = true;
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

			stateChanged = true;
		}
		else if (_nextSpotBtn->GetState(true))
		{
			_spotManager->NextSpot();

			stateChanged = true;
		}
		else if (_previousSettingBtn->GetState(true))
		{
			_spotManager->PreviousSetting();

			stateChanged = true;
		}
		else if (_nextSettingBtn->GetState(true))
		{
			_spotManager->NextSetting();

			stateChanged = true;
		}
		else if (_decreaseSettingValueBtn->GetState(true))
		{
			_spotManager->DecreaseSettingValue(1);

			stateChanged = true;
		}
		else if (_increaseSettingValueBtn->GetState(true))
		{
			_spotManager->IncreaseSettingValue(1);

			stateChanged = true;
		}
		break;
	}

	if (stateChanged)
	{
		switch (_mode)
		{
		case ControllerMode::GlobalSettings:
			_decreaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
			_increaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
			break;
		case ControllerMode::SpotSettings:
			SpotSetting setting = _spotManager->GetCurrentSetting();
			switch (setting)
			{
			case SpotSetting::Position:
			case SpotSetting::SpotTime:
			case SpotSetting::TravelTime:
				_decreaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_FastButton);
				_increaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_FastButton);
				break;
			default:
				_decreaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
				_increaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
				break;
			}
			break;
		}

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
