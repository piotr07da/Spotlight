#include "Runner.h"

#include "Spot.h"

Runner::Runner(SpotManager *spotManager, Motor *motor, Light *light)
{
	_spotManager = spotManager;
	_motor = motor;
	_light = light;
}

void Runner::Setup()
{
}

void Runner::Loop()
{
	if (!_isRunning)
	{
		return;
	}

	Spot spot = *_spotManager->GetSpotByIndex(_spotIndex);
	auto t = millis() - _t0;
	if (_spotStage == RunnerSpotStage::Travel && t > spot.TravelTime)
	{
		_t0 = millis();
		_spotStage = RunnerSpotStage::Spot;
		_setupStage = true;
	}
	else if (_spotStage == RunnerSpotStage::Spot && t > spot.SpotTime)
	{
		_t0 = millis();
		_spotStage = RunnerSpotStage::Travel;
		++_spotIndex;
		if (_spotIndex == _spotManager->GetActiveSpotCount())
		{
			_spotIndex = 0;
		}
		spot = *_spotManager->GetSpotByIndex(_spotIndex);
		_setupStage = true;
	}

	int duration;
	LightActivity activity;

	switch (_spotStage)
	{
	case RunnerSpotStage::Travel:
		duration = spot.TravelTime;
		activity = spot.TravelActivity;
		break;

	case RunnerSpotStage::Spot:
		duration = spot.SpotTime;
		activity = spot.SpotActivity;
		break;
	}

	if (_setupStage)
	{
		_motor->MoveToInTime(spot.Position, duration);
		_light->SetActivity(activity, duration);
		_setupStage = false;
	}
}

void Runner::OnStartRequested()
{
	_isRunning = true;
	_t0 = millis();
	_spotIndex = 0;
	_spotStage = RunnerSpotStage::Spot;
	_setupStage = true;
}

void Runner::OnStopRequested()
{
	_isRunning = false;
	_light->SetActivity(LightActivity::A_0, 1);
}