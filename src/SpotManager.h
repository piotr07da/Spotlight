#ifndef SPOT_MANAGER_H
#define SPOT_MANAGER_H

#include "Spot.h"

const int SpotManager_MaxSpotCount = 32;

class SpotManager
{
private:
	Spot _spots[SpotManager_MaxSpotCount];
	int _activeSpotCount;
	int _currentSpotIndex;

public:
	int GetActiveSpotCount();
	int GetCurrentSpotIndex();
	void DecreaseActiveSpotCount();
	void IncreaseActiveSpotCount();
	void PreviousSpot();
	void NextSpot();
	void PreviousSetting();
	void NextSetting();
	void DecreaseSettingValue(int value);
	void IncreaseSettingValue(int value);
};

#endif // SPOT_MANAGER_H