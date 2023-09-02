#include "pch.h"


#include "MyLog.h"
#include "MainConfig.h"
#include "ImGuiCTX.h"
#include "ImGuiConfigUtils.h"


#include "EditableColorsets/EditableColorsets.h"
#include "PhysicsPatch/Hack_60fpsPhysicsPatch.h"
#include "FPV.h"



class MyFeatures
{
public:
    AutoAssembleWrapper<EditableColorsets> editableColorsets;
    AutoAssembleWrapper<ExampleCameraOverride> exampleCameraOverride;

    void DrawControls()
    {
        if (ImGui::Button("Save config file"))
        {
            WriteConfig(g_Config);
            MainConfig::WriteToFile();
        }
        if (ImGuiCTX::TabBar _tabbar{ "FeaturesBar" })
        {
            if (ImGuiCTX::Tab _mainTab{ "Colorsets" })
            {
                if (ImGuiCTX::WindowChild _{ "ColorsetsChild" })
                {
                    ImGui::DrawCheckboxForHack(editableColorsets, "Editable Colorsets");
                    DrawColorsetsControls();
                }
            }
            if (ImGuiCTX::Tab _mainTab{ "Physics Patch" })
            {
                if (ImGuiCTX::WindowChild _{ "PhysicsChild" })
                {
                    DrawPhysicsPatchControls();
                }
            }
            if (ImGuiCTX::Tab _mainTab{ "Camera override" })
            {
                if (ImGuiCTX::WindowChild _{ "CamOvrdChild" })
                {
                    ImGui::DrawCheckboxForHack(exampleCameraOverride, "Example camera override");
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip(
                            "As an example, I set the camera position to 2 meters above the player's feet."
                        );
                    }
                    if (exampleCameraOverride.IsActive())
                    {
                        DrawFPVPatchControls();
                    }
                }
            }
        }
    }
    void ReadConfig(ConfigTop& cfg)
    {
        auto& featuresSection = cfg.features;
        editableColorsets.Toggle(featuresSection->editableColorsets);
    }
    void WriteConfig(ConfigTop& cfg)
    {
        auto& featuresSection = cfg.features;
        featuresSection->editableColorsets = editableColorsets.IsActive();
    }
};
std::optional<MyFeatures> g_MyHacks;
void WhenMenuOpen_DrawFeaturesControls()
{
    if (g_MyHacks)
    {
        g_MyHacks->DrawControls();
    }
}

void StartVariousFeatures()
{
    g_MyHacks.emplace();
    g_MyHacks->ReadConfig(g_Config);
    g_MyHacks->WriteConfig(g_Config);
    MainConfig::WriteToFile();
}
