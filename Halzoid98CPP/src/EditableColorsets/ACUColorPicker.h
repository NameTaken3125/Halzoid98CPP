#pragma once

#include "vmath/vmath.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
namespace ImGui
{
// AC Unity's "Leather Color" has its hue value offset by 0.41f.
// For example, in Utopia Blue all of Primary, Secondary, Tertiary have hue==1.02f
// and while Leather is pretty much the same blue, its stored hue value is actually 1.43f
// (1.02f on Leather would correspond to, like, reddish brown).
// Why? Heh, Unity.
// Halzoid98's Outfit Manager actually adjusts this value in UI and adjusts it back when exporting.
// I should probably do the same.
inline bool HSVSliders(const char* label, Vector4f& v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const bool isHueNeedsToBeAdjusted = label == std::string_view("Leather color");
    if (isHueNeedsToBeAdjusted)
        v.x -= 0.41f;

    ImGuiDataType data_type = ImGuiDataType_Float;
    const bool showAlpha = false; // "Alpha channel" doesn't seem to do anything.
    int components = showAlpha ? 4 : 3;
    float v_min[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float v_max[4] = { 1.0f, 5.0f, 5.0f, 2.0f };
    const char* format = "%.3f";
    ImGuiSliderFlags flags = 0;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    using _4floats = float[4];
    auto& data = (_4floats&)v;
    {
        const char* id = "H:";
        const int idx = 0;
        ImGui::PushID(id);
        ImGui::AlignTextToFramePadding(); ImGui::Text(id); SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= SliderScalar("", data_type, &data[idx], &v_min[idx], &v_max[idx], format, flags);
        ImGui::PopID();
        PopItemWidth();
    }

    SameLine(0, g.Style.ItemInnerSpacing.x);
    {
        const char* id = "S:";
        const int idx = 1;
        ImGui::PushID(id);
        ImGui::AlignTextToFramePadding(); ImGui::Text(id); SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= SliderScalar("", data_type, &data[idx], &v_min[idx], &v_max[idx], format, flags);
        ImGui::PopID();
        PopItemWidth();
    }

    SameLine(0, g.Style.ItemInnerSpacing.x);
    {
        const char* id = "V:";
        const int idx = 2;
        ImGui::PushID(id);
        ImGui::AlignTextToFramePadding(); ImGui::Text(id); SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= SliderScalar("", data_type, &data[idx], &v_min[idx], &v_max[idx], format, flags);
        ImGui::PopID();
        PopItemWidth();
    }

    if (showAlpha)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= SliderScalar("A", data_type, &v.w, &v_min[3], &v_max[3], format, flags);
        PopItemWidth();
    }

    PopID(); // `label`

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    if (isHueNeedsToBeAdjusted)
        v.x += 0.41f;
    return value_changed;
}
}
inline bool ImGuiDrawACUColorPicker(const char* label, Vector4f& colorHSV)
{
    ImGuiCTX::PushID _id{ &colorHSV };
    return ImGui::HSVSliders(label, colorHSV);
}
