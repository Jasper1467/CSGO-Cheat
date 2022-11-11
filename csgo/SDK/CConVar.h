#pragma once
#include "../Utilities/Color.h"
#include "VFunc.h"

// command to convars and concommands
enum ConVarFlag_e : int
{
	// convar systems
	FCVAR_NONE = 0,
	FCVAR_UNREGISTERED = (1 << 0),	// if this is set, don't add to linked list, etc.
	FCVAR_DEVELOPMENTONLY = (1 << 1),	// hidden in released products. flag is removed automatically if allow_development_cvars is defined.
	FCVAR_GAMEDLL = (1 << 2),	// defined by the game dll
	FCVAR_CLIENTDLL = (1 << 3),	// defined by the client dll
	FCVAR_HIDDEN = (1 << 4),	// hidden. doesn't appear in find or autocomplete. like developmentonly, but can't be compiled out.

	// convar only
	FCVAR_PROTECTED = (1 << 5),	// it's a server cvar, but we don't send the data since it's a password, etc.  sends 1 if it's not bland/zero, 0 otherwise as value
	FCVAR_SPONLY = (1 << 6),	// this cvar cannot be changed by clients connected to a multiplayer server.
	FCVAR_ARCHIVE = (1 << 7),	// set to cause it to be saved to vars.rc
	FCVAR_NOTIFY = (1 << 8),	// notifies players when changed
	FCVAR_USERINFO = (1 << 9),	// changes the client's info string
	FCVAR_CHEAT = (1 << 14),	// only useable in singleplayer / debug / multiplayer & sv_cheats
	FCVAR_PRINTABLEONLY = (1 << 10),	// this cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
	FCVAR_UNLOGGED = (1 << 11),	// if this is a fcvar_server, don't log changes to the log file / console if we are creating a log
	FCVAR_NEVER_AS_STRING = (1 << 12),	// never try to print that cvar

	// it's a convar that's shared between the client and the server.
	// at signon, the values of all such convars are sent from the server to the client (skipped for local client, ofc )
	// if a change is requested it must come from the console (i.e., no remote client changes)
	// if a value is changed while a server is active, it's replicated to all connected clients
	FCVAR_SERVER = (1 << 13),	// server setting enforced on clients, replicated
	FCVAR_DEMO = (1 << 16),	// record this cvar when starting a demo file
	FCVAR_DONTRECORD = (1 << 17),	// don't record these command in demofiles
	FCVAR_RELOAD_MATERIALS = (1 << 20),	// if this cvar changes, it forces a material reload
	FCVAR_RELOAD_TEXTURES = (1 << 21),	// if this cvar changes, if forces a texture reload
	FCVAR_NOT_CONNECTED = (1 << 22),	// cvar cannot be changed by a client that is connected to a server
	FCVAR_MATERIAL_SYSTEM_THREAD = (1 << 23),	// indicates this cvar is read from the material system thread
	FCVAR_ARCHIVE_XBOX = (1 << 24),	// cvar written to config.cfg on the xbox
	FCVAR_ACCESSIBLE_FROM_THREADS = (1 << 25),	// used as a debugging tool necessary to check material system thread convars
	FCVAR_SERVER_CAN_EXECUTE = (1 << 28),	// the server is allowed to execute this command on clients via clientcommand/net_stringcmd/cbaseclientstate::processstringcmd.
	FCVAR_SERVER_CANNOT_QUERY = (1 << 29),	// if this is set, then the server is not allowed to query this cvar's value (via iserverpluginhelpers::startquerycvarvalue).
	FCVAR_CLIENTCMD_CAN_EXECUTE = (1 << 30),	// ivengineclient::clientcmd is allowed to execute this command. 
	FCVAR_MATERIAL_THREAD_MASK = (FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD)
};

using CommandCallbackV1Fn = void(__cdecl*)();
using ChangeCallbackFn = void(__cdecl*)(void*, const char*, float);

class CConVar
{
public:
	const char* GetName()
	{
		return CallVFunc<const char*>(this, 5);
	}

	float GetFloat() // idx @ 11
	{
		std::uint32_t uXored = *reinterpret_cast<std::uint32_t*>(&pParent->flValue) ^ reinterpret_cast<std::uint32_t>(this);
		return *reinterpret_cast<float*>(&uXored);
	}

	int GetInt() // idx @ 12
	{
		return static_cast<int>(pParent->iValue ^ reinterpret_cast<int>(this));
	}

	bool GetBool() // idx @ 13
	{
		return !!GetInt();
	}

	const char* GetString() const
	{
		char const* szValue = pParent->szString;
		return szValue ? szValue : "";
	}

	void SetValue(const char* szValue)
	{
		return CallVFunc<void>(this, 14, szValue);
	}

	void SetValue(float flValue)
	{
		return CallVFunc<void>(this, 15, flValue);
	}

	void SetValue(int iValue)
	{
		return CallVFunc<void>(this, 16, iValue);
	}

	void SetValue(Color colValue)
	{
		return CallVFunc<void>(this, 17, colValue);
	}

public:
	std::byte						pad0[0x4];			//0x00
	CConVar* pNext;				//0x04
	bool							bRegistered;		//0x08
	const char* szName;				//0x0C
	const char* szHelpString;		//0x10
	int								nFlags;				//0x14
	CommandCallbackV1Fn			pCallback;			//0x18
	CConVar* pParent;			//0x1C
	const char* szDefaultValue;		//0x20
	char* szString;			//0x24
	int								iStringLength;		//0x28
	float							flValue;			//0x2C
	int								iValue;				//0x30
	bool							bHasMin;			//0x34
	float							flMinValue;			//0x38
	bool							bHasMax;			//0x3C
	float							flMaxValue;			//0x40
	CUtlVector<ChangeCallbackFn>	fnChangeCallbacks;	//0x44
};