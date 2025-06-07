#include "pch.h"

#include "Hack_60fpsPhysicsPatch.h"

#include "ACU/ManagedObject.h"
#include "ACU/SoftBodySettings.h"
#include "Experimental_StrongPtr.h"
#include "vmath/vmath.h"
#include "ImGuiCTX.h"
#include "MainConfig.h"

#include "SoftbodyDescriptions.h"


struct TrackedSoftbody
{
    const SoftbodyDescription& m_Desc;
    std::optional<ACUSharedPtr_Strong> m_Reference;
    bool m_IsAlreadyPatched = false;

    TrackedSoftbody(const SoftbodyDescription& desc) : m_Desc(desc) {}
};
std::vector<TrackedSoftbody>& GetSoftbodiesOfInterest()
{
    static std::vector<TrackedSoftbody> softbodiesOfInterest = []()
    {
        std::vector<TrackedSoftbody> result;
        result.reserve(g_SoftbodyDescriptions.size());
        for (const SoftbodyDescription& softbodyDesc : g_SoftbodyDescriptions)
        {
            TrackedSoftbody& newPatched = result.emplace_back(softbodyDesc);
        }
        return result;
    }();
    return softbodiesOfInterest;
}
bool g_PhysicsPatchHasBeenToggled = true;
void EveryFrameUntilSuccess_FindAndPatchSoftBodyData()
{
    static bool isEverythingPatched = false;
    const bool isPhysicsPatchHasJustBeenToggled = g_PhysicsPatchHasBeenToggled;
    if (isPhysicsPatchHasJustBeenToggled)
    {
        isEverythingPatched = false;
        g_PhysicsPatchHasBeenToggled = false;
    }
    if (isEverythingPatched) return;
    const bool _1applyPatch0ApplyDefaults = g_Config.features->applyClothPhysicsPatch;
    bool isEverythingPatchedThisTime = true;
    std::vector<TrackedSoftbody>& softbodiesOfInterest = GetSoftbodiesOfInterest();
    if (isPhysicsPatchHasJustBeenToggled)
        for (auto& sb : softbodiesOfInterest)
            sb.m_IsAlreadyPatched = false;
    for (TrackedSoftbody& softbody : softbodiesOfInterest)
    {
        if (softbody.m_IsAlreadyPatched)
        {
            continue;
        }
        if (!softbody.m_Reference)
        {
            softbody.m_Reference.emplace(softbody.m_Desc.m_Handle);
        }
        SoftBodySettings* sbs = softbody.m_Reference->GetPtr<SoftBodySettings>();
        if (!sbs)
        {
            isEverythingPatchedThisTime = false;
            continue;
        }
        const SoftBodySettings_Data& valuesToApply =
            _1applyPatch0ApplyDefaults
            ? softbody.m_Desc.m_HalzoidPatchValues
            : softbody.m_Desc.m_DefaultValues;
        sbs->m_data = valuesToApply;
        softbody.m_IsAlreadyPatched = true;
    }
    if (isEverythingPatchedThisTime)
    {
        isEverythingPatched = true;
    }
}
void DrawControlsForSoftbody(TrackedSoftbody& softbody)
{
    ImGuiCTX::PushID _id(&softbody);
    if (!softbody.m_Reference)
    {
        softbody.m_Reference.emplace(softbody.m_Desc.m_Handle);
    }
    SoftBodySettings* sbs = softbody.m_Reference->GetPtr<SoftBodySettings>();
    if (!sbs)
    {
        ImGui::Text("This SoftBodySettings object isn't loaded yet. Try equipping something that uses it.");
        return;
    }
    SoftBodySettings_Data& data = sbs->m_data;
    if (ImGui::Button("Apply defaults"))
    {
        data = softbody.m_Desc.m_DefaultValues;
    }
    ImGui::SameLine();
    if (ImGui::Button("Apply Halzoid98 60fps Patch"))
    {
        data = softbody.m_Desc.m_HalzoidPatchValues;
    }
    Draw_SoftBodySettings_Data(data);
}
void DrawPhysicsPatchControls()
{
    if (ImGui::Checkbox("Apply 60fps cloth physics patch", &g_Config.features->applyClothPhysicsPatch.get()))
    {
        g_PhysicsPatchHasBeenToggled = true;
    }
    if (!g_Config.features->applyClothPhysicsPatch) return;
    ImGui::Text(
        "When activated, the patches to physics softbodies are applied automatically.\n"
        "If you want to, you can try and tinker with the specific settings below."
    );
    ImGui::Separator();
    std::vector<TrackedSoftbody>& softbodiesOfInterest = GetSoftbodiesOfInterest();
    for (TrackedSoftbody& softbody : softbodiesOfInterest)
    {
        if (ImGui::CollapsingHeader(softbody.m_Desc.m_Name))
        {
            DrawControlsForSoftbody(softbody);
        }
    }
}
