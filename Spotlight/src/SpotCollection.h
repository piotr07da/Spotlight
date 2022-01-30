#ifndef SPOT_COLLECTION_H
#define SPOT_COLLECTION_H

#include "Spot.h"

const int SpotCollection_MaxSpotCount = 32;

class SpotCollection
{
private:
	Spot _spots[SpotCollection_MaxSpotCount];
	int _spotCount;

public:
	Spot *GetByIndex(int index);
};

#endif // SPOT_COLLECTION_H
