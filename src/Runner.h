#ifndef RUNNER_H
#define RUNNER_H

#include "SpotManager.h"
#include "Motor.h"
#include "Light.h"
#include "Event.h"

enum class RunnerSpotStage
{
	Travel,
	Spot,
};

class Runner
{
private:
	SpotManager *_spotManager;
	Motor *_motor;
	Light *_light;
	bool _isRunning;
	int _t0;
	int _spotIndex;
	RunnerSpotStage _spotStage;
	bool _setupStage;

public:
	Runner(SpotManager *spotManager, Motor *motor, Light *light);

	void Setup();
	void Loop();

	void OnStartRequested();
	void OnStopRequested();
};

#endif // RUNNER_H