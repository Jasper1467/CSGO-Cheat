#include "Events.h"

#include "../Utilities/Interfaces.h"
#include "../Utilities/Client.h"
#include "../Utilities/Render.h"
#include "../Utilities/Utilities.h"
#include "../Menu/Config/Vars.h"

const char* GetHitgroup(int nHitgroup)
{
	switch (nHitgroup)
	{
	case HITGROUP_GENERIC:
		return "generic";
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_CHEST:
		return "chest";
	case HITGROUP_STOMACH:
		return "stomach";
	case HITGROUP_LEFTARM:
		return "left arm";
	case HITGROUP_RIGHTARM:
		return "right arm";
	case HITGROUP_LEFTLEG:
		return "left leg";
	case HITGROUP_RIGHTLEG:
		return "right leg";
	default:
		return "error";
	}
}

void Features::Events::ColorLine::Draw(int x, int y)
{
	for (int i = 0; i < m_Texts.size(); i++)
	{
		Renderer::Text(m_Texts[i], ImVec2(x, y), 20.f, m_Colors[i]);
		x += Fonts::m_pDefault->FontSize; // Update x pos for next text
	}
}

void Features::Events::Draw()
{
	if (!Vars::Visuals::Screen::m_bEventLogs)
		return;

	const float text_time = 5.f;
	const float text_fade_in_time = 0.3f;
	const float text_fade_out_time = 0.2f;

	const int max_lines_at_once = 10;

	const int ideal_height = 14;
	const int ideal_width = 20;

	const int slide_in_distance = 20;
	const int slide_out_distance = 20;
	const float slide_out_speed = 0.2f;

	if (m_Notifications.size() > max_lines_at_once)
		m_Notifications.erase(m_Notifications.begin() + max_lines_at_once, m_Notifications.end());

	for (int i = 0; i < m_Notifications.size(); i++)
	{
		auto& Notification = m_Notifications[i];

		float flTimeDelta = fabs(Interfaces::m_pGlobals->m_flCurTime - Notification.m_flTime);
		
		int nHeight = ideal_height + (16 * i);

		// Erase dead notifications
		if (flTimeDelta > text_time)
		{
			m_Notifications.erase(m_Notifications.begin() + i);
			break;
		}

		if (flTimeDelta > text_time - slide_out_speed)
			nHeight = nHeight + (((slide_out_speed - (text_time - flTimeDelta)) / slide_out_speed) * slide_out_distance);

		/// fade out
		if (flTimeDelta > text_time - text_fade_out_time)
			Notification.m_ColorLine.ChangeAlpha(255 - (((flTimeDelta - (text_time - text_fade_out_time)) / text_fade_out_time) * 255.f));
		/// fade in
		if (flTimeDelta < text_fade_in_time)
			Notification.m_ColorLine.ChangeAlpha((flTimeDelta / text_fade_in_time) * 255.f);

		int width = ideal_width;

		/// slide from left
		if (flTimeDelta < text_fade_in_time)
			width = (flTimeDelta / text_fade_in_time) * static_cast<float>(slide_in_distance) + (ideal_width - slide_in_distance);
		/// slider from right
		if (flTimeDelta > text_time - text_fade_out_time)
			width = ideal_width + (((flTimeDelta - (text_time - text_fade_out_time)) / text_fade_out_time) * static_cast<float>(slide_out_distance));

		Notification.m_ColorLine.Draw(width, nHeight);
	}
}

void Features::Events::Add(Notifications_t Notification)
{
	m_Notifications.insert(m_Notifications.begin(), Notification);
}

void Features::Events::OnFireEventClientSide(IGameEvent* pEvent)
{
	if (!Vars::Visuals::Screen::m_bEventLogs)
		return;

	const char* szEvent = pEvent->GetName();
	if (!strcmp(szEvent, "player_hurt"))
	{
		int nVictim = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		int nAttacker = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("attacker"));
		int nHealthRemaining = pEvent->GetInt("health");
		int nDmgHealth = pEvent->GetInt("dmg_health");
		int nHitgroup = pEvent->GetInt("hitgroup");

		// We're attacking someone
		if (nAttacker == Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t VictimInfo;
			Interfaces::m_pEngine->GetPlayerInfo(nVictim, &VictimInfo);

			Add(Notifications_t(Utilities::Format("Hurt %s in the %s for %i, %i remaining", VictimInfo.szName,
				GetHitgroup(nHitgroup), nDmgHealth, nHealthRemaining)));
		}
	}
	else if (!strcmp(szEvent, "item_purchase"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		const char* szWeapon = pEvent->GetString("weapon");

		if (nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t(Utilities::Format("%s bought item %s", Info.szName, szWeapon)));
		}
	}
	else if (!strcmp(szEvent, "bomb_beginplant"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		int nSite = pEvent->GetInt("site");

		if (nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t(Utilities::Format("%s started planting bomb at site %i", Info.szName, nSite)));
		}
	}
	else if (!strcmp(szEvent, "bomb_abortplant"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		int nSite = pEvent->GetInt("site");

		if (nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t(Utilities::Format("%s stopped planting bomb at site %i", Info.szName, nSite)));
		}
	}
	else if (!strcmp(szEvent, "bomb_planted"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		int nSite = pEvent->GetInt("site");

		if (nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t(Utilities::Format("%s planted bomb at site %i", Info.szName, nSite)));
		}
	}
	else if (!strcmp(szEvent, "bomb_defused"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		int nSite = pEvent->GetInt("site");

		if (nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t(Utilities::Format("%s has defused the bomb", Info.szName)));
		}
	}
	else if (!strcmp(szEvent, "bomb_begindefuse"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		int nSite = pEvent->GetInt("site");

		if (nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t(Utilities::Format("%s has started defusing the bomb", Info.szName)));
		}
	}
	else if (!strcmp(szEvent, "bomb_abortdefuse"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		int nSite = pEvent->GetInt("site");

		if (nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t(Utilities::Format("%s has stopped defusing the bomb", Info.szName)));
		}
	}
	else if (!strcmp(szEvent, "enter_bombzone"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		bool bHasBomb = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetBool("hasbomb"));
		bool bIsPlanted = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetBool("isplanted"));

		if (bHasBomb && !bIsPlanted && nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t("Bomb carrier has entered a bomb site"));
		}
	}
	else if (!strcmp(szEvent, "exit_bombzone"))
	{
		int nUserId = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
		bool bHasBomb = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetBool("hasbomb"));
		bool bIsPlanted = Interfaces::m_pEngine->GetPlayerForUserID(pEvent->GetBool("isplanted"));

		if (bHasBomb && !bIsPlanted && nUserId != Client::m_pLocal->GetIndex())
		{
			PlayerInfo_t Info;
			Interfaces::m_pEngine->GetPlayerInfo(nUserId, &Info);

			Add(Notifications_t("Bomb carrier has left a bomb site"));
		}
	}
}