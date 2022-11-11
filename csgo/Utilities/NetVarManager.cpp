#include "NetVarManager.h"

#include "Interfaces.h"

void NetVarManager::Initialize()
{
	for (auto pClientClass = Interfaces::m_pClient->GetAllClasses(); pClientClass != nullptr; pClientClass = pClientClass->m_pNext)
	{
		auto pTable = pClientClass->m_pRecvTable;

		for (int i = 0; i < pTable->m_nProps; i++)
		{
			auto Prop = pTable->m_pProps[i];
			m_Data.push_back(Data_t(FNV1A::Hash(pClientClass->m_pNetworkName), FNV1A::Hash(Prop.m_pVarName), Prop.m_Offset));
		}
	}
}

int NetVarManager::GetOffset(FNV1A_t nNetworkName, FNV1A_t nVarName)
{
	for (const auto& Data : m_Data)
	{
		if (Data.m_nNetworkName == nNetworkName && Data.m_nVarName == nVarName)
			return Data.m_nOffset;
	}
}

void NetVarManager::Dump()
{
	for (auto pClientClass = Interfaces::m_pClient->GetAllClasses(); pClientClass != nullptr; pClientClass = pClientClass->m_pNext)
	{
		auto pTable = pClientClass->m_pRecvTable;

		for (int i = 0; i < pTable->m_nProps; i++)
		{
			auto Prop = pTable->m_pProps[i];

			printf("%s | %s | %i \ %s\n", pClientClass->m_pNetworkName, Prop.m_pVarName, Prop.m_Offset, Prop.GetPropTypeName());
		}
	}
}