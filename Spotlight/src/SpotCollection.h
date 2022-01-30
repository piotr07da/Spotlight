#ifndef SPOT_COLLECTION_H
#define SPOT_COLLECTION_H

#include "Spot.h"
#include "LightActivity.h"

const int SpotCollection_MaxSpotCount = 32;

class SpotCollection
{
private:
	Spot _spots[SpotCollection_MaxSpotCount];
	int _spotCount;

public:
	void Add(int position, int spotTime, LightActivity spotActivity, int travelTime, LightActivity travelActivity);
	void RemoveLast();
	int GetCount();
	Spot *GetByIndex(int index);
};

#endif // SPOT_COLLECTION_H
