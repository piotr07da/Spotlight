#include "Display.h"

#include <Particle.h>
#include "DisplayLogo.h"

Display::Display(SpotManager *spotManager)
	: _oled(OLED_DUMMY_RESET_PIN), _spotManager(spotManager)
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

void Display::ShowSpotProperties(Spot spot, SpotSetting currentProperty)
{
	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);

	_oled.print("SPOT: ");
	_oled.println(spot.Index);

	_oled.print(currentProperty == SpotSetting::Position ? "-> " : "   ");
	_oled.print("Position: ");
	_oled.println(spot.Position);

	_oled.print(currentProperty == SpotSetting::SpotTime ? "-> " : "   ");
	_oled.print("SpotTime: ");
	_oled.println(spot.SpotTime);

	_oled.print(currentProperty == SpotSetting::SpotActivity ? "-> " : "   ");
	_oled.print("SpotActv: ");
	_oled.println(FormatLightActivityShort(spot.SpotActivity));

	_oled.print(currentProperty == SpotSetting::TravelTime ? "-> " : "   ");
	_oled.print("TravTime: ");
	_oled.println(spot.TravelTime);

	_oled.print(currentProperty == SpotSetting::TravelActivity ? "-> " : "   ");
	_oled.print("TravActv: ");
	_oled.println(FormatLightActivityShort(spot.TravelActivity));

	_needsRefresh = true;
}

void Display::ShowSpotProperty(Spot spot, SpotSetting currentProperty)
{
	switch (currentProperty)
	{
	case SpotSetting::Position:
		ShowSpotSetting(spot.Index, "Position:", String(spot.Position));
		break;

	case SpotSetting::SpotTime:
		ShowSpotSetting(spot.Index, "Spot Time:", String(spot.SpotTime));
		break;

	case SpotSetting::SpotActivity:
		ShowSpotSetting(spot.Index, "Spot Activity:", FormatLightActivity(spot.SpotActivity));
		break;

	case SpotSetting::TravelTime:
		ShowSpotSetting(spot.Index, "Travel Time:", String(spot.TravelTime));
		break;

	case SpotSetting::TravelActivity:
		ShowSpotSetting(spot.Index, "Travel Activity:", FormatLightActivity(spot.TravelActivity));
		break;
	}
}

void Display::OnControllerModeChanged(ControllerMode mode)
{
	switch (mode)
	{
	case ControllerMode::GlobalSettings:
		ShowGlobalSettings(_spotManager->GetSpotCount());
		break;

	case ControllerMode::SpotSettings:
		ShowSpotSetting(_spotManager->GetCurrentSpotIndex(), *_spotManager->GetCurrentSpot(), _spotManager->GetCurrentSetting());
		break;

	case ControllerMode::Standby:
		ShowStandby();
		break;
	}
}

void Display::OnNumberOfSpotsChanged()
{
	ShowGlobalSettings(_spotManager->GetSpotCount());
}

void Display::OnSpotChanged()
{
	ShowSpotSetting(_spotManager->GetCurrentSpotIndex(), *_spotManager->GetCurrentSpot(), _spotManager->GetCurrentSetting());
}

void Display::OnSettingChanged(SpotSetting setting)
{
	ShowSpotSetting(_spotManager->GetCurrentSpotIndex(), *_spotManager->GetCurrentSpot(), setting);
}

void Display::OnSettingValueChanged()
{
	ShowSpotSetting(_spotManager->GetCurrentSpotIndex(), *_spotManager->GetCurrentSpot(), _spotManager->GetCurrentSetting());
}

void Display::OnStartRequested()
{
	ShowRunning();
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

void Display::ShowGlobalSettings(int activeSpotCount)
{
	_oled.clearDisplay();
	_oled.setTextColor(WHITE);
	_oled.setTextSize(1);
	_oled.setCursor(0, 0);
	_oled.println("Active spots:");
	_oled.setTextSize(2);
	_oled.println(activeSpotCount);
	_needsRefresh = true;
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