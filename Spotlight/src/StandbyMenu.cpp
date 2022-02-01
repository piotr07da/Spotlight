#include "StandbyMenu.h"

StandbyMenu::StandbyMenu(Button *stopButton, Button *startButton, AudioTrigger *audioTrigger, Display *display, Motor *motor, Light *light, Runner *runner, SpotCollection *spots)
	: _stopButton(stopButton),
	  _startButton(startButton),
	  _audioTrigger(audioTrigger),
	  _display(display),
	  _motor(motor),
	  _light(light),
	  _runner(runner),
	  _spots(spots)
{
}

void StandbyMenu::Loop()
{
	if (!_isActive)
	{
		return;
	}

	if (_stopButton->IsClicked())
	{
		if (_runner->IsRunning())
		{
			Stop();
		}
		else
		{
			Deactivate();
			_masterMenuActivator->Activate();
		}
	}
	else if (_spots->GetCount() > 0)
	{
		if (_startButton->IsClicked())
		{
			Start();
		}
		else if (_audioTrigger->IsTriggered())
		{
			Start();
		}
	}
}

void StandbyMenu::AssingMasterMenuActivator(MasterMenuActivator *masterMenuActivator)
{
	_masterMenuActivator = masterMenuActivator;
}

bool StandbyMenu::IsActive()
{
	return _isActive;
}

void StandbyMenu::Activate()
{
	_stopButton->ResetEnabled(Button_DebounceDelay_SlowButton);
	_startButton->ResetEnabled(Button_DebounceDelay_SlowButton);

	Standby();

	_light->LightDownGentle();

	_isActive = true;
}

void StandbyMenu::Deactivate()
{
	_audioTrigger->Disable();
	_runner->Stop();

	_isActive = false;
}

// PRIVATE METHODS

void StandbyMenu::Start()
{
	_audioTrigger->Disable();
	_display->ShowRunning();
	_runner->Start();
}

void StandbyMenu::Stop()
{
	_runner->Stop();
	Standby();
}

void StandbyMenu::Standby()
{
	_display->ShowStandby(_spots);
	if (_spots->GetCount() > 0)
	{
		_audioTrigger->Enable();
		_motor->MoveToWithSpeed(_spots->GetFirst()->Position, Motor_MaxSpeed);
	}
}