#pragma once

#include "../SDK/CGameEvent.h"
#include "../Utilities/Color.h"
#include "../Utilities/Interfaces.h"
#include "../Menu/Config/Vars.h"

#include <vector>
#include <string>

namespace Features
{
	namespace Events
	{
		class ColorLine
		{
		public:
			ColorLine() {}

			ColorLine(std::string szText)
			{
				m_Texts.push_back(szText);
				m_Colors.push_back(Vars::Visuals::Screen::m_colEventLogs.GetColor());
			}

			void PushBack(std::string szText)
			{
				m_Texts.push_back(szText);
				m_Colors.push_back(Vars::Visuals::Screen::m_colEventLogs.GetColor());
			}

			void PushFront(std::string szText)
			{
				m_Texts.insert(m_Texts.begin(), szText);
				m_Colors.insert(m_Colors.begin(), Vars::Visuals::Screen::m_colEventLogs.GetColor());
			}

			void ChangeAlpha(int nAlpha)
			{
				for (auto& color : m_Colors)
					color.Set<COLOR_A>(nAlpha);
			}

			std::string Text()
			{
				std::string szText;
				for (const auto String : m_Texts)
					szText += String;

				return szText;
			}

			void Draw(int x, int y);

		private:
			std::vector<std::string> m_Texts;
			std::vector<Color> m_Colors;
		};

		struct Notifications_t
		{
			Notifications_t()
			{
				m_flTime = Interfaces::m_pGlobals->m_flCurTime;
			}

			Notifications_t(std::string szText)
			{
				m_ColorLine = ColorLine(szText);
				m_flTime = Interfaces::m_pGlobals->m_flCurTime;
			}

			ColorLine m_ColorLine;
			float m_flTime;
		};

		void Draw();
		void Add(Notifications_t Notification);
		void OnFireEventClientSide(IGameEvent* pEvent);

		inline std::vector<Notifications_t> m_Notifications;
	}
}