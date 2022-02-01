#ifndef MASTER_MENU_ACTIVATOR_H
#define MASTER_MENU_ACTIVATOR_H

// This is a workaround.
// Circular dependency between types is only allowed if both types refer to each other by reference.
// In our case all the menus under MasterMenu refer to MasterMenu by reference which is correct.
// However MasterMenu refers to other menus by value which is not possible to work in c++.
// The only functionality of MasterMenu used by all other menus is Activating it
// and this is the purpose of this workaround.

class MasterMenuActivator
{
private:
	bool _activateRequested;

public:
	void Activate();
	bool ActivateRequested();
};

#endif // MASTER_MENU_ACTIVATOR_H