#include "pch.h"

#include "vmath/vmath.h"
#include <vmath/vmath_extra.h>

#include "ImGuiCTX.h"


#include "MainConfig.h"

#include "Common_Plugins/Common_PluginSide.h"
void DrawModMenuControls()
{
    if (ImGui::Button("Open DLL's folder in File Explorer (has config)"))
    {
        system(("explorer \"" + g_ThisDLLAbsoluteFilepath.parent_path().string() + "\"").c_str());
    }
}

void WhenMenuOpen_DrawFeaturesControls();
void RequestUnloadThisPlugin();
#include "MainConfig.h"
void ImGuiLayer_WhenMenuIsOpen()
{
    if (ImGui::Button("Save config file"))
    {
        void SaveFeaturesStateToConfig(); SaveFeaturesStateToConfig();
        MainConfig::WriteToFile();
    }
    ImGui::Separator();
    if (ImGuiCTX::TabBar _tabbar{ "MainWindowTabs" })
    {
        WhenMenuOpen_DrawFeaturesControls();
        if (ImGuiCTX::Tab _extraoptions{ "Extra" })
        {
            DrawModMenuControls();
            ImGui::Separator();
            if (ImGui::Button("Unload this plugin"))
            {
                RequestUnloadThisPlugin();
            }
        }
    }
}
