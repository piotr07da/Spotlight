#ifndef SPOT_H
#define SPOT_H

#include "LightActivity.h"

struct Spot
{
	int Index;
	int Position;
	int SpotTime;
	LightActivity SpotActivity;
	int TravelTime;
	LightActivity TravelActivity;
};

#endif // SPOT_H
