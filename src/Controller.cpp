#include "Controller.h"

#include <Particle.h>
#include "DiagLed.h"

Controller::Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, SpotManager *spotManager, Motor *motor)
{
	_previousSpotBtn = new Button("prev-spot", previousSpotPin, Button_DebounceDelay_SlowButton);
	_nextSpotBtn = new Button("next-spot", nextSpotPin, Button_DebounceDelay_SlowButton);
	_previousSettingBtn = new Button("prev-sett", previousSettingPin, Button_DebounceDelay_SlowButton);
	_nextSettingBtn = new Button("next-sett", nextSettingPin, Button_DebounceDelay_SlowButton);
	_decreaseSettingValueBtn = new Button("decr-val", decreaseSettingValuePin, Button_DebounceDelay_SlowButton);
	_increaseSettingValueBtn = new Button("incr-val", increaseSettingValuePin, Button_DebounceDelay_SlowButton);
	_spotManager = spotManager;
	_motor = motor;
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

	switch (_mode)
	{
	case ControllerMode::GlobalSettings:
		if (_nextSpotBtn->IsClicked())
		{
			if (_spotManager->GetActiveSpotCount() > 0)
			{
				ChangeMode(ControllerMode::SpotSettings);
				PositionMotorOnFirstSpot(Motor_MaxSpeed, "GS-next-spot");
			}
		}
		else if (_decreaseSettingValueBtn->IsClicked() && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->DecreaseActiveSpotCount();
		}
		else if (_increaseSettingValueBtn->IsClicked() && _spotManager->GetActiveSpotCount() < SpotManager_MaxSpotCount - 1)
		{
			_spotManager->IncreaseActiveSpotCount();
		}
		break;

	case ControllerMode::SpotSettings:
		if (_previousSpotBtn->IsClicked())
		{
			if (_spotManager->GetCurrentSpotIndex() > 0)
			{
				_spotManager->PreviousSpot();
				PositionMotorOnCurrentSpot(Motor_MaxSpeed, "SS-prev-spot[currSpotIx not first]");
			}
			else
			{
				ChangeMode(ControllerMode::GlobalSettings);
				_motor->MoveToWithSpeed(0, Motor_MaxSpeed, "SS-prev-spot[currSpotIx first]");
			}
		}
		else if (_nextSpotBtn->IsClicked())
		{
			if (_spotManager->GetCurrentSpotIndex() < _spotManager->GetActiveSpotCount() - 1)
			{
				_spotManager->NextSpot();
				PositionMotorOnCurrentSpot(Motor_MaxSpeed, "SS-next-spot[currSpotIx not last");
			}
			else
			{
				ChangeMode(ControllerMode::Standby);
				PositionMotorOnFirstSpot(Motor_MaxSpeed, "SS-next-spot[currSpotIx last]");
			}
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
			if (_spotManager->GetCurrentSetting() == SpotSetting::Position)
				PositionMotorOnCurrentSpot(Controller_ButtonSyncMotorSpeed, "SS-decr-val");
		}
		else if (_increaseSettingValueBtn->IsClicked())
		{
			ChangeSettingValue(1);
			if (_spotManager->GetCurrentSetting() == SpotSetting::Position)
				PositionMotorOnCurrentSpot(Controller_ButtonSyncMotorSpeed, "SS-incr-val");
		}
		else if (_decreaseSettingValueBtn->IsReleased() || _increaseSettingValueBtn->IsReleased())
		{
			_settingValueDelta = 0;
			_settingValueChangeCounter = 0;

			if (_spotManager->GetCurrentSetting() == SpotSetting::Position)
				PositionMotorOnCurrentSpot(Motor_MaxSpeed, "SS-decr-incr-val-not-pressed");
		}
		break;

	case ControllerMode::Standby:
		if (_previousSpotBtn->IsClicked(true))
		{
			StopRequested.Raise();
			ChangeMode(ControllerMode::SpotSettings);
		}
		else if (_nextSpotBtn->IsClicked())
		{
			StartRequested.Raise();
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
	ReconfigureButtons();
}

void Controller::OnSettingChanged()
{
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
}

void Controller::PositionMotorOnFirstSpot(int speed, String diag)
{
	_motor->MoveToWithSpeed(_spotManager->GetSpotByIndex(0)->Position, speed, diag);
}

void Controller::PositionMotorOnCurrentSpot(int speed, String diag)
{
	_motor->MoveToWithSpeed(_spotManager->GetCurrentSpot()->Position, speed, diag);
}

void Controller::ReconfigureButtons()
{
	switch (_mode)
	{
	case ControllerMode::GlobalSettings:
	{
		_previousSpotBtn->Disable();
		_nextSpotBtn->Enable();
		_previousSettingBtn->Disable();
		_nextSettingBtn->Disable();
		_decreaseSettingValueBtn->Enable();
		_increaseSettingValueBtn->Enable();
		_decreaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
		_increaseSettingValueBtn->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
		break;
	}

	case ControllerMode::SpotSettings:
	{
		_previousSpotBtn->Enable();
		_nextSpotBtn->Enable();
		_previousSettingBtn->Enable();
		_nextSettingBtn->Enable();
		_decreaseSettingValueBtn->Enable();
		_increaseSettingValueBtn->Enable();
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

	case ControllerMode::Standby:
	{
		_previousSpotBtn->Enable();
		_nextSpotBtn->Enable();
		_previousSettingBtn->Disable();
		_nextSettingBtn->Disable();
		_decreaseSettingValueBtn->Disable();
		_increaseSettingValueBtn->Disable();
		break;
	}
	}
}