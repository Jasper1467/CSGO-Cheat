#include "Menu.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_internal.h"
#include "../Dependencies/imgui/cpp/imgui_stdlib.h"
#include "Addons.h"

#include "Config/Vars.h"
#include "../Utilities/Color.h"
#include "../Utilities/Fonts.h"

#include "../Features/Visuals.h"

#include "imgui_tricks.h"

#include "../CompilerDefinitions.h"

const char* g_szTabNames[] =
{
    "Legitbot",
    "Ragebot",
    "Antiaim",
    "Visuals",
    "Misc",
    "Config"
};

enum
{
    TAB_LEGITBOT,
    TAB_RAGEBOT,
    TAB_ANTIAIM,
    TAB_VISUALS,
    TAB_MISC,
    TAB_CONFIG,
};

#define SIDEBAR_ITEM_WIDTH 150.f
#define SIDEBAR_ITEM_HEIGHT 50.f

template<size_t N>
void render_tabs(const char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    bool values[N] = { false };

    values[activetab] = true;

    for (auto i = 0; i < N; ++i) 
    {
        if (ImGuiEx::ToggleButton(names[i], &values[i], ImVec2{ w, h }))
            activetab = i;

        if (sameline && i < N - 1)
            ImGui::SameLine();
    }
}

ImVec2 GetSidebarSize()
{
    constexpr float padding = 10.0f;
    constexpr auto size_w = padding * 2.0f + SIDEBAR_ITEM_WIDTH;
    constexpr auto size_h = padding * 2.0f + (sizeof(g_szTabNames) / sizeof(char*)) * SIDEBAR_ITEM_HEIGHT;

    return ImVec2{ size_w, ImMax(325.0f, size_h) };
}

void RenderLegitbotTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

    ImGuiEx::BeginGroupBox("##body_content");
    {
        ImGui::Checkbox("Enabled", &Vars::LegitBot::m_bEnabled);
        ImGui::Checkbox("Trigger Bot", &Vars::LegitBot::m_bTriggerBot);
    }
    ImGuiEx::EndGroupBox();


    ImGui::PopStyleVar();
}

void RenderRagebotTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

    ImGuiEx::BeginGroupBox("##body_content");
    {

    }
    ImGuiEx::EndGroupBox();

    ImGui::PopStyleVar();
}

void RenderAntiaimTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

    ImGuiEx::BeginGroupBox("##body_content");
    {
        ImGui::Checkbox("Enabled", &Vars::AntiAim::m_bEnabled);

        const char* szPitchModeNames[] = { "None", "Down", "Up", "Jitter", "Fake Down", "Fake Up" };
        ImGui::Combo("Pitch", &Vars::AntiAim::m_nPitchMode, szPitchModeNames, IM_ARRAYSIZE(szPitchModeNames));

        const char* szFakeLagNames[] = { "Off", "Static", "Adaptive", "Random" };
        ImGui::Combo("Fake Lag", &Vars::AntiAim::m_nFakeLagMode, szFakeLagNames, IM_ARRAYSIZE(szFakeLagNames));

        if (Vars::AntiAim::m_nFakeLagMode == 1) // Static
        {
            ImGui::VerticalSeparator();
            ImGui::SliderInt("Amount", &Vars::AntiAim::m_nFakeLagModeStaticAmount, 0, 14);
        }
        else if (Vars::AntiAim::m_nFakeLagMode == 3) // Random 
        {
            ImGui::VerticalSeparator();
            ImGui::SliderInt("Min", &Vars::AntiAim::m_nFakeLagRandomMin, 0, 14);
            ImGui::SliderInt("Max", &Vars::AntiAim::m_nFakeLagRandomMax, 0, 14);
        }
    }
    ImGuiEx::EndGroupBox();

    //ImGui::PopStyleVar();
}

void RenderVisualsTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    static const char* szEspTabNames[] = { "PLAYERS", "WORLD", "SCREEN", "MISC" };
    static int nCurrentEspTab = 0;

    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    {
        render_tabs(szEspTabNames, nCurrentEspTab, group_w / _countof(szEspTabNames), 25.0f, true);
    }
    //ImGui::PopStyleVar();
    ImGuiEx::BeginGroupBox("##body_content");
    {
        if (nCurrentEspTab == 0) // Players
        {
            const char* szEspPlayerTabNames[] = { "ENEMIES", "TEAMMATES" };
            static int nCurrentEspPlayerTab = 0;

            //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            {
                render_tabs(szEspPlayerTabNames, nCurrentEspPlayerTab, group_w / _countof(szEspPlayerTabNames), 25.0f, true);
            }
            //ImGui::PopStyleVar();

            if (nCurrentEspPlayerTab == 0) // Enemies
            {
                //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

                ImGuiEx::BeginGroupBox("##body_content");
                {

                    ImGui::Spacing();

                    ImGui::Checkbox("Box Esp", &Vars::Visuals::Players::Enemies::m_bBoxEsp);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Box Esp Color", &Vars::Visuals::Players::Enemies::m_colBoxEsp);

                    ImGui::Checkbox("Player Name", &Vars::Visuals::Players::Enemies::m_bPlayerName);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Player Name Color", &Vars::Visuals::Players::Enemies::m_colName);

                    ImGui::Checkbox("Weapon Name", &Vars::Visuals::Players::Enemies::m_bWeaponName);
                    ImGui::SameLine();
                    ImGui::Checkbox("Icon", &Vars::Visuals::Players::Enemies::m_bWeaponIcon);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Weapon Name Color", &Vars::Visuals::Players::Enemies::m_colWeapon);

                    const char* szSnapLineModes[] = { "Off", "Head", "Origin" };
                    ImGui::Combo("Snap Lines", &Vars::Visuals::Players::Enemies::m_nSnapLines, szSnapLineModes, 
                        IM_ARRAYSIZE(szSnapLineModes));
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Snap Lines Color", &Vars::Visuals::Players::Enemies::m_colSnapLines);

                    ImGui::Checkbox("Is C4 Carrier", &Vars::Visuals::Players::Enemies::m_bHasC4);
                    ImGui::Checkbox("Health Flag", &Vars::Visuals::Players::Enemies::m_bHealthFlag);

                    ImGui::Checkbox("Glow", &Vars::Visuals::Players::Enemies::m_bGlow);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Glow Color", &Vars::Visuals::Players::Enemies::m_colGlow);

                    ImGui::Checkbox("Engine Radar", &Vars::Visuals::Players::Enemies::m_bEngineRadar);
                }
                ImGuiEx::EndGroupBox();

                //ImGui::PopStyleVar();
            }
            else if (nCurrentEspPlayerTab == 1) // Teammates
            {
                //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

                ImGuiEx::BeginGroupBox("##body_content");
                {
                    ImGui::Spacing();

                    ImGui::Checkbox("Box Esp", &Vars::Visuals::Players::Teammates::m_bBoxEsp);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Box Esp Color", &Vars::Visuals::Players::Teammates::m_colBoxEsp);

                    ImGui::Checkbox("Player Name", &Vars::Visuals::Players::Teammates::m_bPlayerName);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Player Name Color", &Vars::Visuals::Players::Teammates::m_colName);

                    ImGui::Checkbox("Weapon Name", &Vars::Visuals::Players::Teammates::m_bWeaponName);
                    ImGui::SameLine();
                    ImGui::Checkbox("Icon", &Vars::Visuals::Players::Teammates::m_bWeaponIcon);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Weapon Name Color", &Vars::Visuals::Players::Teammates::m_colWeapon);

                    const char* szSnapLineModes[] = { "Off", "Head", "Origin" };
                    ImGui::Combo("Snap Lines", &Vars::Visuals::Players::Teammates::m_nSnapLines, szSnapLineModes,
                        IM_ARRAYSIZE(szSnapLineModes));
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Snap Lines Color", &Vars::Visuals::Players::Teammates::m_colSnapLines);

                    ImGui::Checkbox("Is C4 Carrier", &Vars::Visuals::Players::Teammates::m_bHasC4);
                    ImGui::Checkbox("Health Flag", &Vars::Visuals::Players::Teammates::m_bHealthFlag);

                    ImGui::Checkbox("Glow", &Vars::Visuals::Players::Teammates::m_bGlow);
                    ImGui::SameLine();
                    ImGuiEx::ColorPicker("Glow Color", &Vars::Visuals::Players::Teammates::m_colGlow);
                }
                ImGuiEx::EndGroupBox();
                //ImGui::PopStyleVar();
            }
        }
        else if (nCurrentEspTab == 1) // World
        {
            //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

            ImGuiEx::BeginGroupBox("##body_content");
            {
                ImGui::Checkbox("World Modulation", &Vars::Visuals::World::m_bWorldModulation);
                ImGui::SameLine();
                ImGuiEx::ColorPicker("World Color", &Vars::Visuals::World::m_colWorldModulation);

                ImGui::Checkbox("Prop Transparency", &Vars::Visuals::World::m_bPropTransparency);
                if (Vars::Visuals::World::m_bPropTransparency)
                    ImGui::SliderFloat("Transparency", &Vars::Visuals::World::m_flPropTransperancy, 0.f, 255.f);

                ImGui::Checkbox("Grenade Prediction", &Vars::Visuals::World::m_bGrenadePrediction);
                ImGui::SameLine();
                ImGui::Checkbox("On Click", &Vars::Visuals::World::m_bGrenadePredictionOnClick);
            }
            ImGuiEx::EndGroupBox();
            
            //ImGui::PopStyleVar();
        }
        else if (nCurrentEspTab == 2) // Screen
        {
            //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

            ImGuiEx::BeginGroupBox("##body_content");
            {
                ImGui::Checkbox("No Scope", &Vars::Visuals::Screen::m_bNoScope);
                ImGui::SameLine();
                ImGuiEx::ColorPicker("No Scope Color", &Vars::Visuals::Screen::m_colNoScope);

                if (Vars::Visuals::Screen::m_bNoScope)
                    ImGui::SliderFloat("Thickness", &Vars::Visuals::Screen::m_flNoScopeThickness, 1.f, 10.f);

                const char* szCrosshairNames[] = { "Normal", "Static", "Recoil" };
                ImGui::Combo("Crosshair", &Vars::Visuals::Screen::m_nCrosshairMode, szCrosshairNames, IM_ARRAYSIZE(szCrosshairNames));

                ImGui::Checkbox("Remove Post Processing", &Vars::Visuals::Screen::m_bRemovePostProcessing);

                ImGui::Checkbox("Event Logs Screen", &Vars::Visuals::Screen::m_bEventLogs);
                ImGui::SameLine();
                ImGuiEx::ColorPicker("Event Logs Color", &Vars::Visuals::Screen::m_colEventLogs);

                ImGui::Checkbox("Hit Marker", &Vars::Visuals::Screen::m_bHitMarker);
                ImGui::SameLine();
                ImGuiEx::ColorPicker("Hit Marker Color", &Vars::Visuals::Screen::m_colHitMarker);

                ImGui::Checkbox("Hit Sound", &Vars::Visuals::Screen::m_bHitMarkerSound);
            }
            ImGuiEx::EndGroupBox();

            //ImGui::PopStyleVar();
        }
        else if (nCurrentEspTab == 3) // Misc
        {
            //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

            ImGuiEx::BeginGroupBox("##body_content");
            {
                ImGui::Checkbox("Fov", &Vars::Visuals::Misc::m_bFov);
                if (Vars::Visuals::Misc::m_bFov)
                    ImGui::SliderFloat("Fov Amount", &Vars::Visuals::Misc::m_flFov, 0.f, 120.f);

                ImGui::Checkbox("Mirror Cam", &Vars::Visuals::Misc::m_bMirrorCam);
            }
            ImGuiEx::EndGroupBox();

            //ImGui::PopStyleVar();
        }
    }
    ImGuiEx::EndGroupBox();
}
void RenderMiscTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    static const char* szMiscTabNames[] = { "MAIN", "MOVEMENT" };
    static int nCurrentMiscTab = 0;

    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    {
        render_tabs(szMiscTabNames, nCurrentMiscTab, group_w / _countof(szMiscTabNames), 25.0f, true);
    }
    //ImGui::PopStyleVar();
    ImGuiEx::BeginGroupBox("##body_content");
    {
        if (nCurrentMiscTab == 0) // Main
        {
            ImGui::Checkbox("Clan Tag", &Vars::Misc::m_bClanTag);

            const char* szCheatSafetyModes[] = { "None", "Untrusted Safe", "Source Mod Safe", "Matchmaking Safe" };
            ImGui::Combo("Cheat Safety", &Vars::Misc::m_nCheatSafety, szCheatSafetyModes, IM_ARRAYSIZE(szCheatSafetyModes));

            ImGuiEx::ColorPicker("Menu Accent", &Vars::Misc::m_colMenuAccent, true);
        }
        else if (nCurrentMiscTab == 1) // Movement
        {
            ImGui::Checkbox("Bunnyhop", &Vars::Misc::m_bBunnyHop);
            if (Vars::Misc::m_bBunnyHop)
                ImGui::SliderInt("Hit Chance", &Vars::Misc::m_nBunnyHopHitChance, 0, 100);

            const char* szAutoStraferModes[] = { "Off", "Straight", "Directional" };
            ImGui::Combo("Auto Strafer", &Vars::Misc::m_nAutoStrafer, szAutoStraferModes, IM_ARRAYSIZE(szAutoStraferModes));

            ImGui::Checkbox("Infinite Duck Stamina", &Vars::Misc::m_bInfiniteDuckStamina);
        }
    }
    ImGuiEx::EndGroupBox();
}

void RenderConfigTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });

    ImGuiEx::BeginGroupBox("##body_content");
    {

    }
    ImGuiEx::EndGroupBox();

    //ImGui::PopStyleVar();
}

void Menu::Render()
{
	ImGui::GetIO().MouseDrawCursor = m_bIsVisible;

	if (!m_bIsVisible)
		return;

    static int nSidebarTab = 0;

    auto& Style = ImGui::GetStyle();

    ImGui::SetNextWindowPos(ImVec2{ 30.f, 20.f }, ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2{ 1000, 700 }, ImGuiSetCond_Once);

    // Backup style stuff, style for menu and tab bar aren't the same
    ImVec2 BackupPadding = Style.WindowPadding;
    ImVec2 BackupSpacing = Style.ItemSpacing;
    ImVec4 BackupBorderColor = Style.Colors[ImGuiCol_Border];
    ImVec4 BackupButtonActiveColor = Style.Colors[ImGuiCol_ButtonActive];
    ImVec4 BackupButtonHoveredColor = Style.Colors[ImGuiCol_ButtonHovered];
    ImVec4 BackupTextColor = Style.Colors[ImGuiCol_Text];

    Style.WindowPadding = ImVec2(0, 0);//not

    Style.Colors[ImGuiCol_Border] = Vars::Misc::m_colMenuAccent.GetColor().GetVec4();

    ImVec4 LigthDarkColor = ImVec4(51.f / 255.f, 47.f / 255.f, 46.f / 255.f, 1.f);
    Style.Colors[ImGuiCol_ButtonActive] = LigthDarkColor;
    Style.Colors[ImGuiCol_ButtonHovered] = LigthDarkColor;

    // Make the color darker than the border
    ImVec4 col = Vars::Misc::m_colMenuAccent.GetColor().GetVec4() - ImVec4(0.1f, 0.1f, 0.1f, 0.f);

    Style.Colors[ImGuiCol_Text] = col;

    ImGui::Begin("##tabarea", &m_bIsVisible, ImVec2(0, 0), 0.f,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar);
    {
        //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        {
            render_tabs(g_szTabNames, nSidebarTab, 110, 170, false);
        }
        ImGui::End();
        //ImGui::PopStyleVar();
    }

    // Restore style for tabs
    Style.Colors[ImGuiCol_Text] = BackupTextColor;
    Style.Colors[ImGuiCol_ButtonActive] = BackupButtonActiveColor;
    Style.Colors[ImGuiCol_ButtonHovered] = BackupButtonHoveredColor;
    Style.Colors[ImGuiCol_Border] = BackupBorderColor;
    Style.WindowPadding = BackupPadding;
    Style.ItemSpacing = BackupSpacing;

    ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2{ 1000, 400 }, ImGuiSetCond_Once);

    if (ImGui::Begin("Cheat Menu",
        &m_bIsVisible,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar))
    {
        //ImGui::Text("Csgo Cheat | Build: %s", __DATE__);

        // Make the body the same vertical size as the sidebar
        // except for the width, which we will set to auto
        //auto size = ImVec2{ 0.f, GetSidebarSize().y };

        //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ Style.WindowPadding.x, Style.ItemSpacing.y });
        //ImGuiEx::BeginGroupBox("##body");
        {
            if (nSidebarTab == TAB_LEGITBOT)
                RenderLegitbotTab();
            else if (nSidebarTab == TAB_RAGEBOT)
                RenderRagebotTab();
            else if (nSidebarTab == TAB_ANTIAIM)
                RenderAntiaimTab();
            else if (nSidebarTab == TAB_VISUALS)
                RenderVisualsTab();
            else if (nSidebarTab == TAB_MISC)
                RenderMiscTab();
            else if (nSidebarTab == TAB_CONFIG)
                RenderConfigTab();
        }
        //ImGuiEx::EndGroupBox();
        ImGui::End();
        //ImGui::PopStyleVar();
    }

#ifdef DEVELOPER
    if (ImGui::Begin("Developer", &Menu::m_bIsVisible))
    {
        

        ImGui::End();
    }
#endif // DEVELOPER
}

void Menu::Initialize()
{
    ImGui::StyleColorsDark();

    auto& Style = ImGui::GetStyle();

    Style.WindowTitleAlign = ImVec2(0.5f, 0.5f); // Center text
    Style.ButtonTextAlign = ImVec2(0.5f, 0.5f); // Center text
    Style.SelectableTextAlign = ImVec2(0.5f, 0.5f); // Center text

    Color colAccent = Vars::Misc::m_colMenuAccent.GetColor();

    ImVec4 ActiveColor = colAccent.GetVec4();
    ImVec4 HoveredColor = ImVec4(140.f / 255.f, 4.f / 255.f, 4.f / 255.f, 1.f);
    ImVec4 DarkColor = ImVec4(33.f / 255.f, 33.f / 255.f, 33.f / 255.f, 1.f);
    ImVec4 LigthDarkColor = ImVec4(51.f / 255.f, 47.f / 255.f, 46.f / 255.f, 1.f);

    Style.Colors[ImGuiCol_Button] = DarkColor;
    Style.Colors[ImGuiCol_ButtonActive] = ActiveColor; //HoveredColor;
    Style.Colors[ImGuiCol_ButtonHovered] = ActiveColor; // HoveredColor;

    Style.Colors[ImGuiCol_CheckMark] = ActiveColor;

    Style.Colors[ImGuiCol_SliderGrab] = ActiveColor;
    Style.Colors[ImGuiCol_SliderGrabActive] = ActiveColor;

    Style.Colors[ImGuiCol_FrameBg] = LigthDarkColor;
    Style.Colors[ImGuiCol_FrameBgActive] = DarkColor;
    Style.Colors[ImGuiCol_FrameBgHovered] = DarkColor;

    Style.Colors[ImGuiCol_PopupBg] = DarkColor;

    Style.Colors[ImGuiCol_MenuBarBg] = DarkColor;

    Style.Colors[ImGuiCol_TextSelectedBg] = ActiveColor;

    Style.GrabRounding = 8.f;
    Style.WindowRounding = 5.f;
    Style.FrameRounding = 4.f;

    Style.PopupBorderSize = 3.f;
    Style.PopupRounding = 5.f;
}