#ifndef SPOT_MANAGER_H
#define SPOT_MANAGER_H

#include "LightActivity.h"
#include "Spot.h"
#include "SpotSetting.h"
#include "Event.h"

const int SpotManager_MaxSpotCount = 32;

class SpotManager
{
private:
	static LightActivity _spotActivites[7];
	static int _spotActivitiesCount;
	static LightActivity _travelActivites[7];
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

	Event0 NumberOfSpotsChanged;
	Event0 SpotChanged;
	Event1<SpotSetting> SettingChanged;
	Event0 SettingValueChanged;

	int GetActiveSpotCount();
	int GetCurrentSpotIndex();
	Spot *GetCurrentSpot();
	Spot *GetSpotByIndex(int index);
	SpotSetting GetCurrentSetting();
	void DecreaseActiveSpotCount();
	void IncreaseActiveSpotCount();
	void PreviousSpot();
	void NextSpot();
	void FirstSpot();
	void LastSpot();
	void PreviousSetting();
	void NextSetting();
	void ChangeSettingValue(int delta);
};

#endif // SPOT_MANAGER_H