/*
 * Project Spotlight
 * Description:
 * Author: Piotr Bejger
 * Date: 2021-12-27
 */

#include "SpotManager.h"
#include "Adafruit_SH1106_headers.h"

#include "Controller.h"
#include "Button.h"
#include "Light.h"
#include "Display.h"
#include "Runner.h"
#include "AdcDma.h"
#include "DiagLed.h"

SpotManager _spotManager;
Motor _motor(A2, A3);
Light _light(D2);
Controller _controller(A0, A1, D3, D4, D5, D6, &_spotManager, &_motor, &_light);
#define OLED_RESET 4
Adafruit_SH1106 _oled(OLED_RESET);
Display _display(&_oled, &_spotManager);
Runner _runner(&_spotManager, &_motor, &_light);

const int AudioSampleBufferSize = 4410;
uint16_t _samplesBuffer[AudioSampleBufferSize];
AdcDma _adcDma(A5, _samplesBuffer, AudioSampleBufferSize, 44100);

TCPServer _server(33334);
TCPClient _client;

void setup()
{
    _server.begin();

    pinMode(D7, OUTPUT);

    pinMode(A5, INPUT);

    _controller.ModeChanged.Subscribe(
        [](ControllerMode mode)
        { _display.OnControllerModeChanged(mode); });

    _controller.StartRequested.Subscribe(
        []()
        { _display.OnStartRequested(); });

    _controller.StartRequested.Subscribe(
        []()
        { _runner.OnStartRequested(); });

    _controller.StopRequested.Subscribe(
        []()
        { _runner.OnStopRequested(); });

    _spotManager.NumberOfSpotsChanged.Subscribe(
        []()
        { _display.OnNumberOfSpotsChanged(); });

    _spotManager.SpotChanged.Subscribe(
        []()
        { _display.OnSpotChanged(); });

    _spotManager.SettingChanged.Subscribe(
        [](SpotSetting setting)
        { _display.OnSettingChanged(setting); });

    _spotManager.SettingValueChanged.Subscribe(
        []()
        { _display.OnSettingValueChanged(); });

    _motor.Setup();
    _light.Setup();
    _controller.Setup();
    _display.Setup();
    _runner.Setup();

    _adcDma.Start();

    // DiagLed::Toggle();
}

void loop()
{
    _controller.Loop();
    _display.Loop();
    _runner.Loop();
    _motor.Loop();
    _light.Loop();

    if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_HTIF0)) // HTIF - half transfer interrupt flag
    {
        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_HTIF0);
    }

    if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0)) // TCIF - transfer complete interrupt flag
    {
        if (_client.connected())
        {
            _client.write("BEGIN SAMPLES_BATCH;t:" + String(millis()) + ";");
            String s = "";
            for (int i = 0; i < AudioSampleBufferSize / 2; ++i)
            {
                s += String(_samplesBuffer[i * 2]) + ";";
                if (i % 10 == 9)
                {
                    _client.write(s);
                    s = "";
                }
            }
            _client.write("END;");
        }
        else
        {
            DiagLed::Toggle();
            _client = _server.available();
            if (_client.connected())
            {
                _client.write("BEGIN WELCOME;<header>;Spotlight here!;END;");
            }
        }

        DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
    }
}
