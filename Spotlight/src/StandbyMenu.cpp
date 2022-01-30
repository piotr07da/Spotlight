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
		Stop();
	}
	else if (_startButton->IsClicked())
	{
		Start();
	}
	else if (_audioTrigger->IsTriggered())
	{
		Start();
	}
}

bool StandbyMenu::IsActive()
{
	return _isActive;
}

void StandbyMenu::Activate()
{
	_stopButton->ResetEnabled();
	_startButton->ResetEnabled();

	_stopButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_startButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);

	Standby();

	_isActive = true;
}

void StandbyMenu::Deactivate()
{
	_audioTrigger->Disable();
	_runner->Stop();

	_isActive = false;
}

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
	_audioTrigger->Enable();
	_display->ShowStandby();
	_motor->MoveToWithSpeed(_spots->GetFirst()->Position, Motor_MaxSpeed);
}