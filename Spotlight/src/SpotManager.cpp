#include "SpotManager.h"

#include <Particle.h>

LightActivity SpotManager::_spotActivites[7] = {
	LightActivity::A_1,
	LightActivity::A_010,
	LightActivity::A_01,
	LightActivity::A_10,
	LightActivity::A_0,
	LightActivity::A_101,
	LightActivity::A_Strobe,
};
int SpotManager::_spotActivitiesCount = 7;
LightActivity SpotManager::_travelActivites[7] = {
	LightActivity::A_0,
	LightActivity::A_101,
	LightActivity::A_10,
	LightActivity::A_01,
	LightActivity::A_1,
	LightActivity::A_010,
	LightActivity::A_Strobe,
};
int SpotManager::_travelActivitiesCount = 7;

SpotManager::SpotManager()
{
	_spotCount = 0;
	_currentSpotIndex = -1;
	_currentSetting = SpotSetting::Position;
}

int SpotManager::GetSpotCount()
{
	return _spotCount;
}

int SpotManager::GetCurrentSpotIndex()
{
	return _currentSpotIndex;
}

Spot *SpotManager::GetCurrentSpot()
{
	return _spots + _currentSpotIndex;
}

Spot *SpotManager::GetSpotByIndex(int index)
{
	return _spots + index;
}

SpotSetting SpotManager::GetCurrentSetting()
{
	return _currentSetting;
}

void SpotManager::DecreaseActiveSpotCount()
{
	if (_spotCount > 0)
	{
		--_spotCount;
		if (_spotCount == 0)
		{
			_currentSpotIndex = -1;
		}

		NumberOfSpotsChanged.Raise();
	}
}

void SpotManager::IncreaseActiveSpotCount()
{
	if (_spotCount < SpotManager_MaxSpotCount - 1)
	{
		Spot *addedSpot = _spots + _spotCount;
		addedSpot->Position = 0;
		addedSpot->SpotTime = 1000;
		addedSpot->SpotActivity = LightActivity::A_1;
		addedSpot->TravelTime = 200;
		addedSpot->TravelActivity = LightActivity::A_0;

		++_spotCount;
		if (_currentSpotIndex == -1)
		{
			_currentSpotIndex = 0;
		}

		NumberOfSpotsChanged.Raise();
	}
}

void SpotManager::PreviousSpot()
{
	if (_currentSpotIndex > 0)
	{
		--_currentSpotIndex;
		SpotChanged.Raise();

		if (_currentSetting != SpotSetting::Position)
		{
			_currentSetting = SpotSetting::Position;
			SettingChanged.Raise(_currentSetting);
		}
	}
}

void SpotManager::NextSpot()
{
	if (_currentSpotIndex < _spotCount - 1)
	{
		++_currentSpotIndex;
		SpotChanged.Raise();

		if (_currentSetting != SpotSetting::Position)
		{
			_currentSetting = SpotSetting::Position;
			SettingChanged.Raise(_currentSetting);
		}
	}
}

void SpotManager::FirstSpot()
{
	if (_spotCount == 0)
	{
		return;
	}

	_currentSpotIndex = 0;
	_currentSetting = SpotSetting::Position;
}

void SpotManager::LastSpot()
{
	if (_spotCount == 0)
	{
		return;
	}

	_currentSpotIndex = _spotCount - 1;
	_currentSetting = SpotSetting::Position;
}

void SpotManager::PreviousSetting()
{
	if (_currentSetting > SpotSetting::FIRST)
	{
		_currentSetting = (SpotSetting)((int)_currentSetting - 1);
	}
	else
	{
		_currentSetting = SpotSetting::LAST;
	}

	SettingChanged.Raise(_currentSetting);
}

void SpotManager::NextSetting()
{
	if (_currentSetting < SpotSetting::LAST)
	{
		_currentSetting = (SpotSetting)((int)_currentSetting + 1);
	}
	else
	{
		_currentSetting = SpotSetting::FIRST;
	}

	SettingChanged.Raise(_currentSetting);
}

void SpotManager::ChangeSettingValue(int delta, bool notify)
{
	Spot *spot = GetCurrentSpot();

	switch (_currentSetting)
	{
	case SpotSetting::Position:
	{
		spot->Position += delta;
		break;
	}

	case SpotSetting::SpotTime:
	{
		spot->SpotTime += delta;
		if (spot->SpotTime < 0)
		{
			spot->SpotTime = 0;
		}
		break;
	}

	case SpotSetting::SpotActivity:
	{
		int spotActivityIndex = GetCurrentSpotActivityIndex();
		spot->SpotActivity = FindActivity(SpotManager::_spotActivites, SpotManager::_spotActivitiesCount, spotActivityIndex, delta);
		break;
	}

	case SpotSetting::TravelTime:
	{
		spot->TravelTime += delta;
		if (spot->TravelTime < 0)
		{
			spot->TravelTime = 0;
		}
		break;
	}

	case SpotSetting::TravelActivity:
	{
		int travelActivityIndex = GetCurrentTravelActivityIndex();
		spot->TravelActivity = FindActivity(SpotManager::_travelActivites, SpotManager::_travelActivitiesCount, travelActivityIndex, delta);
		break;
	}
	}

	if (notify)
	{
		SettingValueChanged.Raise();
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
