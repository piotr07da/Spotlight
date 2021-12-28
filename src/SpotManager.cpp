#include "SpotManager.h"

#include "Particle.h"

LightActivity SpotManager::_spotActivites[6] = {
	LightActivity::A_1,
	LightActivity::A_010,
	LightActivity::A_01,
	LightActivity::A_10,
	LightActivity::A_0,
	LightActivity::A_101,
};
int SpotManager::_spotActivitiesCount = 6;
LightActivity SpotManager::_travelActivites[6] = {
	LightActivity::A_0,
	LightActivity::A_101,
	LightActivity::A_10,
	LightActivity::A_01,
	LightActivity::A_1,
	LightActivity::A_010,
};
int SpotManager::_travelActivitiesCount = 6;

SpotManager::SpotManager()
{
	_activeSpotCount = 0;
	_currentSpotIndex = -1;
	_currentSetting = SpotSetting::Position;
}

int SpotManager::GetActiveSpotCount()
{
	return _activeSpotCount;
}

int SpotManager::GetCurrentSpotIndex()
{
	return _currentSpotIndex;
}

Spot *SpotManager::GetCurrentSpot()
{
	return _spots + _currentSpotIndex;
}

SpotSetting SpotManager::GetCurrentSetting()
{
	return _currentSetting;
}

void SpotManager::DecreaseActiveSpotCount()
{
	if (_activeSpotCount > 0)
	{
		--_activeSpotCount;
	}

	Particle.publish("diag", "Active spot count: " + String(_activeSpotCount));
}

void SpotManager::IncreaseActiveSpotCount()
{
	if (_activeSpotCount < SpotManager_MaxSpotCount - 1)
	{
		++_activeSpotCount;
		Spot *addedSpot = _spots + _activeSpotCount - 1;
		addedSpot->Position = 0;
		addedSpot->SpotTime = 1000;
		addedSpot->SpotActivity = LightActivity::A_1;
		addedSpot->TravelTime = 200;
		addedSpot->TravelActivity = LightActivity::A_0;
	}

	Particle.publish("diag", "Active spot count: " + String(_activeSpotCount));
}

void SpotManager::PreviousSpot()
{
	if (_currentSpotIndex >= 0)
	{
		--_currentSpotIndex;
	}

	Particle.publish("diag", "Current spot index: " + String(_currentSpotIndex));
}

void SpotManager::NextSpot()
{
	if (_currentSpotIndex < _activeSpotCount - 1)
	{
		++_currentSpotIndex;
	}

	Particle.publish("diag", "Current spot index: " + String(_currentSpotIndex));
}

void SpotManager::PreviousSetting()
{
	if (_currentSetting > SpotSetting::FIRST)
	{
		_currentSetting = (SpotSetting)(_currentSetting - 1);
	}
	else
	{
		_currentSetting = SpotSetting::LAST;
	}
}

void SpotManager::NextSetting()
{
	if (_currentSetting < SpotSetting::LAST)
	{
		_currentSetting = (SpotSetting)(_currentSetting + 1);
	}
	else
	{
		_currentSetting = SpotSetting::FIRST;
	}
}

void SpotManager::DecreaseSettingValue(int value)
{
	IncreaseSettingValue(-value);
}

void SpotManager::IncreaseSettingValue(int value)
{
	Spot *spot = GetCurrentSpot();

	switch (_currentSetting)
	{
	case SpotSetting::Position:
	{
		spot->Position += value;
		break;
	}

	case SpotSetting::SpotTime:
	{
		spot->SpotTime += value;
		break;
	}

	case SpotSetting::SpotActivity:
	{
		int spotActivityIndex = GetCurrentSpotActivityIndex();
		spot->SpotActivity = FindActivity(SpotManager::_spotActivites, SpotManager::_spotActivitiesCount, spotActivityIndex, value);
		break;
	}

	case SpotSetting::TravelTime:
	{
		spot->TravelTime += value;
		break;
	}

	case SpotSetting::TravelActivity:
	{
		int travelActivityIndex = GetCurrentTravelActivityIndex();
		spot->TravelActivity = FindActivity(SpotManager::_travelActivites, SpotManager::_travelActivitiesCount, travelActivityIndex, value);
		break;
	}
	}
}

LightActivity SpotManager::FindActivity(LightActivity *activities, int activitiesCount, int currentActivityIndex, int indexChangeValue)
{
	if (indexChangeValue > 0)
	{
		indexChangeValue = 1;
	}
	else if (indexChangeValue < 0)
	{
		indexChangeValue = -1;
	}
	else
	{
		return activities[currentActivityIndex];
	}

	currentActivityIndex += indexChangeValue;

	if (currentActivityIndex >= activitiesCount)
	{
		currentActivityIndex = 0;
	}
	else if (currentActivityIndex < 0)
	{
		currentActivityIndex = activitiesCount - 1;
	}

	return activities[currentActivityIndex];
}

int SpotManager::GetCurrentSpotActivityIndex()
{
	Spot spot = *GetCurrentSpot();
	for (int i = 0; i < SpotManager::_spotActivitiesCount; ++i)
	{
		if (SpotManager::_spotActivites[i] == spot.SpotActivity)
		{
			return i;
		}
	}
	return -1;
}

int SpotManager::GetCurrentTravelActivityIndex()
{
	Spot spot = *GetCurrentSpot();
	for (int i = 0; i < SpotManager::_travelActivitiesCount; ++i)
	{
		if (SpotManager::_travelActivites[i] == spot.TravelActivity)
		{
			return i;
		}
	}
	return -1;
}
