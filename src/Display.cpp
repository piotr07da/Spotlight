#include "Display.h"

#include "Particle.h"

Display::Display(sh1106_lcd *glcd)
{
	_glcd = glcd;
}

void Display::ShowWelcome()
{
	_glcd->ClearScreen();
	_glcd->Print("Welcome!");
}

void Display::ShowDiag(String diag)
{
	Print(diag);
}

void Display::ShowGlobalSettings(int activeSpotCount)
{
	_glcd->ClearScreen();
	String s = "Active spots: " + String(activeSpotCount);
	Print(s);
}

void Display::ShowSpotSetting(int spotIndex, Spot spot, SpotSetting setting)
{
	_glcd->ClearScreen();

	PrintLine("SPOT: " + String(spotIndex));
	PrintLine("");

	switch (setting)
	{
	case SpotSetting::Position:
		PrintLine("Position:");
		PrintLine(String(spot.Position));
		break;

	case SpotSetting::SpotTime:
		PrintLine("Spot Time: ");
		PrintLine(String(spot.SpotTime));
		break;

	case SpotSetting::SpotActivity:
		PrintLine("Spot Activity: ");
		PrintLine(FormatLightActivity(spot.SpotActivity));
		break;

	case SpotSetting::TravelTime:
		PrintLine("Travel Time: ");
		PrintLine(String(spot.TravelTime));
		break;

	case SpotSetting::TravelActivity:
		PrintLine("Travel Activity: ");
		PrintLine(FormatLightActivity(spot.TravelActivity));
		break;
	}
}

void Display::Print(String s)
{
	int sl = s.length();
	char buff[sl + 1];
	for (int i = 0; i < sl; ++i)
	{
		buff[i] = s.charAt(i);
	}
	buff[sl] = NULL;
	_glcd->Print(buff);
}

void Display::PrintLine(String s)
{
	int sl = s.length();
	char buff[sl + 1];
	for (int i = 0; i < sl; ++i)
	{
		buff[i] = s.charAt(i);
	}
	buff[sl] = NULL;
	_glcd->PrintLine(buff);
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
	}

	return "Unknown";
}