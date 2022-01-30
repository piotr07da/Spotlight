#include "SpotCollection.h"

Spot *SpotCollection::GetByIndex(int index)
{
	return _spots + index;
}
