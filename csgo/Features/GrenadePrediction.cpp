#include "GrenadePrediction.h"

#include "../SDK/CUserCmd.h"
#include "../Utilities/Client.h"
#include "../Utilities/Interfaces.h"
#include "../Utilities/Render.h"
#include "../Utilities/Math.h"
#include "../Menu/Config/Vars.h"

#include "AutoWall.h"

void Features::GrenadePrediction::Tick(int nButtons)
{
	if (!Vars::Visuals::World::m_bGrenadePrediction)
		return;

	m_nAct = ACT_NONE;

	auto in_attack = nButtons & IN_ATTACK;
	auto in_attack2 = nButtons & IN_ATTACK2;

	if (in_attack || in_attack2)
	{
		if (in_attack && in_attack2)
			m_nAct = ACT_LOB;
		else if (!in_attack)
			m_nAct = ACT_DROP;
		else
			m_nAct = ACT_THROW;
	}
	else if (!Vars::Visuals::World::m_bGrenadePredictionOnClick)
		m_nAct = ACT_THROW;
}

void Features::GrenadePrediction::View(CViewSetup* pSetup, CWeapon* pWeapon)
{
	if (!Vars::Visuals::World::m_bGrenadePrediction)
		return;

	if (Client::m_pLocal->IsAlive())
	{
		if (m_nAct != ACT_NONE)
		{
			m_nType = pWeapon->m_iItemDefinitionIndex();
			Simulate(pSetup);
		}
		else
			m_nType = 0;
	}
}


float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

void Features::GrenadePrediction::Paint()
{
	if (!Vars::Visuals::World::m_bGrenadePrediction)
		return;

	if (!Client::m_pLocal->IsAlive())
		return;

	auto pWeapon = Client::m_pLocal->GetActiveWeapon();
	if (!pWeapon)
		return;

	if (m_Path.size() < 2)
		return;

	Vector vecPrev = m_Path[0];

	std::vector<ImVec2> Points = {};
	Points.clear();
	
	if (m_nType && m_Path.size() > 1)
	{
		ImVec2 vecStart, vecEnd;

		for (const auto& it : m_Path)
		{
			if (Renderer::WorldToScreen(vecPrev, vecStart) && Renderer::WorldToScreen(it, vecEnd))
				Points.push_back(vecEnd);

			vecPrev = it;
		}

		if (!Points.empty())
			Renderer::PolyLine(Points.data(), Points.size(), Color(255, 0, 0, 255), false, 1.5f);

		Renderer::CircleFilled(vecEnd, 5.f, Color(0, 255, 0, 255), 20);
		Renderer::CircleFilled(vecEnd, 4.f, Color(0, 255, 0, 255), 20);
	}
}

void Features::GrenadePrediction::Setup(Vector& vecSrc, Vector& vecThrow, const QAngle& angViewAngles)
{
	QAngle angThrow = angViewAngles;
	float pitch = Math::NormalizePitch(angThrow.pitch);

	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.pitch = a;

	float flVel = 750.0f * 0.9f;
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[m_nAct];
	b = b * 0.7f; b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	Math::AngleVectors(angThrow, vForward, vRight, vUp);

	vecSrc = Client::m_pLocal->GetEyePos();
	float off = power[m_nAct] * 12.0f - 12.0f;
	vecSrc.z += off;

	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	TraceHull(vecSrc, vecDest, tr);

	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.vecEnd;
	vecSrc -= vecBack;

	vecThrow = Client::m_pLocal->m_vecVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void Features::GrenadePrediction::Simulate(CViewSetup* pSetup)
{
	Vector vecSrc, vecThrow;
	QAngle angles; Interfaces::m_pEngine->GetViewAngles(&angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = Interfaces::m_pGlobals->m_flIntervalPerTick;
	int logstep = (int)(0.05f / interval);
	int logtimer = 0;

	m_Path.clear();
	m_Others.clear();

	for (auto i = 0; i < 4096; ++i)
	{
		if (!logtimer)
			m_Path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);

		if (s & 1)
			break;

		if (s & 2 || logtimer >= logstep)
			logtimer = 0;
		else
			++logtimer;

		if (!vecThrow.IsValid())
			break;
	}

	m_Path.push_back(vecSrc);
	m_Others.emplace_back(std::make_pair(vecSrc, Color(255, 0, 0, 255)));
}


int Features::GrenadePrediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{
	Vector move; AddGravityMove(move, vecThrow, interval, false);
	trace_t tr; PushEntity(vecSrc, move, tr);

	int result = 0;

	if (CheckDetonate(vecThrow, tr, tick, interval))
		result |= 1;

	if (tr.flFraction != 1.0f)
	{
		result |= 2;
		ResolveFlyCollisionCustom(tr, vecThrow, move, interval);

		QAngle angles;
		Math::VectorAngles((tr.vecEnd - tr.vecStart).Normalized(), angles);
		m_Others.emplace_back(std::make_pair(tr.vecEnd, Color(255, 255, 255, 255)));
	}

	vecSrc = tr.vecEnd;
	return result;
}


bool Features::GrenadePrediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int nTick, float flInterval)
{
	static auto molotov_throw_detonate_time = Interfaces::m_pCVar->FindVar("molotov_throw_detonate_time");
	static auto weapon_molotov_maxdetonateslope = Interfaces::m_pCVar->FindVar("weapon_molotov_maxdetonateslope");

	auto time = TICKS_TO_TIME(nTick);

	switch (m_nType)
	{
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		return time >= 1.5f && !(nTick % TIME_TO_TICKS(0.2f));

	case WEAPON_SMOKEGRENADE:
		return vecThrow.Length() <= 0.1f && !(nTick % TIME_TO_TICKS(0.2f));

	case WEAPON_DECOY:
		return vecThrow.Length() <= 0.2f && !(nTick % TIME_TO_TICKS(0.2f));

	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
	case WEAPON_FIREBOMB:

		if (tr.flFraction != 1.f && (std::cos(DEG2RAD(weapon_molotov_maxdetonateslope->GetFloat())) <= tr.plane.vecNormal.z))
			return true;

		return time >= molotov_throw_detonate_time->GetFloat() && !(nTick % TIME_TO_TICKS(0.1f));
	case WEAPON_SNOWBALL:
		if (tr.DidHit())
			return true;
		return vecThrow.Length() <= 0.1f && !(nTick % TIME_TO_TICKS(0.2f));
	default:
		return false;
	}

	return false;
}

void Features::GrenadePrediction::TraceHull(Vector& vecSrc, Vector& vecEnd, trace_t& tr)
{
	CTraceFilter filter(Client::m_pLocal);

	Ray_t Ray(vecSrc, vecEnd, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	const unsigned int mask = 0x200400B;
	Interfaces::m_pTrace->TraceRay(Ray, mask, &filter, &tr);
}


void Features::GrenadePrediction::AddGravityMove(Vector& vecMove, Vector& vecVelocity, float flFrameTime, bool bOnGround)
{
	static auto sv_gravity = Interfaces::m_pCVar->FindVar("sv_gravity");

	float gravity = sv_gravity->GetFloat() * 0.4f;

	float flInterval = Interfaces::m_pGlobals->m_flIntervalPerTick;

	vecMove.x = vecVelocity.x * flInterval;
	vecMove.y = vecVelocity.y * flInterval;

	float z = vecVelocity.z - (gravity * flInterval);

	vecMove.z = ((vecVelocity.z + z) / 2.f) * flInterval;

	vecVelocity.z = z;
}


void Features::GrenadePrediction::PushEntity(Vector& vecSrc, const Vector& vecMove, trace_t& tr)
{
	Vector vecAbsend = vecSrc;
	vecAbsend += vecMove;
	TraceHull(vecSrc, vecAbsend, tr);
}

void Features::GrenadePrediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, const Vector& vecMove, float flInterval)
{
	if (tr.pHitEntity)
	{
		if (AutoWall::IsBreakableEntity(tr.pHitEntity))
		{
			auto client_class = tr.pHitEntity->GetClientClass();

			if (!client_class)
				return;

			auto network_name = client_class->m_pNetworkName;
			if (((CPlayer*)(tr.pHitEntity))->IsPlayer() && strcmp(network_name, "CBaseEntity"))
			{
				PushEntity(tr.vecEnd, vecMove, tr);
				vecVelocity *= 0.1f;
				return;
			}

			if (tr.bAllSolid || tr.DidHit() || strcmp(network_name, "CFuncBrush") && strcmp(network_name, "CFuncBrush") && strcmp(network_name, "CBaseDoor") && strcmp(network_name, "CCSPlayer")) //-V526
			{
				PushEntity(tr.vecEnd, vecMove, tr);
				vecVelocity *= 0.4f;
				return;
			}
		}
	}

	float flSurfaceElasticity = 1.0, flGrenadeElasticity = 0.45f;
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.vecNormal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f;

	if (flSpeedSqr < flMinSpeedSqr)
	{
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	if (tr.plane.vecNormal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.flFraction) * flInterval);
		PushEntity(tr.vecEnd, vecAbsVelocity, tr);
	}
	else
		vecVelocity = vecAbsVelocity;
}

int Features::GrenadePrediction::PhysicsClipVelocity(const Vector& vecIn, const Vector& vecNormal, Vector& vecOut, float flOverBounce)
{
	static const float STOP_EPSILON = 0.1f;

	float backoff, change, angle;
	int   i, blocked;

	blocked = 0;
	angle = vecNormal[2];

	if (angle > 0) blocked |= 1;
	if (!angle) blocked |= 2;

	backoff = vecIn.Dot(vecNormal) * flOverBounce;
	for (i = 0; i < 3; i++)
	{
		change = vecNormal[i] * backoff;
		vecOut[i] = vecIn[i] - change;
		if (vecOut[i] > -STOP_EPSILON && vecOut[i] < STOP_EPSILON)
			vecOut[i] = 0;
	}
	return blocked;
}