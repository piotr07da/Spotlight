#include "Runner.h"

#include <Particle.h>
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
	int travelTime = CalculateCurrentSpotTravelTime();

	auto t = millis() - _t0;
	if (_spotStage == RunnerSpotStage::Travel && t > (ulong)travelTime)
	{
		_t0 = millis();
		_spotStage = RunnerSpotStage::Spot;
		_setupStage = true;
	}
	else if (_spotStage == RunnerSpotStage::Spot && t > (ulong)spot.SpotTime)
	{
		_t0 = millis();
		_spotStage = RunnerSpotStage::Travel;
		++_spotIndex;
		if (_spotIndex == _spotManager->GetSpotCount())
		{
			_spotIndex = 0;
		}
		spot = *_spotManager->GetSpotByIndex(_spotIndex);
		_setupStage = true;
	}

	int duration = 0;
	LightActivity activity = LightActivity::A_0;

	switch (_spotStage)
	{
	case RunnerSpotStage::Travel:
	{
		duration = CalculateCurrentSpotTravelTime();
		activity = spot.TravelActivity;
		break;
	}

	case RunnerSpotStage::Spot:
	{
		duration = spot.SpotTime;
		activity = spot.SpotActivity;
		break;
	}
	}

	if (_setupStage)
	{
		Particle.publish("diag", "_setupStage spotCnt:" + String(_spotManager->GetSpotCount()));
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

int Runner::CalculateCurrentSpotTravelTime()
{
	int previousSpotIndex = _spotIndex - 1;
	if (previousSpotIndex == -1)
	{
		previousSpotIndex = _spotManager->GetSpotCount() - 1;
	}

	Spot currentSpot = *_spotManager->GetSpotByIndex(_spotIndex);
	Spot previousSpot = *_spotManager->GetSpotByIndex(previousSpotIndex);

	auto distance = abs(currentSpot.Position - previousSpot.Position);
	auto minEngineAllowedTime = (int)(distance * Motor_MinStepInterval / 1000.0f);

	auto travelTime = max(currentSpot.TravelTime, minEngineAllowedTime);

	return travelTime;
}