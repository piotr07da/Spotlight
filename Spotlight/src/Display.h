#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SH1106_headers.h"
#include "Controller.h"
#include "SpotManager.h"
#include "Runner.h"
#include "Spot.h"
#include "SpotSetting.h"
#include "LightActivity.h"

#define OLED_DUMMY_RESET_PIN -1

class Display
{
private:
	Adafruit_SH1106 _oled;
	SpotManager *_spotManager;
	bool _needsRefresh;

	void ShowDiag(String diag);
	void ShowWelcome();
	void ShowGlobalSettings(int activeSpotCount);
	void ShowSpotSetting(int spotIndex, Spot spot, SpotSetting setting);
	void ShowSpotSetting(int spotIndex, const char *label, String value);
	void ShowStandby();
	void ShowRunning();
	String FormatLightActivity(LightActivity activity);
	String FormatLightActivityShort(LightActivity activity);

public:
	Display(SpotManager *spotManager);

	void Setup();
	void Loop();

	void ShowSpotProperties(Spot spot, SpotSetting currentProperty);
	void ShowSpotProperty(Spot spot, SpotSetting currentProperty);
	void OnControllerModeChanged(ControllerMode mode);
	void OnNumberOfSpotsChanged();
	void OnSpotChanged();
	void OnSettingChanged(SpotSetting setting);
	void OnSettingValueChanged();
	void OnStartRequested();
};

#endif // DISPLAY_H