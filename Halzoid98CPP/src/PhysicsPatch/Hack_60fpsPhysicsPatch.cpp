#include "pch.h"

#include "Hack_60fpsPhysicsPatch.h"

#include "ACU/ManagedObject.h"
#include "vmath/vmath.h"

#include "ACU/SoftBodySettings.h"
#include "SoftbodyDescriptions.h"

#include "Experimental_StrongPtr.h"

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
void EveryFrameUntilSuccess_FindAndPatchSoftBodyData()
{
    static bool isEverythingPatched = false;
    if (isEverythingPatched)
    {
        return;
    }
    bool isEverythingPatchedThisTime = true;
    std::vector<TrackedSoftbody>& softbodiesOfInterest = GetSoftbodiesOfInterest();
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
        sbs->m_data = softbody.m_Desc.m_HalzoidPatchValues;
        softbody.m_IsAlreadyPatched = true;
    }
    if (isEverythingPatchedThisTime)
    {
        isEverythingPatched = true;
    }
}
#include "ImGuiCTX.h"
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
    std::vector<TrackedSoftbody>& softbodiesOfInterest = GetSoftbodiesOfInterest();
    for (TrackedSoftbody& softbody : softbodiesOfInterest)
    {
        if (ImGui::CollapsingHeader(softbody.m_Desc.m_Name))
        {
            DrawControlsForSoftbody(softbody);
        }
    }
}
