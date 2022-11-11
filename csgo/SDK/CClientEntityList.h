#pragma once
#include <cstdint>

#include "CEntity.h"

class IClientNetworkable;
class IClientUnknown;
class IClientEntity;

class CClientEntityList
{
public:
	virtual IClientNetworkable* GetClientNetworkable(int nIndex) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle hNetworkable) = 0;
	virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle hUnknown) = 0;
	virtual IClientEntity* GetClientEntity(int nIndex) = 0;
	virtual IClientEntity* GetClientEntityFromHandle(CBaseHandle hEntity) = 0;
	virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int GetHighestEntityIndex() = 0;
	virtual void SetMaxEntities(int iMax) = 0;
	virtual int GetMaxEntities() = 0;
};