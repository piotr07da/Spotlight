#include "Display.h"

#include <Particle.h>
#include "DisplayLogo.h"

Display::Display()
	: _oled(OLED_DUMMY_RESET_PIN)
{
}

void Display::Setup()
{
	_oled.begin(SH1106_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
	ShowWelcome();
}

void Display::Loop()
{
	if (_needsRefresh)
	{
		_oled.display();
		_needsRefresh = false;
	}
}

void Display::ShowWelcome()
{
	_oled.clearDisplay();
	_oled.drawBitmap(15, 0, DisplayLogo, 96, 64, 1);
	_needsRefresh = true;
}

void Display::ShowMasterMenu(MasterMenuSubmenu currentSubmenu)
{
	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);

	_oled.println("  Global");
	_oled.println("  Spots");
	_oled.println("  Standby");
	_oled.println("  Help");

	int rowIx = (int)currentSubmenu;
	_oled.drawBitmap(0, rowIx * 8, ArrowBitmap, 8, 8, 1);

	_needsRefresh = true;
}

void Display::ShowGlobalProperties(SpotCollection *spots)
{
	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);
	_oled.println("Active spots:");
	_oled.setTextSize(2);
	_oled.println(spots->GetCount());
	_needsRefresh = true;
}

void Display::ShowSpotProperties(Spot spot, int spotCount, SpotProperty currentProperty)
{
	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);

	_oled.print("SPOT: ");
	_oled.print(spot.Index + 1);
	_oled.print("/");
	_oled.println(spotCount);

	_oled.println();

	_oled.print("  Position: ");
	_oled.println(spot.Position);

	_oled.print("  SpotTime: ");
	_oled.println(spot.SpotTime);

	_oled.print("  SpotActv: ");
	_oled.println(FormatLightActivityShort(spot.SpotActivity));

	_oled.print("  TravTime: ");
	_oled.println(spot.TravelTime);

	_oled.print("  TravActv: ");
	_oled.println(FormatLightActivityShort(spot.TravelActivity));

	int rowIx = (int)currentProperty + 2;
	_oled.drawBitmap(0, rowIx * 8, ArrowBitmap, 8, 8, 1);

	_needsRefresh = true;
}

void Display::ShowSpotProperty(Spot spot, int spotCount, SpotProperty property)
{
	switch (property)
	{
	case SpotProperty::Position:
		ShowSpotProperty(spot.Index, spotCount, "Position:", String(spot.Position));
		break;

	case SpotProperty::SpotTime:
		ShowSpotProperty(spot.Index, spotCount, "Spot Time:", String(spot.SpotTime));
		break;

	case SpotProperty::SpotActivity:
		ShowSpotProperty(spot.Index, spotCount, "Spot Activity:", FormatLightActivity(spot.SpotActivity));
		break;

	case SpotProperty::TravelTime:
		ShowSpotProperty(spot.Index, spotCount, "Travel Time:", String(spot.TravelTime));
		break;

	case SpotProperty::TravelActivity:
		ShowSpotProperty(spot.Index, spotCount, "Travel Activity:", FormatLightActivity(spot.TravelActivity));
		break;
	}
}

void Display::ShowStandby(SpotCollection *spots)
{
	_oled.clearDisplay();
	if (spots->GetCount() == 0)
	{
		_oled.setTextColor(WHITE);
		_oled.setTextSize(2);
		_oled.setCursor(20, 26);
		_oled.println("EMPTY");
	}
	else
	{
		_oled.setTextColor(WHITE);
		_oled.setTextSize(1);
		_oled.setCursor(0, 0);
		_oled.print("STANDBY");
	}
	_needsRefresh = true;
}

void Display::ShowRunning()
{
	_oled.clearDisplay();
	_oled.drawBitmap(15, 0, DisplayLogo, 96, 64, 1);
	_needsRefresh = true;
}

void Display::ShowDiag(String diag)
{
	int sl = diag.length();
	char buff[sl + 1];
	for (int i = 0; i < sl; ++i)
	{
		buff[i] = diag.charAt(i);
	}
	buff[sl] = '\0';

	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);
	_oled.println(buff);
	_needsRefresh = true;
}

void Display::ShowSpotProperty(int spotIndex, int spotCount, const char *label, String value)
{
	int sl = value.length();
	char buff[sl + 1];
	for (int i = 0; i < sl; ++i)
	{
		buff[i] = value.charAt(i);
	}
	buff[sl] = '\0';

	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);
	_oled.print("SPOT: ");
	_oled.print(spotIndex + 1);
	_oled.print("/");
	_oled.println(spotCount);
	_oled.println();
	_oled.println(label);
	_oled.println();
	_oled.setTextSize(2);
	_oled.println(buff);
	_needsRefresh = true;
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
		return "Off-On";
	case LightActivity::A_10:
		return "On-Off";
	case LightActivity::A_010:
		return "Off-On-Off";
	case LightActivity::A_101:
		return "On-Off-On";
	case LightActivity::A_Strobe:
		return "Strobe";
	}

	return "Unknown";
}

String Display::FormatLightActivityShort(LightActivity activity)
{
	switch (activity)
	{
	case LightActivity::A_0:
		return "Off";
	case LightActivity::A_1:
		return "On";
	case LightActivity::A_01:
		return "OffOn";
	case LightActivity::A_10:
		return "OnOff";
	case LightActivity::A_010:
		return "OffOnOff";
	case LightActivity::A_101:
		return "OnOffOn";
	case LightActivity::A_Strobe:
		return "Strobe";
	}

	return "Unknown";
}