#pragma once

class Vector;
class CHudElement;

#include "Interfaces.h"
#include "Client.h"

namespace Game
{
	CHudElement* FindElement(const char* szName);

	inline float GetServerTime()
	{
		static int nTick = 0;
		if (Client::m_pCmd != nullptr)
		{
			static CUserCmd* pLastCmd = nullptr;

			if (!pLastCmd || pLastCmd->m_bHasBeenPredicted)
				nTick = Client::m_pLocal->m_nTickBase();
			else
				nTick++;

			pLastCmd = Client::m_pCmd;
		}

		return TICKS_TO_TIME(nTick);
	}
}