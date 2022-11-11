#include "AntiAim.h"

#include "../Utilities/Client.h"
#include "../Utilities/Interfaces.h"

#include "../Menu/Config/Vars.h"

#include "../SDK/CUserCmd.h"
#include "../Utilities/Math.h"

void Features::AntiAim::Run(bool& bSendPacket)
{
	if (!Vars::AntiAim::m_bEnabled)
		return;

	if (!Client::m_pLocal->IsAlive() || Client::m_pLocal->m_fFlags() & FL_FROZEN)
		return;

	if (Client::m_pCmd->m_nButtons & IN_USE)
		return;

	if (Client::m_pCmd->m_nButtons & IN_ATTACK)
		return;

	FakeLag(bSendPacket);

	if (bSendPacket)
		Pitch();

	Yaw(bSendPacket);
}

void Features::AntiAim::Pitch()
{
	switch (Vars::AntiAim::m_nPitchMode)
	{
	case 0: // None, do nothing
		break;
	case 1: // Down
		Client::m_pCmd->m_angViewAngles.pitch = 89.f;
		break;
	case 2: // Up
		Client::m_pCmd->m_angViewAngles.pitch = -89.f;
		break;
	case 3: // Jitter
		Client::m_pCmd->m_angViewAngles.pitch = (Client::m_pCmd->m_nCommandNumber) ? 89.f : -89.f;
		break;
	case 4: // Fake Down
		Client::m_pCmd->m_angViewAngles.pitch = -180.f;
		break;
	case 5: // Fake Up
		Client::m_pCmd->m_angViewAngles.pitch = 180.f;
		break;
	}
}

void Features::AntiAim::Yaw(bool& bSendPacket)
{
	Client::m_pCmd->m_angViewAngles.yaw = Math::NormalizeYaw(Client::m_pCmd->m_angViewAngles.yaw + 180.f);

	int nDesyncSide = bSendPacket ? 1 : 0;

	if (!bSendPacket)
	{
		float flDesyncAmount = static_cast<float>((nDesyncSide > 0 ? 58.f : -58.f));

		Client::m_pCmd->m_angViewAngles.yaw += flDesyncAmount;
	}
}

void Features::AntiAim::FakeLag(bool& bSendPacket)
{
	int nTicksToChoke = 0;

	int sv_maxusrcmdprocessticks = Interfaces::m_pCVar->FindVar("sv_maxusrcmdprocessticks")->GetInt();
	switch (Vars::AntiAim::m_nFakeLagMode)
	{
	case 0: // None, do nothing
		return;
		break;
	case 1: // Static
		nTicksToChoke = Vars::AntiAim::m_nFakeLagModeStaticAmount;
		break;
	case 2: // Adaptive, based on velocity
	{
		bool should_choke = false;
		const float flUnitsPerTick = std::ceilf(68.0f / TICKS_TO_TIME(Client::m_pLocal->m_vecVelocity().Length()));

		if (flUnitsPerTick > 0 && flUnitsPerTick < 14)
		{
			if (Client::m_pLocal->m_vecVelocity().LengthSqr() > 0.15f)
			{
				if (Client::m_pLocal->m_vecVelocity().LengthSqr() > 4096.f)
				{
					nTicksToChoke = 14;
					Client::m_bBrokeLC = true;
				}
				else
					nTicksToChoke = flUnitsPerTick ? 14 - 1 : 15;
			}
			else
				nTicksToChoke = 14;//g_cfg->m_lag_limit;
		}
		break;
	}
	case 3: // Random
		int nRandom = rand() % (sv_maxusrcmdprocessticks - 2);

		// Is nRandom between our min and max value
		if (nRandom > Vars::AntiAim::m_nFakeLagRandomMin && nRandom < Vars::AntiAim::m_nFakeLagRandomMax)
			nTicksToChoke = nRandom;
		break;
	}

	// Make sure to not fuck up by not sending enough packets, we devide by 2 to reserve some ticks for exploits later on
	if (nTicksToChoke >= sv_maxusrcmdprocessticks - 2)
		nTicksToChoke = sv_maxusrcmdprocessticks - 2;

	bSendPacket = Interfaces::m_pClientState->m_nChokedCommands >= nTicksToChoke;
}