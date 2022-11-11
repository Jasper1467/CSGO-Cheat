#pragma once

class CPlayer;
class CUserCmd;

#include "../SDK/Vectors.h"

namespace Client
{
	inline CPlayer* m_pLocal = nullptr;
	inline CUserCmd* m_pCmd = nullptr;

	inline int m_nScreenSizeX = 0;
	inline int m_nScreenSizeY = 0;

	inline const char* m_szUserName = "user";

	inline QAngle m_angOldAngles = QAngle();

	inline bool m_bBrokeLC = false;
}