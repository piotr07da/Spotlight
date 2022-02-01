#include "MenuSpotsNavigator.h"

MenuSpotsNavigator::MenuSpotsNavigator(SpotCollection *spots, Motor *motor)
	: _spots(spots),
	  _motor(motor)
{
}

SpotCollection *MenuSpotsNavigator::GetAll()
{
	return _spots;
}

int MenuSpotsNavigator::GetCount()
{
	return _spots->GetCount();
}

Spot *MenuSpotsNavigator::GetCurrent()
{
	return _spots->GetByIndex(_currentSpotIndex);
}

int MenuSpotsNavigator::GetCurrentIndex()
{
	return _currentSpotIndex;
}

void MenuSpotsNavigator::EnsureCurrentIndexInRange()
{
	auto lastIndex = _spots->GetCount() - 1;
	if (_currentSpotIndex > lastIndex)
		_currentSpotIndex = lastIndex;
}

void MenuSpotsNavigator::Prev()
{
	--_currentSpotIndex;
	if (_currentSpotIndex < 0)
	{
		_currentSpotIndex = _spots->GetCount() - 1;
	}
	_motor->MoveToWithSpeed(GetCurrent()->Position, Motor_MaxSpeed);
}

void MenuSpotsNavigator::Next()
{
	++_currentSpotIndex;
	if (_currentSpotIndex > _spots->GetCount() - 1)
	{
		_currentSpotIndex = 0;
	}
	_motor->MoveToWithSpeed(GetCurrent()->Position, Motor_MaxSpeed);
}

void MenuSpotsNavigator::RepositionToCurrent()
{
	_motor->MoveToWithSpeed(GetCurrent()->Position, Motor_MaxSpeed);
}