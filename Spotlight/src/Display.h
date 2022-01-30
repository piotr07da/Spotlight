#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SH1106_headers.h"
#include "Spot.h"
#include "SpotCollection.h"
#include "SpotProperty.h"
#include "LightActivity.h"

#define OLED_DUMMY_RESET_PIN -1

class Display
{
private:
	Adafruit_SH1106 _oled;
	bool _needsRefresh;

	void ShowSpotProperty(int spotIndex, Spot spot, SpotProperty property);
	void ShowSpotProperty(int spotIndex, const char *label, String value);
	String FormatLightActivity(LightActivity activity);
	String FormatLightActivityShort(LightActivity activity);

public:
	Display();

	void Setup();
	void Loop();

	void ShowWelcome();
	void ShowGlobalProperties(SpotCollection *spots);
	void ShowSpotProperties(Spot spot, SpotProperty currentProperty);
	void ShowSpotProperty(Spot spot, SpotProperty property);
	void ShowStandby();
	void ShowRunning();
	void ShowDiag(String diag);
};

#endif // DISPLAY_H