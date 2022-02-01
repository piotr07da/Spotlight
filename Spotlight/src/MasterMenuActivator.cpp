#include "MasterMenuActivator.h"

void MasterMenuActivator::Activate()
{
	_activateRequested = true;
}

bool MasterMenuActivator::ActivateRequested()
{
	auto r = _activateRequested;
	if (_activateRequested)
	{
		_activateRequested = false;
	}
	return r;
}