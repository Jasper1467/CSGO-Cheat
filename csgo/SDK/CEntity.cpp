#include "CEntity.h"

#include "../Utilities/Utilities.h"
#include "../Utilities/Interfaces.h"
#include "../Utilities/Math.h"
#include "../Utilities/Client.h"

void IClientEntity::SetAbsOrigin(const Vector& vecorigin)
{
	using Fn = void(__thiscall*)(void*, const Vector&);
	((Fn)SCAN_SIGNATURE("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"))(this, vecorigin);
}

void IClientEntity::SetAbsAngles(const QAngle& angView)
{
	using Fn = void(__thiscall*)(void*, const QAngle&);
	((Fn)SCAN_SIGNATURE("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"))(this, angView);
}

CWeapon* CPlayer::GetActiveWeapon()
{
	return (CWeapon*)(Interfaces::m_pEntityList->GetClientEntityFromHandle(m_hActiveWeapon()));
}

std::array<float, 24>& CPlayer::GetPoseParameter()
{
	return *(std::array<float, 24>*)(uintptr_t(this + 0x2778));
}

float CPlayer::GetPoseParameter(int nIndex)
{
	using Fn = float(__thiscall*)(void*, int);
	return ((Fn)(Utilities::ScanSignature("client.dll", "E8 ? ? ? ? D9 45 08 5F") + 1))(this, nIndex);
}

int CPlayer::LookupPoseParameter(CStudioHdr* pHdr, const char* szName)
{
	using Fn = int(__thiscall*)(void*, CStudioHdr*, const char*);
	return ((Fn)Utilities::ScanSignature("client.dll", "55 8B EC 57 8B 7D 08 85 FF 75 08"))(this, pHdr, szName);
}

void CPlayer::InvalidatePhysicsRecursive(int nBits)
{
	using Fn = void(__thiscall*)(void*, int);
	return ((Fn)Utilities::ScanSignature("client.dll", "E8 ? ? ? ? 8B 86 10 03 00 00 83 F8 FF"))(this, nBits);
}

Vector CPlayer::GetHitboxPos(int nHitboxId)
{
	matrix3x4_t matBone[MAXSTUDIOBONES];

	if (SetupBones(matBone, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f))
	{
		auto pStudioModel = Interfaces::m_pModelInfo->GetStudiomodel(GetModel());
		if (pStudioModel)
		{
			auto pHitbox = pStudioModel->GetHitboxSet(0)->GetHitbox(nHitboxId);
			if (pHitbox)
			{
				Vector vecMins{}, vecMaxs{};

				vecMins = Math::VectorTransform(pHitbox->bbmin, matBone[pHitbox->bone]);
				vecMins = Math::VectorTransform(pHitbox->bbmax, matBone[pHitbox->bone]);

				return (vecMins + vecMaxs) / 2.f;
			}
		}
	}

	return Vector{};
}

const char* CPlayer::GetPlayerName()
{
	using Fn = const char* (__thiscall*)(void*);
	return ((Fn)Utilities::ScanSignature("client.dll", "56 8B ? ? ? ? ? 85 F6 74 21 8B 41"))(this);
}

bool CPlayer::HasC4()
{
	static auto fnHasC4
		= reinterpret_cast<bool(__thiscall*)(void*)>(
			Utilities::ScanSignature("client.dll", "56 8B F1 85 F6 74 31"));

	return fnHasC4(this);
}

int CPlayer::LookupBone(const char* szBoneName)
{
	using Fn = int(__thiscall*)(void*, const char*);
	return ((Fn)Utilities::ScanSignature("client.dll",
	"55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 0F 84"))
		(this, szBoneName);
}

bool CPlayer::IsEnemy()
{
	// TODO: deathmatch fix
	return Client::m_pLocal->m_iTeamNum() != this->m_iTeamNum() && Client::m_pLocal->GetIndex() != this->GetIndex();
}

float CPlayer::GetMaxSpeed()
{
	auto pWeapon = GetActiveWeapon();
	if (!pWeapon)
		return 250.f;

	auto pData = pWeapon->GetWeaponData();
	if (!pData)
		return 250.f;

	return 250.f;

	//return (pData-> == WEAPONTYPE_SNIPER) ? pData->m_flMaxPlayerSpeedAlt : pData->m_flMaxPlayerSpeed;
}

Vector CPlayer::GetBonePos(int nBone)
{
	matrix3x4_t matBone[MAXSTUDIOBONES];
	if (SetupBones(matBone, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f))
		return matBone[nBone][3];

	return Vector{};
}

CUtlVector<C_AnimationLayer>& CPlayer::GetAnimationOverlays()
{
	auto nOffset = SCAN_SIGNATURE("client.dll", "8B 89 ? ? ? ? 8D 0C D1") + 2;
	return *(CUtlVector<C_AnimationLayer>*)(uintptr_t(this) + nOffset);
}

////////////////////////////////////////////////////////////

CCSWeaponData* CWeapon::GetWeaponData()
{
	return Interfaces::m_pWeaponSystem->GetWeaponData(m_iItemDefinitionIndex());
}