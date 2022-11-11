#pragma once

#include <cstdint>


#define MAX_EVENT_NAME_LENGTH	32 // max game event name length
#define MAX_EVENT_BITS 9 // max bits needed for an event index
#define MAX_EVENT_NUMBER (1 << MAX_EVENT_BITS) // max number of events allowed
#define MAX_EVENT_BYTES 1024 // max size in bytes for a serialized event

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class IGameEventVisitor2;
class IGameEvent
{
public:
	virtual ~IGameEvent() { }
	virtual const char* GetName() const = 0;

	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char* szKeyName = nullptr) = 0;

	virtual bool GetBool(const char* szKeyName = nullptr, bool bDefault = false) = 0;
	virtual int GetInt(const char* szKeyName = nullptr, int nDefault = 0) = 0;
	virtual uint64_t GetUint64(const char* szKeyName = nullptr, uint64_t nDefault = 0) = 0;
	virtual float GetFloat(const char* szKeyName = nullptr, float flDefault = 0.f) = 0;
	virtual const char* GetString(const char* szKeyName = nullptr, const char* szDefault = "") = 0;
	virtual const wchar_t* GetWString(const char* szKeyName = nullptr, const wchar_t* szDefault = L"") = 0;
	virtual const void* GetPtr(const char* szKeyName = nullptr) const = 0;

	virtual void SetBool(const char* szKeyName, bool bValue) = 0;
	virtual void SetInt(const char* szKeyName, int nValue) = 0;
	virtual void SetUint64(const char* szKeyName, uint64_t nValue) = 0;
	virtual void SetFloat(const char* szKeyName, float flValue) = 0;
	virtual void SetString(const char* szKeyName, const char* szValue) = 0;
	virtual void SetWString(const char* szKeyName, const wchar_t* szValue) = 0;
	virtual void SetPtr(const char* szKeyName, const void* pValue) = 0;

	virtual bool ForEventData(IGameEventVisitor2* pVisitor) const = 0;
};

class CGameEventDescriptor
{
public:
	int m_nEventId;
	char m_UtlVec_pCGameEventCallback_Listeners[28];
	bool m_bLocal;
};

class KeyValues;
class CGameEvent
{
public:
	CGameEvent(CGameEventDescriptor* descriptor, const char* name);
	virtual ~CGameEvent() = 0;

	virtual const char* GetName() const = 0;
	virtual bool  IsEmpty(const char* keyName = NULL) const = 0;
	virtual bool  IsLocal() const = 0;
	virtual bool  IsReliable() const = 0;

	virtual bool  GetBool(const char* keyName = NULL, bool defaultValue = false) const = 0;
	virtual int   GetInt(const char* keyName = NULL, int defaultValue = 0) const = 0;
	virtual uint64_t GetUint64(const char* keyName = NULL, uint64_t defaultValue = 0) const = 0;
	virtual float GetFloat(const char* keyName = NULL, float defaultValue = 0.0f) const = 0;
	virtual const char* GetString(const char* keyName = NULL, const char* defaultValue = "") const = 0;
	virtual const wchar_t* GetWString(const char* keyName = NULL, const wchar_t* defaultValue = L"") const = 0;
	virtual const void* GetPtr(const char* keyName = NULL) const;

	virtual void SetBool(const char* keyName, bool value) = 0;
	virtual void SetInt(const char* keyName, int value) = 0;
	virtual void SetUint64(const char* keyName, uint64_t value) = 0;
	virtual void SetFloat(const char* keyName, float value) = 0;
	virtual void SetString(const char* keyName, const char* value) = 0;
	virtual void SetWString(const char* keyName, const wchar_t* value) = 0;
	virtual void SetPtr(const char* keyName, const void* value) = 0;

	virtual bool ForEventData(IGameEventVisitor2* visitor) const = 0;

	CGameEventDescriptor* m_pDescriptor;
	KeyValues* m_pDataKeys;
};

class IGameEventListener2
{
public:
	virtual ~IGameEventListener2() {}
	virtual void FireGameEvent(IGameEvent* pEvent) = 0;
	virtual int GetEventDebugID() = 0;
};

class CSVCMsg_GameEvent;
class IGameEventManager2
{
public:
	virtual ~IGameEventManager2() { }
	virtual int LoadEventsFromFile(const char* szFileName) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(IGameEventListener2* pListener, const char* szName, bool bServerSide) = 0;
	virtual bool FindListener(IGameEventListener2* pListener, const char* szName) = 0;
	virtual void RemoveListener(IGameEventListener2* pListener) = 0;
	virtual void AddListenerGlobal(IGameEventListener2* pListener, bool bServerSide) = 0;
	virtual IGameEvent* CreateNewEvent(const char* szName, bool bForce = false, int* unknown = nullptr) = 0;
	virtual bool FireEvent(IGameEvent* pEvent, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(IGameEvent* pEvent) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent* pEvent) = 0;
	virtual void FreeEvent(IGameEvent* pEvent) = 0;
	virtual bool SerializeEvent(IGameEvent* pEvent, CSVCMsg_GameEvent* pEventMsg) = 0;
	virtual IGameEvent* UnserializeEvent(const CSVCMsg_GameEvent& eventMsg) = 0;
	virtual KeyValues* GetEventDataTypes(IGameEvent* pEvent) = 0;
};

class CGameEventManager : public IGameEventManager2
{
public:
	enum
	{
		SERVERSIDE = 0,		// this is a server side listener, event logger etc
		CLIENTSIDE,			// this is a client side listenet, HUD element etc
		CLIENTSTUB,			// this is a serverside stub for a remote client listener (used by engine only)
		SERVERSIDE_OLD,		// legacy support for old server event listeners
		CLIENTSIDE_OLD,		// legecy support for old client event listeners
	};

	enum
	{
		TYPE_LOCAL = 0,	// not networked
		TYPE_STRING,	// zero terminated ASCII string
		TYPE_FLOAT,		// float 32 bit
		TYPE_LONG,		// signed int 32 bit
		TYPE_SHORT,		// signed int 16 bit
		TYPE_BYTE,		// unsigned int 8 bit
		TYPE_BOOL,		// unsigned int 1 bit
		TYPE_UINT64,	// unsigned int 64 bit
		TYPE_WSTRING,	// zero terminated wide char string
		TYPE_COUNT,
	};
};

class IGameEventListener
{
public:
	virtual	~IGameEventListener(void) {};

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent(KeyValues* pEvent) = 0;
};

