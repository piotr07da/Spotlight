#ifndef DIAG_LED_H
#define DIAG_LED_H

#include <Particle.h>

class DiagLed
{
private:
	static bool _on;

public:
	static bool Toggle();
};

#endif // DIAG_LED_H