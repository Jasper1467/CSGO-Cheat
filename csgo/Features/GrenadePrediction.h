#pragma once

#include "../SDK/Vectors.h"
#include "../Utilities/Color.h"
#include "../SDK/CEngineTrace.h"

#include <vector>

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

			NadePoint_t(Vector vecStart, Vector vecEnd, Vector vecNormal, bool bPlane, bool bValid, bool bDetonate)
			{
				m_vecStart = vecStart;
				m_vecEnd = vecEnd;
				m_vecNormal = vecNormal;
				m_bPlane = bPlane;
				m_bValid = bValid;
				m_bDetonate = bDetonate;
			}

			Vector m_vecStart;
			Vector m_vecEnd;
			Vector m_vecNormal;

			bool m_bValid;
			bool m_bPlane;
			bool m_bDetonate;
		};

		void Predict();
		bool Detonated(CWeapon* pWeapon, float flTime, trace_t& Trace);
		void Trace();
		void Draw();

		inline std::vector<NadePoint_t> m_Points = {};
		inline bool m_bPredicted = false;
	}
}