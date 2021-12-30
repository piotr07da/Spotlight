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

	_settingValueDelta = 0;
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
		if (_nextSpotBtn->IsClicked() && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->NextSpot();
			_mode = ControllerMode::SpotSettings;
			stateChanged = true;
		}
		else if (_decreaseSettingValueBtn->IsClicked() && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->DecreaseActiveSpotCount();

			stateChanged = true;
		}
		else if (_increaseSettingValueBtn->IsClicked() && _spotManager->GetActiveSpotCount() < SpotManager_MaxSpotCount - 1)
		{
			_spotManager->IncreaseActiveSpotCount();

			stateChanged = true;
		}
		break;

	case ControllerMode::SpotSettings:
		if (_previousSpotBtn->IsClicked())
		{
			_spotManager->PreviousSpot();

			if (_spotManager->GetCurrentSpotIndex() == -1)
			{
				_mode = ControllerMode::GlobalSettings;
			}

			stateChanged = true;
		}
		else if (_nextSpotBtn->IsClicked())
		{
			_spotManager->NextSpot();

			stateChanged = true;
		}
		else if (_previousSettingBtn->IsClicked())
		{
			_spotManager->PreviousSetting();

			stateChanged = true;
		}
		else if (_nextSettingBtn->IsClicked())
		{
			_spotManager->NextSetting();

			stateChanged = true;
		}
		else if (_decreaseSettingValueBtn->IsClicked())
		{
			ChangeSettingValue(-1);
			stateChanged = true;
		}
		else if (_increaseSettingValueBtn->IsClicked())
		{
			ChangeSettingValue(1);
			stateChanged = true;
		}
		else if (!_decreaseSettingValueBtn->IsPressed() && !_increaseSettingValueBtn->IsPressed())
		{
			_settingValueDelta = 0;
			_settingValueChangeCounter = 0;
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

void Controller::ChangeSettingValue(int sign)
{
	switch (_spotManager->GetCurrentSetting())
	{
	case SpotSetting::Position:
		_settingValueDelta = 1;
		break;

	case SpotSetting::SpotTime:
	case SpotSetting::TravelTime:
		if (_settingValueChangeCounter == 0)
		{
			_settingValueDelta = 1;
		}
		else if (_settingValueChangeCounter == 10)
		{
			_settingValueDelta = 2;
		}
		else if (_settingValueChangeCounter == 15)
		{
			_settingValueDelta = 5;
		}
		else if (_settingValueChangeCounter == 19)
		{
			_settingValueDelta = 10;
		}
		else if (_settingValueChangeCounter == 25)
		{
			_settingValueDelta = 100;
		}
		break;

	case SpotSetting::SpotActivity:
	case SpotSetting::TravelActivity:
		_settingValueDelta = 1;
		break;
	}

	_spotManager->ChangeSettingValue(_settingValueDelta * sign);

	++_settingValueChangeCounter;
}