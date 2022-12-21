#pragma once

#include "Vectors.h"
#include "VFunc.h"

#include "Studio.h"
#include "Animations.h"
#include "CUtlVector.h"

#include "CEngineTrace.h"

#include "../Utilities/NetVarManager.h"

enum EFlags_e
{
	EFL_KILLME = 0x1,
	EFL_DORMANT = 0x2,
	EFL_NOCLIP_ACTIVE = 0x4,
	EFL_SETTING_UP_BONES = 0x8,
	EFL_KEEP_ON_RECREATE_ENTITIES = 0x10,
	EFL_HAS_PLAYER_CHILD = 0x10,
	EFL_DIRTY_SHADOWUPDATE = 0x20,
	EFL_NOTIFY = 0x40,
	EFL_FORCE_CHECK_TRANSMIT = 0x80,
	EFL_BOT_FROZEN = 0x100,
	EFL_SERVER_ONLY = 0x200,
	EFL_NO_AUTO_EDICT_ATTACH = 0x400,
	EFL_DIRTY_ABSTRANSFORM = 0x800,
	EFL_DIRTY_ABSVELOCITY = 0x1000,
	EFL_DIRTY_ABSANGVELOCITY = 0x2000,
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS = 0x4000,
	EFL_DIRTY_SPATIAL_PARTITION = 0x8000,
	EFL_IN_SKYBOX = 0x20000,
	EFL_USE_PARTITION_WHEN_NOT_SOLID = 0x40000,
	EFL_TOUCHING_FLUID = 0x80000,
	EFL_IS_BEING_LIFTED_BY_BARNACLE = 0x100000,
	EFL_NO_ROTORWASH_PUSH = 0x200000,
	EFL_NO_THINK_FUNCTION = 0x400000,
	EFL_NO_GAME_PHYSICS_SIMULATION = 0x800000,
	EFL_CHECK_UNTOUCH = 0x1000000,
	EFL_DONTBLOCKLOS = 0x2000000,
	EFL_DONTWALKON = 0x4000000,
	EFL_NO_DISSOLVE = 0x8000000,
	EFL_NO_MEGAPHYSCANNON_RAGDOLL = 0x10000000,
	EFL_NO_WATER_VELOCITY_CHANGE = 0x20000000,
	EFL_NO_PHYSCANNON_INTERACTION = 0x40000000,
	EFL_NO_DAMAGE_FORCES = 0x80000000,
};

enum EntityFlags_e
{
	FL_ONGROUND = 0x1,
	FL_DUCKING = 0x2,
	FL_ANIMDUCKING = 0x4,
	FL_WATERJUMP = 0x8,
	FL_ONTRAIN = 0x10,
	FL_INRAIN = 0x20,
	FL_FROZEN = 0x40,
	FL_ATCONTROLS = 0x80,
	FL_CLIENT = 0x100,
	FL_FAKECLIENT = 0x200,
	FL_INWATER = 0x400,
	FL_FLY = 0x800,
	FL_SWIM = 0x1000,
	FL_CONVEYOR = 0x2000,
	FL_NPC = 0x4000,
	FL_GODMODE = 0x8000,
	FL_NOTARGET = 0x10000,
	FL_AIMTARGET = 0x20000,
	FL_PARTIALGROUND = 0x40000,
	FL_STATICPROP = 0x80000,
	FL_GRAPHED = 0x100000,
	FL_GRENADE = 0x200000,
	FL_STEPMOVEMENT = 0x400000,
	FL_DONTTOUCH = 0x800000,
	FL_BASEVELOCITY = 0x1000000,
	FL_WORLDBRUSH = 0x2000000,
	FL_OBJECT = 0x4000000,
	FL_KILLME = 0x8000000,
	FL_ONFIRE = 0x10000000,
	FL_DISSOLVING = 0x20000000,
	FL_TRANSRAGDOLL = 0x40000000,
	FL_UNBLOCKABLE_BY_PLAYER = 0x80000000,
};

enum MoveType_e
{
	MOVETYPE_NONE = 0x0,
	MOVETYPE_ISOMETRIC = 0x1,
	MOVETYPE_WALK = 0x2,
	MOVETYPE_STEP = 0x3,
	MOVETYPE_FLY = 0x4,
	MOVETYPE_FLYGRAVITY = 0x5,
	MOVETYPE_VPHYSICS = 0x6,
	MOVETYPE_PUSH = 0x7,
	MOVETYPE_NOCLIP = 0x8,
	MOVETYPE_LADDER = 0x9,
	MOVETYPE_OBSERVER = 0xA,
	MOVETYPE_CUSTOM = 0xB,
	MOVETYPE_LAST = 0xB,
	MOVETYPE_MAX_BITS = 0x4,
};

enum WaterLevel_e
{
	WL_NotInWater = 0x0,
	WL_Feet = 0x1,
	WL_Waist = 0x2,
	WL_Eyes = 0x3,
};

enum LifeState_e
{
	LIFE_ALIVE = 0x0,
	LIFE_DYING = 0x1,
	LIFE_DEAD = 0x2,
	MAX_LIFESTATE = 0x3,
};

using CBaseHandle = std::uintptr_t; 
class IHandleEntity
{
public:
	virtual ~IHandleEntity() { }
	virtual void SetRefEHandle(const CBaseHandle& hRef) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class IClientUnknown;
class ICollideable
{
public:
	virtual IHandleEntity* GetEntityHandle() = 0;
	virtual const Vector& OBBMins() const = 0;
	virtual const Vector& OBBMaxs() const = 0;
	virtual void WorldSpaceTriggerBounds(Vector* pVecWorldMins, Vector* pVecWorldMaxs) const = 0;
	virtual bool TestCollision(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual int GetCollisionModelIndex() = 0;
	virtual const model_t* GetCollisionModel() = 0;
	virtual Vector& GetCollisionOrigin() const = 0;
	virtual QAngle& GetCollisionAngles() const = 0;
	virtual const matrix3x4_t& CollisionToWorldTransform() const = 0;
	virtual int GetSolid() const = 0;
	virtual int GetSolidFlags() const = 0;
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual int GetCollisionGroup() const = 0;

	/*
	 * @note: should be equivalent to C_BaseAnimating::ComputeHitboxSurroundingBox
	 * DOESNT NEEDED TRANSORMATION!
	 * and bugged when trying to get non-player entity box
	 */
	virtual void WorldSpaceSurroundingBounds(Vector* pVecMins, Vector* pVecMaxs) = 0;
	virtual unsigned int GetRequiredTriggerFlags() const = 0;
	virtual const matrix3x4_t* GetRootParentToWorldTransform() const = 0;
	virtual void* GetVPhysicsObject() const = 0;
};

class IClientAlphaProperty
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void SetAlphaModulation(std::uint8_t uAlpha) = 0;
	virtual void SetRenderFX(int nRenderFx, int nRenderMode, float flStartTime = FLT_MAX, float flDuration = 0.0f) = 0;
	virtual void SetFade(float flGlobalFadeScale, float flDistFadeStart, float flDistFadeEnd) = 0;
	virtual void SetDesyncOffset(int nOffset) = 0;
	virtual void EnableAlphaModulationOverride(bool bEnable) = 0;
	virtual void EnableShadowAlphaModulationOverride(bool bEnable) = 0;
	virtual void SetDistanceFadeMode(int nFadeMode) = 0;
};

class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class IClientThinkable;
class CEntity;
class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual CEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};

struct RenderableInstance_t
{
	std::uint8_t uAlpha;
};

using ClientShadowHandle_t = std::uint16_t;
using ClientRenderHandle_t = std::uint16_t;
using ModelInstanceHandle_t = std::uint16_t;
class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual Vector& GetRenderOrigin() = 0;
	virtual QAngle& GetRenderAngles() = 0;
	virtual bool ShouldDraw() = 0;
	virtual int GetRenderFlags() = 0;
	virtual bool IsTransparent() = 0;
	virtual ClientShadowHandle_t GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t& RenderHandle() = 0;
	virtual const model_t* GetModel() const = 0;
	virtual int DrawModel(int nFlags, const RenderableInstance_t& uInstance) = 0;
	virtual int GetBody() = 0;
	virtual void GetColorModulation(float* pColor) = 0;
	virtual bool LODTest() = 0;
	virtual bool SetupBones(matrix3x4_t* matBoneToWorldOut, int nMaxBones, int fBoneMask, float flCurrentTime) = 0;
	virtual void SetupWeights(const matrix3x4_t* matBoneToWorld, int nFlexWeightCount, float* flFlexWeights, float* flFlexDelayedWeights) = 0;
	virtual void DoAnimationEvents() = 0;
	virtual void* GetPVSNotifyInterface() = 0;
	virtual void GetRenderBounds(Vector& vecMins, Vector& vecMaxs) = 0;
	virtual void GetRenderBoundsWorldspace(Vector& vecMins, Vector& vecMaxs) = 0;
	virtual void GetShadowRenderBounds(Vector& vecMins, Vector& vecMaxs, int iShadowType) = 0;
	virtual bool ShouldReceiveProjectedTextures(int nFlags) = 0;
	virtual bool GetShadowCastDistance(float* pDistance, int iShadowType) const = 0;
	virtual bool GetShadowCastDirection(Vector* vecDirection, int iShadowType) const = 0;
	virtual bool IsShadowDirty() = 0;
	virtual void MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual int ShadowCastType() = 0;
	virtual void unused2() {}
	virtual void CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t GetModelInstance() = 0;
	virtual const matrix3x4_t& RenderableToWorldTransform() = 0;
	virtual int LookupAttachment(const char* pAttachmentName) = 0;
	virtual bool GetAttachment(int nIndex, Vector& vecOrigin, QAngle& angView) = 0;
	virtual bool GetAttachment(int nIndex, matrix3x4_t& matAttachment) = 0;
	virtual bool ComputeLightingOrigin(int nAttachmentIndex, Vector vecModelLightingCenter, const matrix3x4_t& matrix, Vector& vecTransformedLightingCenter) = 0;
	virtual float* GetRenderClipPlane() = 0;
	virtual int GetSkin() = 0;
	virtual void OnThreadedDrawSetup() = 0;
	virtual bool UsesFlexDelayedWeights() = 0;
	virtual void RecordToolMessage() = 0;
	virtual bool ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual std::uint8_t OverrideAlphaModulation(std::uint8_t uAlpha) = 0;
	virtual std::uint8_t OverrideShadowAlphaModulation(std::uint8_t uAlpha) = 0;
	virtual void* GetClientModelRenderable() = 0;
};

enum DataUpdateType_e : int
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

struct bf_read;

class ClientClass;
class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void					Release() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void					NotifyShouldTransmit(int iState) = 0;
	virtual void					OnPreDataChanged(DataUpdateType_e updateType) = 0;
	virtual void					OnDataChanged(DataUpdateType_e updateType) = 0;
	virtual void					PreDataUpdate(DataUpdateType_e updateType) = 0;
	virtual void					PostDataUpdate(DataUpdateType_e updateType) = 0;
	virtual void					OnDataUnchangedInPVS() = 0;
	virtual bool					IsDormant() const = 0;
	virtual int						GetIndex() const = 0;
	virtual void					ReceiveMessage(int classIndex, bf_read& msg) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void					SetDestroyedOnRecreateEntities() = 0;
};

class CClientThinkHandle;
using ClientThinkHandle_t = CClientThinkHandle*;
class IClientThinkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void ClientThink() = 0;
	virtual ClientThinkHandle_t		GetThinkHandle() = 0;
	virtual void SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void Release() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual const Vector& GetAbsOrigin() const = 0;
	virtual const QAngle& GetAbsAngles() const = 0;
	virtual void* GetMouth() = 0;
	virtual bool GetSoundSpatialization(struct SpatializationInfo_t& info) = 0;
	virtual bool IsBlurred() = 0;

	void SetAbsOrigin(const Vector& vecorigin);
	void SetAbsAngles(const QAngle& angView);
};

#include <cstdint>

// 2022-12-18 18:15:15.246968900 UTC

namespace hazedumper {
constexpr ::std::int64_t timestamp = 1671387315;
namespace netvars {
constexpr ::std::ptrdiff_t cs_gamerules_data = 0x0;
constexpr ::std::ptrdiff_t m_ArmorValue = 0x117CC;
constexpr ::std::ptrdiff_t m_Collision = 0x320;
constexpr ::std::ptrdiff_t m_CollisionGroup = 0x474;
constexpr ::std::ptrdiff_t m_Local = 0x2FCC;
constexpr ::std::ptrdiff_t m_MoveType = 0x25C;
constexpr ::std::ptrdiff_t m_OriginalOwnerXuidHigh = 0x31D4;
constexpr ::std::ptrdiff_t m_OriginalOwnerXuidLow = 0x31D0;
constexpr ::std::ptrdiff_t m_SurvivalGameRuleDecisionTypes = 0x1328;
constexpr ::std::ptrdiff_t m_SurvivalRules = 0xD00;
constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
constexpr ::std::ptrdiff_t m_aimPunchAngleVel = 0x3048;
constexpr ::std::ptrdiff_t m_angEyeAnglesX = 0x117D0;
constexpr ::std::ptrdiff_t m_angEyeAnglesY = 0x117D4;
constexpr ::std::ptrdiff_t m_bBombDefused = 0x29C0;
constexpr ::std::ptrdiff_t m_bBombPlanted = 0x9A5;
constexpr ::std::ptrdiff_t m_bBombTicking = 0x2990;
constexpr ::std::ptrdiff_t m_bFreezePeriod = 0x20;
constexpr ::std::ptrdiff_t m_bGunGameImmunity = 0x9990;
constexpr ::std::ptrdiff_t m_bHasDefuser = 0x117DC;
constexpr ::std::ptrdiff_t m_bHasHelmet = 0x117C0;
constexpr ::std::ptrdiff_t m_bInReload = 0x32B5;
constexpr ::std::ptrdiff_t m_bIsDefusing = 0x997C;
constexpr ::std::ptrdiff_t m_bIsQueuedMatchmaking = 0x74;
constexpr ::std::ptrdiff_t m_bIsScoped = 0x9974;
constexpr ::std::ptrdiff_t m_bIsValveDS = 0x7C;
constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
constexpr ::std::ptrdiff_t m_bStartedArming = 0x3400;
constexpr ::std::ptrdiff_t m_bUseCustomAutoExposureMax = 0x9D9;
constexpr ::std::ptrdiff_t m_bUseCustomAutoExposureMin = 0x9D8;
constexpr ::std::ptrdiff_t m_bUseCustomBloomScale = 0x9DA;
constexpr ::std::ptrdiff_t m_clrRender = 0x70;
constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
constexpr ::std::ptrdiff_t m_fAccuracyPenalty = 0x3340;
constexpr ::std::ptrdiff_t m_fFlags = 0x104;
constexpr ::std::ptrdiff_t m_flC4Blow = 0x29A0;
constexpr ::std::ptrdiff_t m_flCustomAutoExposureMax = 0x9E0;
constexpr ::std::ptrdiff_t m_flCustomAutoExposureMin = 0x9DC;
constexpr ::std::ptrdiff_t m_flCustomBloomScale = 0x9E4;
constexpr ::std::ptrdiff_t m_flDefuseCountDown = 0x29BC;
constexpr ::std::ptrdiff_t m_flDefuseLength = 0x29B8;
constexpr ::std::ptrdiff_t m_flFallbackWear = 0x31E0;
constexpr ::std::ptrdiff_t m_flFlashDuration = 0x10470;
constexpr ::std::ptrdiff_t m_flFlashMaxAlpha = 0x1046C;
constexpr ::std::ptrdiff_t m_flLastBoneSetupTime = 0x2928;
constexpr ::std::ptrdiff_t m_flLowerBodyYawTarget = 0x9ADC;
constexpr ::std::ptrdiff_t m_flNextAttack = 0x2D80;
constexpr ::std::ptrdiff_t m_flNextPrimaryAttack = 0x3248;
constexpr ::std::ptrdiff_t m_flSimulationTime = 0x268;
constexpr ::std::ptrdiff_t m_flTimerLength = 0x29A4;
constexpr ::std::ptrdiff_t m_hActiveWeapon = 0x2F08;
constexpr ::std::ptrdiff_t m_hBombDefuser = 0x29C4;
constexpr ::std::ptrdiff_t m_hMyWeapons = 0x2E08;
constexpr ::std::ptrdiff_t m_hObserverTarget = 0x339C;
constexpr ::std::ptrdiff_t m_hOwner = 0x29DC;
constexpr ::std::ptrdiff_t m_hOwnerEntity = 0x14C;
constexpr ::std::ptrdiff_t m_hViewModel = 0x3308;
constexpr ::std::ptrdiff_t m_iAccountID = 0x2FD8;
constexpr ::std::ptrdiff_t m_iClip1 = 0x3274;
constexpr ::std::ptrdiff_t m_iCompetitiveRanking = 0x1A84;
constexpr ::std::ptrdiff_t m_iCompetitiveWins = 0x1B88;
constexpr ::std::ptrdiff_t m_iCrosshairId = 0x11838;
constexpr ::std::ptrdiff_t m_iDefaultFOV = 0x333C;
constexpr ::std::ptrdiff_t m_iEntityQuality = 0x2FBC;
constexpr ::std::ptrdiff_t m_iFOV = 0x31F4;
constexpr ::std::ptrdiff_t m_iFOVStart = 0x31F8;
constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
constexpr ::std::ptrdiff_t m_iHealth = 0x100;
constexpr ::std::ptrdiff_t m_iItemDefinitionIndex = 0x2FBA;
constexpr ::std::ptrdiff_t m_iItemIDHigh = 0x2FD0;
constexpr ::std::ptrdiff_t m_iMostRecentModelBoneCounter = 0x2690;
constexpr ::std::ptrdiff_t m_iObserverMode = 0x3388;
constexpr ::std::ptrdiff_t m_iShotsFired = 0x103E0;
constexpr ::std::ptrdiff_t m_iState = 0x3268;
constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
constexpr ::std::ptrdiff_t m_nBombSite = 0x2994;
constexpr ::std::ptrdiff_t m_nFallbackPaintKit = 0x31D8;
constexpr ::std::ptrdiff_t m_nFallbackSeed = 0x31DC;
constexpr ::std::ptrdiff_t m_nFallbackStatTrak = 0x31E4;
constexpr ::std::ptrdiff_t m_nForceBone = 0x268C;
constexpr ::std::ptrdiff_t m_nModelIndex = 0x258;
constexpr ::std::ptrdiff_t m_nTickBase = 0x3440;
constexpr ::std::ptrdiff_t m_nViewModelIndex = 0x29D0;
constexpr ::std::ptrdiff_t m_rgflCoordinateFrame = 0x444;
constexpr ::std::ptrdiff_t m_szCustomName = 0x304C;
constexpr ::std::ptrdiff_t m_szLastPlaceName = 0x35C4;
constexpr ::std::ptrdiff_t m_thirdPersonViewAngles = 0x31E8;
constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
constexpr ::std::ptrdiff_t m_vecVelocity = 0x114;
constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
constexpr ::std::ptrdiff_t m_viewPunchAngle = 0x3030;
constexpr ::std::ptrdiff_t m_zoomLevel = 0x33E0;
} // namespace netvars
namespace signatures {
constexpr ::std::ptrdiff_t anim_overlays = 0x2990;
constexpr ::std::ptrdiff_t clientstate_choked_commands = 0x4D30;
constexpr ::std::ptrdiff_t clientstate_delta_ticks = 0x174;
constexpr ::std::ptrdiff_t clientstate_last_outgoing_command = 0x4D2C;
constexpr ::std::ptrdiff_t clientstate_net_channel = 0x9C;
constexpr ::std::ptrdiff_t convar_name_hash_table = 0x301A0;
constexpr ::std::ptrdiff_t dwClientState = 0x59F194;
constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;
constexpr ::std::ptrdiff_t dwClientState_IsHLTV = 0x4D48;
constexpr ::std::ptrdiff_t dwClientState_Map = 0x28C;
constexpr ::std::ptrdiff_t dwClientState_MapDirectory = 0x188;
constexpr ::std::ptrdiff_t dwClientState_MaxPlayer = 0x388;
constexpr ::std::ptrdiff_t dwClientState_PlayerInfo = 0x52C0;
constexpr ::std::ptrdiff_t dwClientState_State = 0x108;
constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D90;
constexpr ::std::ptrdiff_t dwEntityList = 0x4DFFF14;
constexpr ::std::ptrdiff_t dwForceAttack = 0x322DD10;
constexpr ::std::ptrdiff_t dwForceAttack2 = 0x322DD1C;
constexpr ::std::ptrdiff_t dwForceBackward = 0x322DD40;
constexpr ::std::ptrdiff_t dwForceForward = 0x322DD34;
constexpr ::std::ptrdiff_t dwForceJump = 0x52BBC9C;
constexpr ::std::ptrdiff_t dwForceLeft = 0x322DD4C;
constexpr ::std::ptrdiff_t dwForceRight = 0x322DD58;
constexpr ::std::ptrdiff_t dwGameDir = 0x63AD80;
constexpr ::std::ptrdiff_t dwGameRulesProxy = 0x532F4CC;
constexpr ::std::ptrdiff_t dwGetAllClasses = 0xE0BFDC;
constexpr ::std::ptrdiff_t dwGlobalVars = 0x59EE58;
constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x535A9D8;
constexpr ::std::ptrdiff_t dwInput = 0x525D4E8;
constexpr ::std::ptrdiff_t dwInterfaceLinkList = 0x99BC84;
constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA964;
constexpr ::std::ptrdiff_t dwMouseEnable = 0x5239148;
constexpr ::std::ptrdiff_t dwMouseEnablePtr = 0x5239118;
constexpr ::std::ptrdiff_t dwPlayerResource = 0x322C0C0;
constexpr ::std::ptrdiff_t dwRadarBase = 0x52369EC;
constexpr ::std::ptrdiff_t dwSensitivity = 0xDEEB80;
constexpr ::std::ptrdiff_t dwSensitivityPtr = 0xDEEB80;
constexpr ::std::ptrdiff_t dwSetClanTag = 0x8DA80;
constexpr ::std::ptrdiff_t dwViewMatrix = 0x4DF0D44;
constexpr ::std::ptrdiff_t dwWeaponTable = 0x525E5C4;
constexpr ::std::ptrdiff_t dwWeaponTableIndex = 0x326C;
constexpr ::std::ptrdiff_t dwYawPtr = 0xDEE910;
constexpr ::std::ptrdiff_t dwZoomSensitivityRatioPtr = 0xDF4380;
constexpr ::std::ptrdiff_t dwbSendPackets = 0xDD072;
constexpr ::std::ptrdiff_t dwppDirect3DDevice9 = 0xA62C0;
constexpr ::std::ptrdiff_t find_hud_element = 0x297EFBD0;
constexpr ::std::ptrdiff_t force_update_spectator_glow = 0x3D91CA;
constexpr ::std::ptrdiff_t interface_engine_cvar = 0x3FA9C;
constexpr ::std::ptrdiff_t is_c4_owner = 0x3E69E0;
constexpr ::std::ptrdiff_t m_bDormant = 0xED;
constexpr ::std::ptrdiff_t m_bIsLocalPlayer = 0x3628;
constexpr ::std::ptrdiff_t m_flSpawnTime = 0x103C0;
constexpr ::std::ptrdiff_t m_pStudioHdr = 0x2950;
constexpr ::std::ptrdiff_t m_pitchClassPtr = 0x5239040;
constexpr ::std::ptrdiff_t m_yawClassPtr = 0xDEE910;
constexpr ::std::ptrdiff_t model_ambient_min = 0x5A118C;
constexpr ::std::ptrdiff_t set_abs_angles = 0x1E72D0;
constexpr ::std::ptrdiff_t set_abs_origin = 0x1E7110;
} // namespace signatures
} // namespace hazedumper

class CEntity : public IClientEntity
{
public:
	Vector m_vecOrigin()
	{
		return *(Vector*)(uintptr_t(this) + hazedumper::netvars::m_vecOrigin);
	}

	int& m_fFlags()
	{
		return *(int*)(uintptr_t(this) + hazedumper::netvars::m_fFlags);
	}

	// Not working ):
	//GET_NETVAR(int, m_fFlags, "CBaseEntity", "m_fFlags");

	int& m_iHealth()
	{
		return *(int*)(uintptr_t(this) + hazedumper::netvars::m_iHealth);
	}

	int& m_iTeamNum()
	{
		return *(int*)(uintptr_t(this) + hazedumper::netvars::m_iTeamNum);
	}

	Vector& m_vecVelocity()
	{
		return *(Vector*)(uintptr_t(this) + hazedumper::netvars::m_vecVelocity);
	}

	matrix3x4_t m_rgflCoordinateFrame()
	{
		uintptr_t nOffset = hazedumper::netvars::m_CollisionGroup - 0x30;
		return *(matrix3x4_t*)(uintptr_t(this) + nOffset);
	}

	const char* GetClassname()
	{
		// @ida: client.dll @ [8B 01 FF 90 ? ? ? ? 90 + 0x4] / sizeof(std::uintptr_t)
		return CallVFunc<const char*>(this, 143);
	}

	bool IsPlayer()
	{
		// XRef: "effects/nightvision"
		return CallVFunc<bool>(this, 158);
	}

	Vector& m_vecViewOffset()
	{
		return *(Vector*)(uintptr_t(this) + 0x108);
	}

	Vector GetEyePos()
	{
		return m_vecOrigin() + m_vecViewOffset();
	}

	QAngle& m_angPunchAngle()
	{
		return *(QAngle*)(uintptr_t(this) + 0x303c);
	}
};

class CWeapon;

class CPlayer : public CEntity
{
public:

	bool& m_bSpottedByMask()
	{
		return *(bool*)(uintptr_t(this) + hazedumper::netvars::m_bSpottedByMask);
	}

	bool& m_bIsScoped()
	{
		return *(bool*)(uintptr_t(this) + hazedumper::netvars::m_bIsScoped);
	}

	int& m_lifeState()
	{
		return *(int*)(uintptr_t(this) + hazedumper::netvars::m_lifeState);
	}

	int& m_hActiveWeapon()
	{
		return *(int*)(uintptr_t(this) + hazedumper::netvars::m_hActiveWeapon);
	}

	int& m_nTickBase()
	{
		return *(int*)(uintptr_t(this) + hazedumper::netvars::m_nTickBase);
	}

	bool IsAlive()
	{
		return m_lifeState() == LIFE_ALIVE;
	}

	CStudioHdr* GetModelPtr()
	{
		return (CStudioHdr*)(uintptr_t(this) + 0xA54);
	}
	
	CCSGOPlayerAnimState* GetAnimationState()
	{
		uintptr_t nOffset = hazedumper::netvars::m_bIsScoped - 0x14;
		return (CCSGOPlayerAnimState*)(uintptr_t(this) + nOffset);
	}

	QAngle& m_angEyeAngles()
	{
		return *(QAngle*)(uintptr_t(this) + 0x117d0);
	}

	float& m_flSpawnTime()
	{
		return *(float*)(uintptr_t(this) + 0xA370);
	}

	QAngle& m_aimPunchAngle()
	{
		return *(QAngle*)(uintptr_t(this) + hazedumper::netvars::m_aimPunchAngle);
	}

	bool IsEnemy();
	float GetMaxSpeed();
	CWeapon* GetActiveWeapon();
	Vector GetBonePos(int nBone);
	CUtlVector<C_AnimationLayer>& GetAnimationOverlays();

	C_AnimationLayer* GetAnimationLayer(int nLayer)
	{
		if (nLayer >= 0 && nLayer < LAYER_COUNT)
			return &GetAnimationOverlays()[nLayer];

		return nullptr;
	}

	const char* GetPlayerName();
	bool HasC4();
	int LookupPoseParameter(CStudioHdr* pHdr, const char* szName);
	std::array<float, 24>& GetPoseParameter();
	float GetPoseParameter(int nIndex);
	void InvalidatePhysicsRecursive(int nBits);
	Vector GetHitboxPos(int nHitboxId);
	int LookupBone(const char* szBoneName);
};

enum ItemDefinitionIndex_t : short
{
	WEAPON_NONE,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_ZONE_REPULSOR,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG553,
	WEAPON_SSG08,
	WEAPON_KNIFE_GG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_CORD,
	WEAPON_KNIFE_CANIS,
	WEAPON_KNIFE_URSUS,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_OUTDOOR,
	WEAPON_KNIFE_STILETTO,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE,
	GLOVE_CT_SIDE,
	GLOVE_SPORTY,
	GLOVE_SLICK,
	GLOVE_LEATHER_WRAP,
	GLOVE_MOTORCYCLE,
	GLOVE_SPECIALIST,
	GLOVE_HYDRA,
};

enum WeaponType_t
{
	WEAPONTYPE_KNIFE,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SMG,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER,
	WEAPONTYPE_MACHINE_GUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACE_HOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN,
	WEAPONTYPE_MEDISHOT,
	WEAPONTYPE_FISTS,
	WEAPONTYPE_BREACH_CHARGE,
	WEAPONTYPE_BUMP_MINE,
	WEAPONTYPE_TABLET,
	WEAPONTYPE_MELEE
};

enum WeaponModelType_t
{
	WEAPONMODEL_UNRECOGNIZED = 0x0,
	WEAPONMODEL_VIEWMODEL = 0x1,
	WEAPONMODEL_DROPPEDMODEL = 0x2,
	WEAPONMODEL_UNCLASSIFIED = 0x4,
};

enum WeaponMode_t
{
	WEAPONMODE_PRIMARY = 0x0,
	WEAPONMODE_SECONDARY = 0x1,
};

// Ref: server.dll 0F 47 C8 80 7C 24 22 00
#define KNIFE_MAX_DOT_FOR_BACKSTAB 0.475f
// Ref: server.dll B9 30 00 00 00 0F 44 C1
#define KNIFE_RANGE_PRIMARY 48.f
// Ref: server.dll B8 20 00 00 00 B9 30 00 00 00
#define KNIFE_RANGE_SECONDARY 32.f

// Ref: server.dll F3 0F 59 1D ? ? ? ? F3 0F 5C E1
#define GRENADE_SECONDARY_TRANSITION 1.3f
// Ref: 
#define GRENADE_FAILSAFE_MAX_BOUNCES 20
// Ref: server.dll 72 04 6A 11
#define GRENADE_UNDERHAND_THRESHOLD 0.33f

class CCSWeaponData
{
public:
	int8_t pad0[20];
	int32_t iMaxClip1;
	int8_t pad1[12];
	int32_t iMaxReservedAmmo;
	int8_t pad2[96];
	char* szHudName;
	char* szWeaponName;
	int8_t pad3[56];
	int32_t iWeaponType;
	int8_t pad4[4];
	int32_t iWeaponPrice;
	int32_t iKillAward;
	int8_t pad5[20];
	uint8_t bFullAuto;
	int8_t pad6[3];
	int32_t iDamage;
	float_t flArmorRatio;
	int32_t iBullets;
	float_t flPenetration;
	int8_t pad7[8];
	float_t flRange;
	float_t flRangeModifier;
	int8_t pad8[16];
	uint8_t bHasSilencer;
	int8_t pad9[15];
	float_t flSpread;
	float_t flSpreadAlt;
	int8_t pad10[76];
	int32_t iRecoilSeed;
	int8_t pad11[32];

	bool IsGun() const
	{
		switch (this->iWeaponType)
		{
		case WEAPONTYPE_PISTOL:
		case WEAPONTYPE_SMG:
		case WEAPONTYPE_RIFLE:
		case WEAPONTYPE_SHOTGUN:
		case WEAPONTYPE_SNIPER:
		case WEAPONTYPE_MACHINE_GUN:
			return true;
		}

		return false;
	}
};

class CWeapon
{
public:
	short m_iItemDefinitionIndex()
	{
		return *(short*)(uintptr_t(this) + 0x2fba);
	}

	[[nodiscard]] float GetSpread()
	{
		return CallVFunc<float>(this, 453);
	}

	[[nodiscard]] float GetInaccuracy()
	{
		return CallVFunc<float>(this, 483);
	}
	
	int& m_iRecoilIndex()
	{
		return *(int*)(uintptr_t(this) + 0x3350);
	}

	float& m_flRecoilIndex()
	{
		return *(float*)(uintptr_t(this) + 0x3354);
	}

	CCSWeaponData* GetWeaponData();
};