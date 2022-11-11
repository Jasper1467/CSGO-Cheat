#pragma once

#include "VFunc.h"

class CPanel
{
public:
	void SetKeyBoardInputEnabled(unsigned int nPanelIndex, bool bState)
	{
		return CallVFunc<void>(this, 31, nPanelIndex, bState);
	}

	void SetMouseInputEnabled(unsigned int nPanelIndex, bool bState)
	{
		return CallVFunc<void>(this, 32, nPanelIndex, bState);
	}

	const char* GetName(int nPanelIndex)
	{
		return CallVFunc<const char*>(this, 36, nPanelIndex);
	}

	const char* GetNameClass(int nPanelIndex)
	{
		return CallVFunc<const char*>(this, 37, nPanelIndex);
	}
};