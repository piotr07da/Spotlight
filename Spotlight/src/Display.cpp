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

void Display::ShowSpotProperties(Spot spot, SpotProperty currentProperty)
{
	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);

	_oled.print("SPOT: ");
	_oled.println(spot.Index);

	_oled.print(currentProperty == SpotProperty::Position ? "-> " : "   ");
	_oled.print("Position: ");
	_oled.println(spot.Position);

	_oled.print(currentProperty == SpotProperty::SpotTime ? "-> " : "   ");
	_oled.print("SpotTime: ");
	_oled.println(spot.SpotTime);

	_oled.print(currentProperty == SpotProperty::SpotActivity ? "-> " : "   ");
	_oled.print("SpotActv: ");
	_oled.println(FormatLightActivityShort(spot.SpotActivity));

	_oled.print(currentProperty == SpotProperty::TravelTime ? "-> " : "   ");
	_oled.print("TravTime: ");
	_oled.println(spot.TravelTime);

	_oled.print(currentProperty == SpotProperty::TravelActivity ? "-> " : "   ");
	_oled.print("TravActv: ");
	_oled.println(FormatLightActivityShort(spot.TravelActivity));

	_needsRefresh = true;
}

void Display::ShowSpotProperty(Spot spot, SpotProperty property)
{
	switch (property)
	{
	case SpotProperty::Position:
		ShowSpotProperty(spot.Index, "Position:", String(spot.Position));
		break;

	case SpotProperty::SpotTime:
		ShowSpotProperty(spot.Index, "Spot Time:", String(spot.SpotTime));
		break;

	case SpotProperty::SpotActivity:
		ShowSpotProperty(spot.Index, "Spot Activity:", FormatLightActivity(spot.SpotActivity));
		break;

	case SpotProperty::TravelTime:
		ShowSpotProperty(spot.Index, "Travel Time:", String(spot.TravelTime));
		break;

	case SpotProperty::TravelActivity:
		ShowSpotProperty(spot.Index, "Travel Activity:", FormatLightActivity(spot.TravelActivity));
		break;
	}
}

void Display::ShowStandby()
{
	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);
	_oled.print("STANDBY");
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

void Display::ShowSpotProperty(int spotIndex, Spot spot, SpotProperty property)
{
	switch (property)
	{
	case SpotProperty::Position:
		ShowSpotProperty(spotIndex, "Position:", String(spot.Position));
		break;

	case SpotProperty::SpotTime:
		ShowSpotProperty(spotIndex, "Spot Time:", String(spot.SpotTime));
		break;

	case SpotProperty::SpotActivity:
		ShowSpotProperty(spotIndex, "Spot Activity:", FormatLightActivity(spot.SpotActivity));
		break;

	case SpotProperty::TravelTime:
		ShowSpotProperty(spotIndex, "Travel Time:", String(spot.TravelTime));
		break;

	case SpotProperty::TravelActivity:
		ShowSpotProperty(spotIndex, "Travel Activity:", FormatLightActivity(spot.TravelActivity));
		break;
	}
}

void Display::ShowSpotProperty(int spotIndex, const char *label, String value)
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
	_oled.setTextSize(2);
	_oled.println(spotIndex);
	_oled.setTextSize(1);
	_oled.println("");
	_oled.println(label);
	_oled.println("");
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