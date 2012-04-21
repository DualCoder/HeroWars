#ifndef _ENTITY_H
#define _ENTITY_H
#include "common.h"

class Entity
{
	uint16 getNetID();
	void setNetID(uint16 _netId);
private:
	uint16 _netId;
};
#endif