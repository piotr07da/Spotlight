#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SH1106_headers.h"
#include "MasterMenuSubmenu.h"
#include "Spot.h"
#include "SpotCollection.h"
#include "SpotProperty.h"
#include "LightActivity.h"

#define OLED_DUMMY_RESET_PIN -1

static const unsigned char PROGMEM ArrowBitmap[] =
	{
		0b00001000,
		0b00000100,
		0b00000010,
		0b11111111,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00000000,
};

class Display
{
private:
	Adafruit_SH1106 _oled;
	bool _needsRefresh;

	void ShowSpotProperty(int spotIndex, int spotCount, const char *label, String value);
	String FormatLightActivity(LightActivity activity);
	String FormatLightActivityShort(LightActivity activity);

public:
	Display();

	void Setup();
	void Loop();

	void ShowWelcome();
	void ShowMasterMenu(MasterMenuSubmenu currentSubmenu);
	void ShowGlobalProperties(SpotCollection *spots);
	void ShowSpotProperties(Spot spot, int spotCount, SpotProperty currentProperty);
	void ShowSpotProperty(Spot spot, int spotCount, SpotProperty property);
	void ShowStandby(SpotCollection *spots);
	void ShowRunning();
	void ShowDiag(String diag);
};

#endif // DISPLAY_H