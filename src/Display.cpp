#include "Display.h"

#include <Particle.h>

Display::Display(Adafruit_SH1106 *oled)
{
	_oled = oled;

	_oled->begin(SH1106_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
}

void Display::ShowWelcome()
{
	_oled->clearDisplay();

	_oled->setTextSize(2);
	_oled->setTextColor(WHITE);
	_oled->setCursor(0, 0);
	_oled->println("Welcome!");
	_oled->display();
}

void Display::ShowDiag(String diag)
{
}

void Display::ShowGlobalSettings(int activeSpotCount)
{
	_oled->clearDisplay();
	_oled->setTextColor(WHITE);
	_oled->setTextSize(1);
	_oled->setCursor(0, 0);
	_oled->println("Active spots:");
	_oled->setTextSize(2);
	_oled->println(activeSpotCount);
	_oled->display();
}

void Display::ShowSpotSetting(int spotIndex, Spot spot, SpotSetting setting)
{
	switch (setting)
	{
	case SpotSetting::Position:
		ShowSpotSetting(spotIndex, "Position:", String(spot.Position));
		break;

	case SpotSetting::SpotTime:
		ShowSpotSetting(spotIndex, "Spot Time:", String(spot.SpotTime));
		break;

	case SpotSetting::SpotActivity:
		ShowSpotSetting(spotIndex, "Spot Activity:", FormatLightActivity(spot.SpotActivity));
		break;

	case SpotSetting::TravelTime:
		ShowSpotSetting(spotIndex, "Travel Time:", String(spot.TravelTime));
		break;

	case SpotSetting::TravelActivity:
		ShowSpotSetting(spotIndex, "Travel Activity:", FormatLightActivity(spot.TravelActivity));
		break;
	}
}

void Display::ShowSpotSetting(int spotIndex, const char *label, String value)
{
	int sl = value.length();
	char buff[sl + 1];
	for (int i = 0; i < sl; ++i)
	{
		buff[i] = value.charAt(i);
	}
	buff[sl] = NULL;

	_oled->clearDisplay();
	_oled->setTextColor(WHITE);
	_oled->setTextSize(1);
	_oled->setCursor(0, 0);
	_oled->print("SPOT: ");
	_oled->setTextSize(2);
	_oled->println(spotIndex);
	_oled->setTextSize(1);
	_oled->println("");
	_oled->println(label);
	_oled->println("");
	_oled->setTextSize(2);
	_oled->println(buff);
	_oled->display();
}

String Display::FormatLightActivity(LightActivity activity)
{
	switch (activity)
	{
	case LightActivity::A_0:
		return "Off";
	case LightActivity::A_1:
		return "On";
	case LightActivity::A_01:
		return "On-Off";
	case LightActivity::A_10:
		return "Off-On";
	case LightActivity::A_010:
		return "Off-On-Off";
	case LightActivity::A_101:
		return "On-Off-On";
	case LightActivity::A_Strobe:
		return "Strobe";
	}

	return "Unknown";
}