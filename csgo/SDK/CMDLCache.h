#pragma once

#include "VFunc.h"

class CMDLCache
{
public:
	void BeginLock()
	{
		return CallVFunc<void>(this, 33);
	}

	void EndLock()
	{
		return CallVFunc<void>(this, 34);
	}
};