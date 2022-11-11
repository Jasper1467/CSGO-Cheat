#pragma once

#include "ClientClass.h"
#include "VFunc.h"

class CHLClient
{
public:
	ClientClass* GetAllClasses()
	{
		return CallVFunc<ClientClass*>(this, 8);
	}
};