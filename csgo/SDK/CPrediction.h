#pragma once
#include <cstdint>

#include "CGlobalVarsBase.h"
#include "CUserCmd.h"

#define MAX_PREDICTION_ERROR 64.f
#define MIN_PREDICTION_EPSILON 0.5f

class CMoveData
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	bool m_bNoAirControl : 1;
	uintptr_t m_hPlayerHandle;
	int m_nImpulseCommand;
	QAngle m_angViewAngles;
	QAngle m_angAbsViewAngles;
	int m_nButtons;
	int m_nOldButtons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	Vector m_vecVelocity;
	Vector m_vecTrailingVelocity;
	float m_flTrailingVelocityTime;
	Vector m_vecAngles;
	Vector m_vecOldAngles;
	float m_flOutStepHeight;
	Vector m_vecOutWishVel;
	Vector m_vecOutJumpVel;
	Vector m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	bool m_bConstraintPastRadius;
	Vector m_vecAbsOrigin;
};

class IMoveHelper;

class CPrediction
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(int nStartFrame, bool bValidFrame, int nIncomingAcknowledged, int nOutgoingCommand) = 0;

	virtual void PreEntityPacketReceived(int nCommandsAcknowledged, int nCurrentWorldUpdatePacket, int nServerTicksElapsed) = 0;
	virtual void PostEntityPacketReceived() = 0;
	virtual void PostNetworkDataReceived(int nCommandsAcknowledged) = 0;
	virtual void OnReceivedUncompressedPacket() = 0;

	virtual void GetViewOrigin(Vector& vecOrigin) = 0;
	virtual void SetViewOrigin(Vector& vecOrigin) = 0;

	virtual void GetViewAngles(QAngle& angAngles) = 0;
	virtual void SetViewAngles(QAngle& angAngles) = 0;

	virtual void GetLocalViewAngles(QAngle& angAngles) = 0;
	virtual void SetLocalViewAngles(QAngle& aangAnglesng) = 0;

	virtual bool InPrediction() = 0;
	virtual bool IsFirstTimePredicted() = 0;
	virtual int GetLastCommandAcknowledged() = 0;
	virtual int GetIncomingPacketNumber() = 0;

	virtual void CheckMovingGround(CPlayer* pPlayer, double dbFrameTime) = 0;

	virtual void RunCommand(CPlayer* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper) = 0;
	virtual void SetupMove(CPlayer* pPlayer, CUserCmd* pCmd, IMoveHelper* pHelper, CMoveData* pMove) = 0;

	virtual void FinishMove(CPlayer* pPlayer, CUserCmd* pCmd, CMoveData* pMove) = 0;

	virtual void SetIdealPitch(int nSlot, CPlayer* pPlayer, const Vector& vecOrigin, const QAngle& angAngles,
		const Vector& vecViewHeight) = 0;

	virtual void CheckError(int nSlot, CPlayer* pPlayer, int nCommandsAcknowledged) = 0;

	virtual void _Update(int nSlot, bool bReceivedNewWorldUpdate, bool bValidFrame, int nIncomingAckowledged, int nOutgoingCommand) = 0;

	struct Split_t
	{
		bool m_bIsFirstTimePredicted;
		char pad0[3];
		int m_nCommandsPredicted;
		int m_nServerCommandsAcknowledged;
		int m_nPreviousAckHadErrors;
		float m_flIdealPitch;
		int m_nLastCommandAcknowledged;
		bool m_bPreviousAckErrorTriggersFullLatchReset;
		void* m_EntitiesWithPredictionErrorsInLastAck;
		bool m_bPerformedTickShift;
	};

	uintptr_t m_hLastGround;
	bool m_bInPrediction;
	bool m_bIsFirstTimePredicted;
	bool m_bEnginePaused;
	bool m_bOldCLPredictValue;
	int m_nPreviousStartFrame;
	int m_nIncomingPacketNumber;
	float m_flLastServerWorldTimeStamp;
	Split_t m_Split[2];
	CGlobalVarsBase m_SavedVars;
	char pad[76];
	bool m_bPlayerOriginTypedescriptionSearched;
	char pad1[22];
	void* m_pPDumpPanel;
};