#include "Utilities.h"
#include <vector>
#include "../SDK/CEntity.h"

uint8_t* Utilities::ScanSignature(HMODULE hModule, const char* szSignature)
{
    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)hModule;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)hModule + dosHeader->e_lfanew);

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte(szSignature);
    auto scanBytes = reinterpret_cast<std::uint8_t*>(hModule);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i)
    {
        bool bFound = true;
        for (auto j = 0ul; j < s; ++j) 
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                bFound = false;
                break;
            }
        }

        if (bFound)
            return &scanBytes[i];
    }

    return nullptr;
}

uint8_t* Utilities::ScanSignature(const char* szModule, const char* szSignature)
{
	return ScanSignature(GetModuleHandleA(szModule), szSignature);
}

const char* Utilities::Format(const char* szFmt, ...)
{
    char buf[1024];
    va_list va;

    va_start(va, szFmt);
    _vsnprintf_s(buf, 1024, szFmt, va);
    va_end(va);

    return buf;
}

Color Utilities::GetHealthColor(int nHealth)
{
    return Color(min(510 * (100 - nHealth) / 100, 255), min(510 * nHealth / 100, 255), 25);
}

const char8_t* Utilities::GetWeaponIcon(short nItemDefinitionIndex)
{
	/*
	 * @note: icon code = weapon item definition index in hex
	 * list of other icons:
	 *	"E210" - kevlar
	 *	"E20E" - helmet
	 *	"E20F" - defuser kit
	 *	"E211" - banner
	 *	"E212" - target
	 */
	switch (nItemDefinitionIndex)
	{
	case WEAPON_DEAGLE:
		return u8"\uE001";
	case WEAPON_ELITE:
		return u8"\uE002";
	case WEAPON_FIVESEVEN:
		return u8"\uE003";
	case WEAPON_GLOCK:
		return u8"\uE004";
	case WEAPON_AK47:
		return u8"\uE007";
	case WEAPON_AUG:
		return u8"\uE008";
	case WEAPON_AWP:
		return u8"\uE009";
	case WEAPON_FAMAS:
		return u8"\uE00A";
	case WEAPON_G3SG1:
		return u8"\uE00B";
	case WEAPON_GALILAR:
		return u8"\uE00D";
	case WEAPON_M249:
		return u8"\uE00E";
	case WEAPON_M4A1:
		return u8"\uE010";
	case WEAPON_MAC10:
		return u8"\uE011";
	case WEAPON_P90:
		return u8"\uE013";
	case WEAPON_MP5SD:
		return u8"\uE017";
	case WEAPON_UMP45:
		return u8"\uE018";
	case WEAPON_XM1014:
		return u8"\uE019";
	case WEAPON_BIZON:
		return u8"\uE01A";
	case WEAPON_MAG7:
		return u8"\uE01B";
	case WEAPON_NEGEV:
		return u8"\uE01C";
	case WEAPON_SAWEDOFF:
		return u8"\uE01D";
	case WEAPON_TEC9:
		return u8"\uE01E";
	case WEAPON_TASER:
		return u8"\uE01F";
	case WEAPON_HKP2000:
		return u8"\uE020";
	case WEAPON_MP7:
		return u8"\uE021";
	case WEAPON_MP9:
		return u8"\uE022";
	case WEAPON_NOVA:
		return u8"\uE023";
	case WEAPON_P250:
		return u8"\uE024";
	case WEAPON_SCAR20:
		return u8"\uE026";
	case WEAPON_SG553:
		return u8"\uE027";
	case WEAPON_SSG08:
		return u8"\uE028";
	case WEAPON_KNIFE:
		return u8"\uE02A";
	case WEAPON_FLASHBANG:
		return u8"\uE02B";
	case WEAPON_HEGRENADE:
		return u8"\uE02C";
	case WEAPON_SMOKEGRENADE:
		return u8"\uE02D";
	case WEAPON_MOLOTOV:
		[[fallthrough]];
	case WEAPON_FIREBOMB:
		return u8"\uE02E";
	case WEAPON_DECOY:
		[[fallthrough]];
	case WEAPON_DIVERSION:
		return u8"\uE02F";
	case WEAPON_INCGRENADE:
		return u8"\uE030";
	case WEAPON_C4:
		return u8"\uE031";
	case WEAPON_HEALTHSHOT:
		return u8"\uE039";
	case WEAPON_KNIFE_GG:
		[[fallthrough]];
	case WEAPON_KNIFE_T:
		return u8"\uE03B";
	case WEAPON_M4A1_SILENCER:
		return u8"\uE03C";
	case WEAPON_USP_SILENCER:
		return u8"\uE03D";
	case WEAPON_CZ75A:
		return u8"\uE03F";
	case WEAPON_REVOLVER:
		return u8"\uE040";
	case WEAPON_TAGRENADE:
		return u8"\uE044";
	case WEAPON_FISTS:
		return u8"\uE045";
	case WEAPON_TABLET:
		return u8"\uE048";
	case WEAPON_MELEE:
		return u8"\uE04A";
	case WEAPON_AXE:
		return u8"\uE04B";
	case WEAPON_HAMMER:
		return u8"\uE04C";
	case WEAPON_SPANNER:
		return u8"\uE04E";
	case WEAPON_KNIFE_BAYONET:
		return u8"\uE1F4";
	case WEAPON_KNIFE_CSS:
		return u8"\uE1F7";
	case WEAPON_KNIFE_FLIP:
		return u8"\uE1F9";
	case WEAPON_KNIFE_GUT:
		return u8"\uE1FA";
	case WEAPON_KNIFE_KARAMBIT:
		return u8"\uE1FB";
	case WEAPON_KNIFE_M9_BAYONET:
		return u8"\uE1FC";
	case WEAPON_KNIFE_TACTICAL:
		return u8"\uE1FD";
	case WEAPON_KNIFE_FALCHION:
		return u8"\uE200";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return u8"\uE202";
	case WEAPON_KNIFE_BUTTERFLY:
		return u8"\uE203";
	case WEAPON_KNIFE_PUSH:
		return u8"\uE204";
	case WEAPON_KNIFE_CORD:
		return u8"\uE205";
	case WEAPON_KNIFE_CANIS:
		return u8"\uE206";
	case WEAPON_KNIFE_URSUS:
		return u8"\uE207";
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return u8"\uE208";
	case WEAPON_KNIFE_OUTDOOR:
		return u8"\uE209";
	case WEAPON_KNIFE_STILETTO:
		return u8"\uE20A";
	case WEAPON_KNIFE_WIDOWMAKER:
		return u8"\uE20B";
	case WEAPON_KNIFE_SKELETON:
		return u8"\uE20D";
	default:
		return u8"\u003F";
	}
}

const char* Utilities::GetWeaponName(short int nItemDefinitionIndex)
{
	switch (nItemDefinitionIndex)
	{
	case WEAPON_DEAGLE:					return "DEAGLE";
	case WEAPON_ELITE:					return "DUAL BERETTAS";
	case WEAPON_FIVESEVEN:				return "FIVE-SEVEN";
	case WEAPON_GLOCK:					return "GLOCK";
	case WEAPON_AK47:					return "AK-47";
	case WEAPON_AUG:					return "AUG";
	case WEAPON_AWP:					return "AWP";
	case WEAPON_FAMAS:					return "FAMAS";
	case WEAPON_G3SG1:					return "G3SG1";
	case WEAPON_GALILAR:				return "GAIL";
	case WEAPON_M249:					return "M249";
	case WEAPON_M4A1:					return "M4A1";
	case WEAPON_MAC10:					return "MAC-10";
	case WEAPON_P90:					return "P90";
	case WEAPON_ZONE_REPULSOR:			return "REPULSOR";
	case WEAPON_MP5SD:					return "MP5";
	case WEAPON_UMP45:					return "UMP-45";
	case WEAPON_XM1014:					return "XM1014";
	case WEAPON_BIZON:					return "PP-BIZON";
	case WEAPON_MAG7:					return "MAG-7";
	case WEAPON_NEGEV:					return "NEGEV";
	case WEAPON_SAWEDOFF:				return "SAWEDOFF";
	case WEAPON_TEC9:					return "TEC-9";
	case WEAPON_TASER:					return "TASER";
	case WEAPON_HKP2000:				return "P2000";
	case WEAPON_MP7:					return "MP7";
	case WEAPON_MP9:					return "MP9";
	case WEAPON_NOVA:					return "NOVA";
	case WEAPON_P250:					return "P250";
	case WEAPON_SHIELD:					return "SHIELD";
	case WEAPON_SCAR20:					return "SCAR-20";
	case WEAPON_SG553:					return "SG-553";
	case WEAPON_SSG08:					return "SCOUT";
	case WEAPON_FLASHBANG:				return "FLASHBANG";
	case WEAPON_HEGRENADE:				return "GRENADE";
	case WEAPON_SMOKEGRENADE:			return "SMOKE";
	case WEAPON_MOLOTOV:				return "MOLOTOV";
	case WEAPON_DECOY:					return "DECOY";
	case WEAPON_INCGRENADE:				return "INC NADE";
	case WEAPON_C4:						return "C4";
	case WEAPON_HEALTHSHOT:				return "HEALTHSHOT";
	case WEAPON_KNIFE_T:				return "KNIFE";
	case WEAPON_M4A1_SILENCER:			return "M4A1-S";
	case WEAPON_USP_SILENCER:			return "USP-S";
	case WEAPON_CZ75A:					return "CZ75-A";
	case WEAPON_REVOLVER:				return "REVOLVER";
	case WEAPON_TAGRENADE:				return "TA GRENADE";
	case WEAPON_FISTS:					return "FISTS";
	case WEAPON_BREACHCHARGE:			return "BREACH CHARGE";
	case WEAPON_TABLET:					return "TABLET";
	case WEAPON_MELEE:					return "KNIFE";
	case WEAPON_AXE:					return "AXE";
	case WEAPON_HAMMER:					return "HAMMER";
	case WEAPON_SPANNER:				return "SPANNER";
	case WEAPON_KNIFE_GHOST:			return "KNIFE";
	case WEAPON_FIREBOMB:				return "FIREBOMB";
	case WEAPON_DIVERSION:				return "DIVERSION";
	case WEAPON_FRAG_GRENADE:			return "FRAG NADE";
	case WEAPON_SNOWBALL:				return "SNOWBALL";
	case WEAPON_BUMPMINE:				return "BUMPMINE";
	case WEAPON_KNIFE_GG:
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_CSS:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
		return "KNIFE";
	default:
		return "Error";
	}
}