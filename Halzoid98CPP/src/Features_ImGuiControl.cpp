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
        {
            if (ImGuiCTX::Tab _mainTab{ "Colorsets" })
            {
                ImGui::DrawCheckboxForHack(editableColorsets, "Editable Colorsets");
                DrawColorsetsControls();
            }
            if (ImGuiCTX::Tab _mainTab{ "Physics Patch" })
            {
                DrawPhysicsPatchControls();
            }
            //if (ImGuiCTX::Tab _mainTab{ "Camera override" })
            //{
            //    {
            //        ImGui::DrawCheckboxForHack(exampleCameraOverride, "Example camera override");
            //        if (ImGui::IsItemHovered())
            //        {
            //            ImGui::SetTooltip(
            //                "As an example, I set the camera position to 2 meters above the player's feet."
            //            );
            //        }
            //        if (exampleCameraOverride.IsActive())
            //        {
            //            DrawFPVPatchControls();
            //        }
            //    }
            //}
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
void SaveFeaturesStateToConfig()
{

    if (g_MyHacks)
    {
        g_MyHacks->WriteConfig(g_Config);
    }
}
