#ifndef SPOT_MANAGER_H
#define SPOT_MANAGER_H

#include "LightActivity.h"
#include "Spot.h"
#include "SpotSetting.h"

const int SpotManager_MaxSpotCount = 32;

class SpotManager
{
private:
	static LightActivity _spotActivites[6];
	static int _spotActivitiesCount;
	static LightActivity _travelActivites[6];
	static int _travelActivitiesCount;

	Spot _spots[SpotManager_MaxSpotCount];
	int _activeSpotCount;
	int _currentSpotIndex;
	SpotSetting _currentSetting;

	LightActivity FindActivity(LightActivity *activities, int activitiesCount, int currentActivityIndex, int indexChangeValue);
	int GetCurrentSpotActivityIndex();
	int GetCurrentTravelActivityIndex();

public:
	SpotManager();
	int GetActiveSpotCount();
	int GetCurrentSpotIndex();
	Spot *GetCurrentSpot();
	SpotSetting GetCurrentSetting();
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