#ifndef RUNNER_H
#define RUNNER_H

#include "SpotCollection.h"
#include "Motor.h"
#include "Light.h"

#include <Particle.h>

enum class RunnerSpotStage
{
	Travel,
	Spot,
};

class Runner
{
private:
	SpotCollection *_spots;
	Motor *_motor;
	Light *_light;
	bool _isRunning;
	ulong _t0;
	int _spotIndex;
	RunnerSpotStage _spotStage;
	bool _setupStage;

	int CalculateCurrentSpotTravelTime();

public:
	Runner(SpotCollection *spots, Motor *motor, Light *light);

	void Setup();
	void Loop();

	void Start();
	void Stop();
};

#endif // RUNNER_H