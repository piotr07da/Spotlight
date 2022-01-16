#include "Controller.h"

#include <Particle.h>
#include "DiagLed.h"

Controller::Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, SpotManager *spotManager, Motor *motor, Light *light)
	: _previousSpotBtn("prev-spot", previousSpotPin, Button_DebounceDelay_SlowButton),
	  _nextSpotBtn("next-spot", nextSpotPin, Button_DebounceDelay_SlowButton),
	  _previousSettingBtn("prev-sett", previousSettingPin, Button_DebounceDelay_SlowButton),
	  _nextSettingBtn("next-sett", nextSettingPin, Button_DebounceDelay_SlowButton),
	  _decreaseSettingValueBtn("decr-val", decreaseSettingValuePin, Button_DebounceDelay_SlowButton),
	  _increaseSettingValueBtn("incr-val", increaseSettingValuePin, Button_DebounceDelay_SlowButton),
	  _spotManager(spotManager),
	  _motor(motor),
	  _light(light)
{
	_mode = ControllerMode::GlobalSettings;
	_settingValueDelta = 0;
	_settingValueChangeCounter = 0;
}

void Controller::Setup()
{
	_previousSpotBtn.Setup();
	_nextSpotBtn.Setup();
	_previousSettingBtn.Setup();
	_nextSettingBtn.Setup();
	_decreaseSettingValueBtn.Setup();
	_increaseSettingValueBtn.Setup();

	_settingValueDelta = 0;
}

void Controller::Loop()
{
	_previousSpotBtn.Loop();
	_nextSpotBtn.Loop();
	_previousSettingBtn.Loop();
	_nextSettingBtn.Loop();
	_decreaseSettingValueBtn.Loop();
	_increaseSettingValueBtn.Loop();

	switch (_mode)
	{
	case ControllerMode::GlobalSettings:
		if (_nextSpotBtn.IsClicked())
		{
			if (_spotManager->GetSpotCount() > 0)
			{
				ChangeMode(ControllerMode::SpotSettings);
				PositionMotorOnFirstSpot(Motor_MaxSpeed);
				LightUp();
			}
		}
		else if (_decreaseSettingValueBtn.IsClicked() && _spotManager->GetSpotCount() > 0)
		{
			_spotManager->DecreaseActiveSpotCount();
		}
		else if (_increaseSettingValueBtn.IsClicked() && _spotManager->GetSpotCount() < SpotManager_MaxSpotCount - 1)
		{
			_spotManager->IncreaseActiveSpotCount();
		}
		break;

	case ControllerMode::SpotSettings:
		if (_previousSpotBtn.IsClicked())
		{
			if (_spotManager->GetCurrentSpotIndex() > 0)
			{
				_spotManager->PreviousSpot();
				PositionMotorOnCurrentSpot(Motor_MaxSpeed);
			}
			else
			{
				ChangeMode(ControllerMode::GlobalSettings);
				_motor->MoveToWithSpeed(0, Motor_MaxSpeed);
				LightDown();
			}
		}
		else if (_nextSpotBtn.IsClicked())
		{
			if (_spotManager->GetCurrentSpotIndex() < _spotManager->GetSpotCount() - 1)
			{
				_spotManager->NextSpot();
				PositionMotorOnCurrentSpot(Motor_MaxSpeed);
			}
			else
			{
				ChangeMode(ControllerMode::Standby);
				PositionMotorOnFirstSpot(Motor_MaxSpeed);
				LightDown();
			}
		}
		else if (_previousSettingBtn.IsClicked())
		{
			_spotManager->PreviousSetting();
			OnSettingChanged();
		}
		else if (_nextSettingBtn.IsClicked())
		{
			_spotManager->NextSetting();
			OnSettingChanged();
		}
		else if (_decreaseSettingValueBtn.IsClicked())
		{
			ChangeSettingValue(-1);
			if (_spotManager->GetCurrentSetting() == SpotSetting::Position)
				PositionMotorOnCurrentSpot(Controller_ButtonSyncMotorSpeed);
		}
		else if (_increaseSettingValueBtn.IsClicked())
		{
			ChangeSettingValue(1);
			if (_spotManager->GetCurrentSetting() == SpotSetting::Position)
				PositionMotorOnCurrentSpot(Controller_ButtonSyncMotorSpeed);
		}
		else if (_decreaseSettingValueBtn.IsReleased() || _increaseSettingValueBtn.IsReleased())
		{
			ChangeSettingValue(0);
			if (_spotManager->GetCurrentSetting() == SpotSetting::Position)
				PositionMotorOnCurrentSpot(Motor_MaxSpeed);
		}
		break;

	case ControllerMode::Standby:
		if (_previousSpotBtn.IsClicked(true))
		{
			StopRequested.Raise();
			_spotManager->LastSpot();
			PositionMotorOnCurrentSpot(Motor_MaxSpeed);
			LightUp();
			ChangeMode(ControllerMode::SpotSettings);
		}
		else if (_nextSpotBtn.IsClicked())
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
	auto currentSetting = _spotManager->GetCurrentSetting();

	if (sign == 0)
	{
		_settingValueDelta = 0;
		_settingValueChangeCounter = 0;

		_spotManager->ChangeSettingValue(0, currentSetting == SpotSetting::Position);

		return;
	}

	switch (currentSetting)
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

	_spotManager->ChangeSettingValue(_settingValueDelta * sign, currentSetting != SpotSetting::Position);

	++_settingValueChangeCounter;
}

void Controller::PositionMotorOnFirstSpot(int speed)
{
	_motor->MoveToWithSpeed(_spotManager->GetSpotByIndex(0)->Position, speed);
}

void Controller::PositionMotorOnCurrentSpot(int speed)
{
	_motor->MoveToWithSpeed(_spotManager->GetCurrentSpot()->Position, speed);
}

void Controller::LightUp()
{
	_light->SetActivity(LightActivity::A_01, 300);
}

void Controller::LightDown()
{
	_light->SetActivity(LightActivity::A_10, 300);
}

void Controller::ReconfigureButtons()
{
	switch (_mode)
	{
	case ControllerMode::GlobalSettings:
	{
		_previousSpotBtn.ResetDisabled();
		_nextSpotBtn.ResetEnabled();
		_previousSettingBtn.ResetDisabled();
		_nextSettingBtn.ResetDisabled();
		_decreaseSettingValueBtn.ResetEnabled();
		_increaseSettingValueBtn.ResetEnabled();
		_decreaseSettingValueBtn.ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
		_increaseSettingValueBtn.ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
		break;
	}

	case ControllerMode::SpotSettings:
	{
		_previousSpotBtn.ResetEnabled();
		_nextSpotBtn.ResetEnabled();
		_previousSettingBtn.ResetEnabled();
		_nextSettingBtn.ResetEnabled();
		_decreaseSettingValueBtn.ResetEnabled();
		_increaseSettingValueBtn.ResetEnabled();
		SpotSetting setting = _spotManager->GetCurrentSetting();
		switch (setting)
		{
		case SpotSetting::Position:
			_decreaseSettingValueBtn.ChangeDebounceDelay(100);
			_increaseSettingValueBtn.ChangeDebounceDelay(100);
			break;
		case SpotSetting::SpotTime:
		case SpotSetting::TravelTime:
			_decreaseSettingValueBtn.ChangeDebounceDelay(Button_DebounceDelay_FastButton);
			_increaseSettingValueBtn.ChangeDebounceDelay(Button_DebounceDelay_FastButton);
			break;
		default:
			_decreaseSettingValueBtn.ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
			_increaseSettingValueBtn.ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
			break;
		}
		break;
	}

	case ControllerMode::Standby:
	{
		_previousSpotBtn.ResetEnabled();
		_nextSpotBtn.ResetEnabled();
		_previousSettingBtn.ResetDisabled();
		_nextSettingBtn.ResetDisabled();
		_decreaseSettingValueBtn.ResetDisabled();
		_increaseSettingValueBtn.ResetDisabled();
		break;
	}
	}
}