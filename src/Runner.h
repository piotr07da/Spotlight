#ifndef RUNNER_H
#define RUNNER_H

#include "SpotManager.h"
#include "Event.h"

class Runner
{
private:
	SpotManager *_spotManager;

public:
	Runner(SpotManager *spotManager);

	void OnStartRequested();
	void OnStopRequested();
};

#endif // RUNNER_H