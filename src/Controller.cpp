#include "Controller.h"

#include "Particle.h"

Controller::Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, SpotManager *spotManager)
{
	_previousSpotBtn = new Button(previousSpotPin, Button_DebounceDelay_SlowButton);
	_nextSpotBtn = new Button(nextSpotPin, Button_DebounceDelay_SlowButton);
	_previousSettingBtn = new Button(previousSettingPin, Button_DebounceDelay_SlowButton);
	_nextSettingBtn = new Button(nextSettingPin, Button_DebounceDelay_SlowButton);
	_decreaseSettingValueBtn = new Button(decreaseSettingValuePin, Button_DebounceDelay_SlowButton);
	_increaseSettingValueBtn = new Button(increaseSettingValuePin, Button_DebounceDelay_SlowButton);
	//_motor = motor;
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

	//_motor->Setup();

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

	//_motor->Loop();

	switch (_mode)
	{
	case ControllerMode::GlobalSettings:
		if (_previousSpotBtn->IsClicked(true))
		{
			// nothing to do but we want to reset click status on this button
		}
		else if (_nextSpotBtn->IsClicked() && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->NextSpot();
			ChangeMode(ControllerMode::SpotSettings);
		}
		else if (_decreaseSettingValueBtn->IsClicked() && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->DecreaseActiveSpotCount();
			// RefreshDisplay();
		}
		else if (_increaseSettingValueBtn->IsClicked() && _spotManager->GetActiveSpotCount() < SpotManager_MaxSpotCount - 1)
		{
			_spotManager->IncreaseActiveSpotCount();
			// RefreshDisplay();
		}
		break;

	case ControllerMode::SpotSettings:
		if (_previousSpotBtn->IsClicked())
		{
			_spotManager->PreviousSpot();

			if (_spotManager->GetCurrentSpotIndex() == -1)
			{
				ChangeMode(ControllerMode::GlobalSettings);
			}
			else
			{
				RefreshDisplay();
			}
		}
		else if (_nextSpotBtn->IsClicked())
		{
			_spotManager->NextSpot();
			RefreshDisplay();
			PositionMotor(Controller_MaxMotorSpeed);
		}
		else if (_previousSettingBtn->IsClicked())
		{
			_spotManager->PreviousSetting();
			OnSettingChanged();
		}
		else if (_nextSettingBtn->IsClicked())
		{
			_spotManager->NextSetting();
			OnSettingChanged();
		}
		else if (_decreaseSettingValueBtn->IsClicked())
		{
			ChangeSettingValue(-1);
			PositionMotor(Controller_ButtonSyncMotorSpeed);
		}
		else if (_increaseSettingValueBtn->IsClicked())
		{
			ChangeSettingValue(1);
			PositionMotor(Controller_ButtonSyncMotorSpeed);
		}
		else if (!_decreaseSettingValueBtn->IsPressed() && !_increaseSettingValueBtn->IsPressed())
		{
			_settingValueDelta = 0;
			_settingValueChangeCounter = 0;

			PositionMotor(Controller_MaxMotorSpeed);
		}

		break;
	}
}

void Controller::ChangeMode(ControllerMode mode)
{
	_mode = mode;
	ModeChanged.Raise(mode);
	OnModeChanged();
}

void Controller::OnModeChanged()
{
	RefreshDisplay();
	ReconfigureButtons();
}

void Controller::OnSettingChanged()
{
	RefreshDisplay();
	ReconfigureButtons();
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

	OnSettingValueChanged();
}

void Controller::OnSettingValueChanged()
{
	RefreshDisplay();
}

void Controller::PositionMotor(int speed)
{
	if (_spotManager->GetCurrentSetting() == SpotSetting::Position)
	{
		//_motor->MoveToWithSpeed(_spotManager->GetCurrentSpot()->Position, speed);
	}
}

void Controller::RefreshDisplay()
{
	switch (_mode)
	{
	case ControllerMode::GlobalSettings:
		//_display->ShowGlobalSettings(_spotManager->GetActiveSpotCount());
		break;
	case ControllerMode::SpotSettings:
		//_display->ShowSpotSetting(_spotManager->GetCurrentSpotIndex(), *_spotManager->GetCurrentSpot(), _spotManager->GetCurrentSetting());
		break;
	}
}

void Controller::ReconfigureButtons()
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
			_decreaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SuperFastButton);
			_increaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SuperFastButton);
			break;
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
}