#include "LegitBot.h"

#include "../Utilities/Interfaces.h"
#include "../Utilities/Client.h"
#include "../Utilities/Math.h"
#include "../Menu/Config/Vars.h"

void Features::LegitBot::TriggerBot()
{
	if (!Vars::LegitBot::m_bEnabled || Vars::LegitBot::m_bTriggerBot)
		return;

	auto pWeapon = Client::m_pLocal->GetActiveWeapon();
	if (!pWeapon)
		return;

	auto pWeaponData = pWeapon->GetWeaponData();

	if (!pWeaponData || pWeaponData->IsGun())
		return;

	float weapon_recoil_scale = Interfaces::m_pCVar->FindVar("weapon_recoil_scale")->GetFloat();

	QAngle angView = Client::m_pCmd->m_angViewAngles;
	angView += Client::m_pLocal->m_aimPunchAngle() * weapon_recoil_scale;

	Vector vecStart, vecEnd, vecForward;
	Math::AngleVectors(angView, vecForward);

	vecStart = Client::m_pLocal->GetEyePos();
	vecForward *= pWeaponData->flRange;
	vecEnd = vecStart + vecForward;

	trace_t tr = {};
	Ray_t Ray(vecStart, vecEnd);
	CTraceFilter Filter(Client::m_pLocal);

	Interfaces::m_pTrace->TraceRay(Ray, MASK_SHOT, &Filter, &tr);

	auto pTarget = tr.pHitEntity;
	if (!pTarget || !tr.DidHit())
		return;

	Client::m_pCmd->m_nButtons |= IN_ATTACK;
}