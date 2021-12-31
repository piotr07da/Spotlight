#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SH1106_headers.h"
#include "Spot.h"
#include "SpotSetting.h"
#include "LightActivity.h"



class Display
{
private:
	Adafruit_SH1106 *_oled;

	void ShowSpotSetting(int spotIndex, const char *label, String value);
	String FormatLightActivity(LightActivity activity);

public:
	Display(Adafruit_SH1106 *oled);

	void ShowWelcome();
	void ShowDiag(String diag);
	void ShowGlobalSettings(int activeSpotCount);
	void ShowSpotSetting(int spotIndex, Spot spot, SpotSetting setting);
};

#endif // DISPLAY_H