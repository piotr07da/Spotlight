#ifndef DISPLAY_H
#define DISPLAY_H

#include "../lib/sh1106/src/sh1106.h"
#include "Spot.h"
#include "SpotSetting.h"
#include "LightActivity.h"

class Display
{
private:
	sh1106_lcd *_glcd;

	void Print(String s);
	void PrintLine(String s);
	String FormatLightActivity(LightActivity activity);

public:
	Display(sh1106_lcd *glcd);

	void ShowWelcome();
	void ShowDiag(String diag);
	void ShowGlobalSettings(int activeSpotCount);
	void ShowSpotSetting(int spotIndex, Spot spot, SpotSetting setting);
};

#endif // DISPLAY_H