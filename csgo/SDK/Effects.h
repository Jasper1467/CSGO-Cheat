#pragma once

#include "Vectors.h"

class CEffectData
{
public:
	Vector m_vecOrigin; //0x0
	Vector m_vecStart; //0xC
	Vector m_vecNormal; //0x18
	QAngle m_angAngles; //0x24
	int	m_nFlags; //0x30
	int /*ClientEntityHandle_t*/ m_hEntity; //0x34
	int Pad; //0x38
	float m_flScale; //0x3C
	float m_flMagnitude; //0x40
	float m_flRadius; //0x44
	int	m_nAttachmentIndex; //0x48
	short m_nSurfaceProp; //0x4C
	short Pad2; //0x4E

	// Some TF2 specific things
	int	m_nMaterial; //0x50
	int	m_nDamageType; //0x54
	int	m_nHitBox; //0x58

	int	m_nOtherEntIndex; //0x5C

	unsigned char	m_nColor; //0x60
	bool m_bPositionsAreRelativeToEntity; //0x61
	char Pad3[2]; //0x62

	// Don't mess with stuff below here. DispatchEffect handles all of this.
public:
	CEffectData()
	{
		m_vecOrigin.Init();
		m_vecStart.Init();
		m_vecNormal.Init();
		m_angAngles.Init();

		m_nFlags = 0;
		m_hEntity = -1;
		m_flScale = 1.f;
		m_nAttachmentIndex = 0;
		m_nSurfaceProp = 0;

		m_flMagnitude = 0.0f;
		m_flRadius = 0.0f;

		m_nMaterial = 0;
		m_nDamageType = 0;
		m_nHitBox = 0;

		m_nColor = 0;

		m_nOtherEntIndex = 0;
	}

	int GetEffectNameIndex() { return m_iEffectName; }

	//private:

	int m_iEffectName;	//0x64 // Entry in the EffectDispatch network string table. The is automatically handled by DispatchEffect().
};

typedef void (*ClientEffectCallback)(const CEffectData& Data);

struct CClientEffectRegistration
{
	CClientEffectRegistration() = default;
	CClientEffectRegistration(const char* szEffectName, ClientEffectCallback fn) : m_szEffectName{ szEffectName }, m_Function{ fn } {}

	const char* m_szEffectName;
	ClientEffectCallback m_Function;
	CClientEffectRegistration* m_pNext;
};