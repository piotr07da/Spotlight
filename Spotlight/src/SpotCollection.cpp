#include "SpotCollection.h"

void SpotCollection::Add(int position, int spotTime, LightActivity spotActivity, int travelTime, LightActivity travelActivity)
{
	if (_spotCount < SpotCollection_MaxSpotCount)
	{
		Spot *spot = _spots + _spotCount;
		spot->Index = _spotCount;
		spot->Position = position;
		spot->SpotTime = spotTime;
		spot->SpotActivity = spotActivity;
		spot->TravelTime = travelTime;
		spot->TravelActivity = travelActivity;
		++_spotCount;
	}
}

void SpotCollection::RemoveLast()
{
	if (_spotCount > 0)
	{
		--_spotCount;
	}
}

int SpotCollection::GetCount()
{
	return _spotCount;
}

Spot *SpotCollection::GetByIndex(int index)
{
	return _spots + index;
}
