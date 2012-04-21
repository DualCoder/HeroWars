#include "Entity.h"

uint16 Entity::getNetID()
{
	return _netId;
}

void Entity::setNetID(uint16 netId)
{
	_netId = netId;
}