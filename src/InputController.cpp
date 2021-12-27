#include "InputController.h"

#include "Particle.h"

InputController::InputController(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, int settingValueSensitivityPin, SpotManager *spotManager)
{
	_previousSpotBtn = Button(previousSpotPin);
	_nextSpotBtn = Button(nextSpotPin);
	_previousSettingBtn = Button(previousSettingPin);
	_nextSettingBtn = Button(nextSettingPin);
	_decreaseSettingValueBtn = Button(decreaseSettingValuePin);
	_increaseSettingValueBtn = Button(increaseSettingValuePin);
	_settingValueSensitivityPin = settingValueSensitivityPin;
	_spotManager = spotManager;

	_mode = InputControllerMode::GlobalSettings;
}

InputController::~InputController()
{
}

void InputController::Setup()
{
	_previousSpotBtn.Setup();
	_nextSpotBtn.Setup();
	_previousSettingBtn.Setup();
	_nextSettingBtn.Setup();
	_decreaseSettingValueBtn.Setup();
	_increaseSettingValueBtn.Setup();
	pinMode(_settingValueSensitivityPin, PinMode::INPUT);
}

void InputController::Loop()
{
	_previousSpotBtn.Loop();
	_nextSpotBtn.Loop();
	_previousSettingBtn.Loop();
	_nextSettingBtn.Loop();
	_decreaseSettingValueBtn.Loop();
	_increaseSettingValueBtn.Loop();

	switch (_mode)
	{
	case InputControllerMode::GlobalSettings:
		if (_nextSpotBtn.GetState())
		{
			_spotManager->NextSpot();

			_mode = InputControllerMode::SpotSettings;
		}
		else if (_decreaseSettingValueBtn.GetState() && _spotManager->GetActiveSpotCount() > 0)
		{
			_spotManager->DecreaseActiveSpotCount();
		}
		else if (_increaseSettingValueBtn.GetState() && _spotManager->GetActiveSpotCount() < SpotManager_MaxSpotCount - 1)
		{
			_spotManager->IncreaseActiveSpotCount();
		}
		break;

	case InputControllerMode::SpotSettings:
		if (_previousSpotBtn.GetState())
		{
			_spotManager->PreviousSpot();

			if (_spotManager->GetCurrentSpotIndex() == 0)
			{
				_mode = InputControllerMode::GlobalSettings;
			}
		}
		else if (_nextSpotBtn.GetState())
		{
			_spotManager->NextSpot();
		}
		else if (_previousSettingBtn.GetState())
		{
			_spotManager->PreviousSetting();
		}
		else if (_nextSettingBtn.GetState())
		{
			_spotManager->NextSetting();
		}
		else if (_decreaseSettingValueBtn.GetState())
		{
			_spotManager->DecreaseSettingValue(1);
		}
		else if (_increaseSettingValueBtn.GetState())
		{
			_spotManager->IncreaseSettingValue(1);
		}
		break;
	}
}
