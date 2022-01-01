#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SH1106_headers.h"
#include "Controller.h"
#include "SpotManager.h"
#include "Runner.h"
#include "Spot.h"
#include "SpotSetting.h"
#include "LightActivity.h"

class Display
{
private:
	Adafruit_SH1106 *_oled;
	Controller *_controller;
	SpotManager *_spotManager;
	Runner *_runner;

	void ShowSpotSetting(int spotIndex, const char *label, String value);
	String FormatLightActivity(LightActivity activity);

public:
	Display(Adafruit_SH1106 *oled, Controller *controller, SpotManager *spotManager, Runner *runner);

	void ShowWelcome();
	void ShowDiag(String diag);
	void ShowGlobalSettings(int activeSpotCount);
	void ShowSpotSetting(int spotIndex, Spot spot, SpotSetting setting);

	void OnControllerModeChanged(ControllerMode mode);
	void OnNumberOfSpotsChanged();
	void OnSpotChanged();
};

#endif // DISPLAY_H