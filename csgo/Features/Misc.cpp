#include "Misc.h"

#include "../Menu/Config/Vars.h"
#include "../Utilities/Client.h"

#include "../SDK/CEntity.h"
#include "../SDK/CUserCmd.h"

#include "../Utilities/Math.h"
#include "../Utilities/Utilities.h"
#include "../Utilities/Interfaces.h"

void Features::Misc::ClanTag()
{
	if (!Vars::Misc::m_bClanTag)
		return;

	auto SetClanTag = [](const char* szTag) -> void
	{
		static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>
			(((DWORD)Utilities::ScanSignature("engine.dll", ("53 56 57 8B DA 8B F9 FF 15"))));

		if (!pSetClanTag)
			throw std::runtime_error("pSetClanTag is nullptr");

			pSetClanTag(szTag, szTag);
	};

	std::string szTag = "Elite ";

	float flTime = Interfaces::m_pGlobals->m_flRealTime - Interfaces::m_pEngine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);

	// Build clantag using marquee logic
	std::string szCurTag;
	for (int i = 0; i < szTag.length(); i++)
	{
		int nCurLetter = i + int(flTime * 2.f); // Rate is 2 characters a second
		szCurTag += szTag.at(nCurLetter % szTag.length()); // Current char is wrapped to clantag length so it loops
	}

	SetClanTag(szCurTag.c_str());
}

void Features::Misc::BunnyHop()
{
	// TODO: movetype fixes

	int nChance = rand() % 100;
	if (nChance >= Vars::Misc::m_nBunnyHopHitChance)
		return;

	if (Vars::Misc::m_bBunnyHop && (Client::m_pCmd->m_nButtons & IN_JUMP &&
		!(Client::m_pLocal->m_fFlags() & FL_PARTIALGROUND || Client::m_pLocal->m_fFlags() & FL_ONGROUND)))
	{
		Client::m_pCmd->m_nButtons &= ~IN_JUMP;
	}
}

float Features::Misc::GetDegreeFromVelocity(float flSpeed)
{
	float retn = RAD2DEG(asinf(30.f / flSpeed));
	float v2 = 90.f;

	if (!isfinite(retn) || retn > 90.f || (v2 = 0.f, retn < 0.f))
		return v2;
	else
		return retn;
}

float Features::Misc::GetIdealRotation(float flSpeed)
{
	// 15.f is the ideal angle of rotation
	float flFactor = RAD2DEG(atan2(15.f, flSpeed));
	return std::clamp(flFactor, 0.f, 45.f) * Interfaces::m_pGlobals->m_flIntervalPerTick;
}

void Features::Misc::RotateMovement(float flYawToRotateTowards)
{
	const float flRotation = DEG2RAD(Client::m_pCmd->m_angViewAngles.yaw - flYawToRotateTowards);

	const float flNewForward = (cos(flRotation) * Client::m_pCmd->m_flForwardMove) - (sin(flRotation) * Client::m_pCmd->m_flSideMove);
	const float flNewSide = (sin(flRotation) * Client::m_pCmd->m_flForwardMove) + (cos(flRotation) * Client::m_pCmd->m_flSideMove);

	Client::m_pCmd->m_flForwardMove = flNewForward;
	Client::m_pCmd->m_flSideMove = flNewSide;
}

void Features::Misc::AutoStrafer()
{
	if (Vars::Misc::m_nAutoStrafer == 0) // If autostrafer is set to off, just return
		return;

	// TODO: movetype fixes

	if (Client::m_pLocal->m_fFlags() & FL_ONGROUND || Client::m_pCmd->m_nButtons & IN_SPEED)
		return;

	// TODO: cvars
	static float cl_sidespeed = 450.f;
	static float cl_forwardspeed = 450.f;

	static float flOldYaw = Client::m_pCmd->m_angViewAngles.yaw;

	Vector vecVelocity = Client::m_pLocal->m_vecVelocity();
	float flSpeed = vecVelocity.Length2D();

	float flIdeal = (flSpeed > 0.f) ? RAD2DEG(asin(15.f / flSpeed)) : 90.f;
	float flIdeal2 = (flSpeed > 0.f) ? RAD2DEG(asin(30.f / flSpeed)) : 90.f;

	flIdeal = std::clamp(flIdeal, 0.f, 90.f);
	flIdeal2 = std::clamp(flIdeal2, 0.f, 90.f);

	static float flSwitchValue = -1.f;
	flSwitchValue *= -1.f;

	if (Vars::Misc::m_nAutoStrafer == 2)
	{
		// took this idea from stacker, thank u !!!!
		enum EDirections {
			FORWARDS = 0,
			BACKWARDS = 180,
			LEFT = 90,
			RIGHT = -90,
			BACK_LEFT = 135,
			BACK_RIGHT = -135
		};

		float wish_dir{ };

		// get our key presses.
		bool holding_w = Client::m_pCmd->m_nButtons & IN_FORWARD;
		bool holding_a = Client::m_pCmd->m_nButtons & IN_MOVELEFT;
		bool holding_s = Client::m_pCmd->m_nButtons & IN_BACK;
		bool holding_d = Client::m_pCmd->m_nButtons & IN_MOVERIGHT;

		// move in the appropriate direction.
		if (holding_w) {
			//	forward left
			if (holding_a) {
				wish_dir += (EDirections::LEFT / 2);
			}
			//	forward right
			else if (holding_d) {
				wish_dir += (EDirections::RIGHT / 2);
			}
			//	forward
			else {
				wish_dir += EDirections::FORWARDS;
			}
		}
		else if (holding_s) {
			//	back left
			if (holding_a) {
				wish_dir += EDirections::BACK_LEFT;
			}
			//	back right
			else if (holding_d) {
				wish_dir += EDirections::BACK_RIGHT;
			}
			//	back
			else {
				wish_dir += EDirections::BACKWARDS;
			}

			Client::m_pCmd->m_flForwardMove = 0;
		}
		else if (holding_a) {
			//	left
			wish_dir += EDirections::LEFT;
		}
		else if (holding_d) {
			//	right
			wish_dir += EDirections::RIGHT;
		}

		Client::m_pCmd->m_angViewAngles += Math::NormalizeYaw(wish_dir);
	}


	Client::m_pCmd->m_flForwardMove = 0.f;

	float flDelta = Math::NormalizeYaw(Client::m_pCmd->m_angViewAngles.yaw - flOldYaw);
	float flAbsDelta = abs(flDelta);

	// Set strafe direction based on mouse direction change
	if (flDelta > 0.f)
		Client::m_pCmd->m_flSideMove = -450.f;
	else
		Client::m_pCmd->m_flSideMove = 450.f;

	// We accelerated more, because we strafed less then needed or we got off track and need to be retracked
	if (flAbsDelta <= flIdeal || flAbsDelta >= 30.f)
	{
		// Compute angle of the direction we are traveling in
		QAngle angVelocity;
		Math::VectorAngles(vecVelocity, angVelocity);

		// get the delta between our direction and where we are looking at.
		float flVelocityDelta = Math::NormalizeYaw(Client::m_pCmd->m_angViewAngles.yaw - angVelocity.yaw);

		// correct our strafe amongst the path of a circle.
		float correct = flIdeal;

		if (flVelocityDelta <= correct || flSpeed <= 15.f) {
			// not moving mouse, switch strafe every tick.
			if (-correct <= flVelocityDelta || flSpeed <= 15.f)
			{
				Client::m_pCmd->m_angViewAngles.yaw += (flIdeal * flSwitchValue);
				Client::m_pCmd->m_flSideMove = 450.f * flSwitchValue;
			}

			else
			{
				Client::m_pCmd->m_angViewAngles.yaw = angVelocity.yaw - correct;
				Client::m_pCmd->m_flSideMove = 450.f;
			}
		}
		else
		{
			Client::m_pCmd->m_angViewAngles.yaw = angVelocity.yaw + correct;
			Client::m_pCmd->m_flSideMove = -450.f;
		}
	}
}

void Features::Misc::CorrectMovement()
{
	QAngle va;
	Interfaces::m_pEngine->GetViewAngles(&va);

	Vector vecMove(Client::m_pCmd->m_flForwardMove, Client::m_pCmd->m_flSideMove, Client::m_pCmd->m_flUpMove);
	float flSpeed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	QAngle angMove;
	Math::VectorAngles(vecMove, angMove);

	float yaw = DEG2RAD(Client::m_pCmd->m_angViewAngles.yaw - Client::m_angOldAngles.yaw + angMove.yaw);

	Client::m_pCmd->m_flForwardMove = cos(yaw) * flSpeed;
	Client::m_pCmd->m_flSideMove = sin(yaw) * flSpeed;

	Client::m_pCmd->m_nButtons &= ~IN_RIGHT;
	Client::m_pCmd->m_nButtons &= ~IN_MOVERIGHT;
	Client::m_pCmd->m_nButtons &= ~IN_LEFT;
	Client::m_pCmd->m_nButtons &= ~IN_MOVELEFT;
	Client::m_pCmd->m_nButtons &= ~IN_FORWARD;
	Client::m_pCmd->m_nButtons &= ~IN_BACK;

	if (Client::m_pCmd->m_flForwardMove > 0.f)
		Client::m_pCmd->m_nButtons |= IN_FORWARD;
	else if (Client::m_pCmd->m_flForwardMove < 0.f)
		Client::m_pCmd->m_nButtons |= IN_BACK;

	if (Client::m_pCmd->m_flSideMove > 0.f)
	{
		Client::m_pCmd->m_nButtons |= IN_RIGHT;
		Client::m_pCmd->m_nButtons |= IN_MOVERIGHT;
	}
	else if (Client::m_pCmd->m_flSideMove < 0.f)
	{
		Client::m_pCmd->m_nButtons |= IN_LEFT;
		Client::m_pCmd->m_nButtons |= IN_MOVELEFT;
	}
}