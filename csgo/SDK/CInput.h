#pragma once

#include "CUserCmd.h"

#include "VFunc.h"

#define MULTIPLAYER_BACKUP 150

class CInput
{
public:
	unsigned pad0[169];
    bool m_cameraInThirdPerson;
    unsigned char pad1[2];
    Vector m_cameraOffset;
    unsigned char pad2[56];
    CUserCmd* m_commands;
    CVerifiedUserCmd* m_verifiedCommands;

	[[nodiscard]] CUserCmd* GetUserCmd(const int nSequenceNumber) const
	{
		return &m_commands[nSequenceNumber % MULTIPLAYER_BACKUP];
	}

	[[nodiscard]] CVerifiedUserCmd* GetVerifiedCmd(const int nSequenceNumber) const
	{
		return &m_verifiedCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
	}
};