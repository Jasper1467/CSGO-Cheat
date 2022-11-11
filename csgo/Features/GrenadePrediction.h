#pragma once

#include "../SDK/Vectors.h"
#include <vector>
#include "../Utilities/Color.h"
#include "../SDK/CEngineTrace.h"

class CViewSetup;
class CWeapon;

namespace Features
{
	namespace GrenadePrediction
	{
		enum
		{
			ACT_NONE = 0,
			ACT_THROW,
			ACT_LOB,
			ACT_DROP,
		};

		struct NadePoint_t
		{
			NadePoint_t()
			{
				m_bValid = false;
			}

			NadePoint_t(Vector vecStart, Vector vecEnd, Vector vecNormal, bool bPlane, bool bValid, bool bDetonate, bool bGround)
			{
				m_vecStart = vecStart;
				m_vecEnd = vecEnd;
				m_vecNormal = vecNormal;
				m_bPlane = bPlane;
				m_bValid = bValid;
				m_bDetonate = bDetonate;
				m_bGround = bGround;
			}

			Vector m_vecStart;
			Vector m_vecEnd;
			Vector m_vecNormal;

			bool m_bValid;
			bool m_bPlane;
			bool m_bDetonate;
			bool m_bGround;
		};

		struct AfterPrediction_t
		{
			Vector m_vecStart;
			Vector m_vecEnd;
			bool m_bValid;
			bool m_bGround;
			bool m_bWall;
			bool m_bDetonate;
		};

		void Setup(Vector& vecSrc, Vector& vecThrow, const QAngle& angViewAngles);
		void Simulate(CViewSetup* pSetup);

		int Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval);
		bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int nTick, float flInterval);

		void TraceHull(Vector& vecSrc, Vector& vecEnd, trace_t& tr);
		void AddGravityMove(Vector& vecMove, Vector& vecVelocity, float flFrameTime, bool bOnGround);
		void PushEntity(Vector& vecSrc, const Vector& vecMove, trace_t& tr);
		void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, const Vector& vecMove, float flInterval);
		int PhysicsClipVelocity(const Vector& vecIn, const Vector& vecNormal, Vector& vecOut, float flOverBounce);

		void Tick(int nButtons);
		void View(CViewSetup* pSetup, CWeapon* pWeapon);
		void Paint();

		inline bool m_bPredicted = false;
		inline int m_nType = 0;
		inline int m_nAct = 0;
		
		inline std::vector<std::pair<Vector, Color>> m_Others = {};
		inline std::vector<AfterPrediction_t> m_AfterPrediction = {};
		inline std::array<NadePoint_t, 500> m_Points = {};
		inline std::vector<Vector> m_Path = {};
	}
}