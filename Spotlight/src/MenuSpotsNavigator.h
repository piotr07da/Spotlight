#ifndef MENU_SPOTS_NAVIGATOR_H
#define MENU_SPOTS_NAVIGATOR_H

#include "SpotCollection.h"
#include "Motor.h"

class MenuSpotsNavigator
{
private:
	SpotCollection *_spots;
	Motor *_motor;
	int _currentSpotIndex;

public:
	MenuSpotsNavigator(SpotCollection *spots, Motor *motor);

	SpotCollection *GetAll();
	int GetCount();
	Spot *GetCurrent();
	int GetCurrentIndex();
	void Prev();
	void Next();
};

#endif // MENU_SPOT_NAVIGATOR_H