#include "Animations.h"

#include "CEntity.h"
#include "../Utilities/Interfaces.h"
#include "../Utilities/Utilities.h"

float animstate_pose_param_cache_t::GetValue(CPlayer* pPlayer)
{
	if (!m_bInitialized)
	{
		Init(pPlayer, m_szName);
	}

	if (m_bInitialized && pPlayer)
	{
		Interfaces::m_pMDLCache->BeginLock();
		float flValue = pPlayer->GetPoseParameter(m_nIndex);
		Interfaces::m_pMDLCache->EndLock();
		return flValue;
	}

	return 0.f;
}

bool animstate_pose_param_cache_t::Init(CPlayer* pPlayer, const char* szName)
{
	Interfaces::m_pMDLCache->BeginLock();

	const char* v5 = szName;
	m_szName = szName;
	if (!pPlayer->GetModelPtr())
		v5 = szName;

	CStudioHdr* pModelPtr = pPlayer->GetModelPtr();
	if (!pModelPtr)
		pModelPtr = 0;

	m_nIndex = pPlayer->LookupPoseParameter(pModelPtr, v5);
	if (m_nIndex != -1)
		m_bInitialized = true;

	Interfaces::m_pMDLCache->EndLock();

	return m_bInitialized;
}