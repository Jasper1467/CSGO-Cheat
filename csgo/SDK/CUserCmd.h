#pragma once
#include "Vectors.h"
#include "CRC.h"

// Above this, bf_read::SetOverflowFlag gets called
// This happens in CServerGameClients::ProcessUsercmds
// server.dll 83 FE 3E
#define MAX_COMMANDS_PROCESSABLE 62

#define IN_ATTACK  (1 << 0)
#define IN_JUMP   (1 << 1)
#define IN_DUCK   (1 << 2)
#define IN_FORWARD  (1 << 3)
#define IN_BACK   (1 << 4)
#define IN_USE   (1 << 5)
#define IN_CANCEL  (1 << 6)
#define IN_LEFT   (1 << 7)
#define IN_RIGHT  (1 << 8)
#define IN_MOVELEFT  (1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2  (1 << 11)
#define IN_RUN   (1 << 12)
#define IN_RELOAD  (1 << 13)
#define IN_ALT1   (1 << 14)
#define IN_ALT2   (1 << 15)
#define IN_SCORE  (1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED  (1 << 17) // Player is holding the speed key
#define IN_WALK   (1 << 18) // Player holding walk key
#define IN_ZOOM   (1 << 19) // Zoom key for HUD zoom
#define IN_WEAPON1  (1 << 20) // weapon defines these bits
#define IN_WEAPON2  (1 << 21) // weapon defines these bits
#define IN_BULLRUSH  (1 << 22)
#define IN_GRENADE1  (1 << 23) // grenade 1
#define IN_GRENADE2  (1 << 24) // grenade 2
#define IN_LOOKSPIN  (1 << 25)

class CUserCmd
{
public:
	CUserCmd()
	{
		memset(this, 0, sizeof(*this));
	};

	virtual ~CUserCmd() {};

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;
		CRC32_Init(&crc);

		CRC32_ProcessBuffer(&crc, &m_nCommandNumber, sizeof(m_nCommandNumber));
		CRC32_ProcessBuffer(&crc, &m_nTickCount, sizeof(m_nTickCount));
		CRC32_ProcessBuffer(&crc, &m_angViewAngles, sizeof(m_angViewAngles));
		CRC32_ProcessBuffer(&crc, &m_vecAimDirection, sizeof(m_vecAimDirection));
		CRC32_ProcessBuffer(&crc, &m_flForwardMove, sizeof(m_flForwardMove));
		CRC32_ProcessBuffer(&crc, &m_flSideMove, sizeof(m_flSideMove));
		CRC32_ProcessBuffer(&crc, &m_flUpMove, sizeof(m_flUpMove));
		CRC32_ProcessBuffer(&crc, &m_nButtons, sizeof(m_nButtons));
		CRC32_ProcessBuffer(&crc, &m_nImpulse, sizeof(m_nImpulse));
		CRC32_ProcessBuffer(&crc, &m_nWeaponSelect, sizeof(m_nWeaponSelect));
		CRC32_ProcessBuffer(&crc, &m_nWeaponSubType, sizeof(m_nWeaponSubType));
		CRC32_ProcessBuffer(&crc, &m_nRandomSeed, sizeof(m_nRandomSeed));
		CRC32_ProcessBuffer(&crc, &m_nMousedX, sizeof(m_nMousedX));
		CRC32_ProcessBuffer(&crc, &m_nMousedY, sizeof(m_nMousedY));

		CRC32_Final(&crc);
		return crc;
	}

	void Reset()
	{
		m_nCommandNumber = 0;
		m_nTickCount = 0;
		m_angViewAngles.Reset();
		m_vecAimDirection.Reset();
		m_flForwardMove = 0.f;
		m_flSideMove = 0.f;
		m_flUpMove = 0.f;
		m_nButtons = 0;
		m_nImpulse = 0;
		m_nWeaponSelect = 0;
		m_nWeaponSubType = 0;
		m_nRandomSeed = 0;
		m_nMousedX = 0;
		m_nMousedY = 0;
		m_bHasBeenPredicted = false;
		m_angHeadAngles.Reset();
		m_vecHeadOffset.Reset();
	}

	int m_nCommandNumber;
	int m_nTickCount;
	QAngle m_angViewAngles;
	Vector m_vecAimDirection;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	int m_nButtons;
	uint8_t m_nImpulse;
	int m_nWeaponSelect;
	int m_nWeaponSubType;
	int m_nRandomSeed;
	__int16 m_nMousedX;
	__int16 m_nMousedY;
	bool m_bHasBeenPredicted;
	QAngle m_angHeadAngles;
	Vector m_vecHeadOffset;
};

struct CVerifiedUserCmd
{
	CUserCmd m_Cmd;
	CRC32_t m_nCrc;
};