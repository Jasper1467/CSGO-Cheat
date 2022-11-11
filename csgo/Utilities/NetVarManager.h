#pragma once

#include <Windows.h>
#include <vector>

#include "fnv1a.h"

namespace NetVarManager
{
	void Initialize();

	int GetOffset(FNV1A_t nNetworkName, FNV1A_t nVarName);

	void Dump();

	struct Data_t
	{
		Data_t(FNV1A_t nNetworkName, FNV1A_t nVarName, uintptr_t nOffset)
		{
			m_nNetworkName = nNetworkName;
			m_nVarName = nVarName;
			m_nOffset = nOffset;
		}

		FNV1A_t m_nNetworkName;
		FNV1A_t m_nVarName;
		uintptr_t m_nOffset;
	};

	inline std::vector<Data_t> m_Data = {};
}

#define GET_NETVAR(type, var, networkname, varname) \
		type& var() { return *(type*)(uintptr_t(this) + NetVarManager::GetOffset(FNV1A::Hash(networkname), FNV1A::Hash(varname))); };