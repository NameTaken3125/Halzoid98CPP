#pragma once

#include "vmath/vmath.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
namespace ImGui
{
inline bool HSVSliders(const char* label, Vector4f& v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiDataType data_type = ImGuiDataType_Float;
    int components = 4;
    float v_min[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float v_max[4] = { 1.0f, 5.0f, 5.0f, 2.0f };
    const char* format = "%.3f";
    ImGuiSliderFlags flags = 0;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());

    value_changed |= SliderScalar("H", data_type, &v.x, &v_min[0], &v_max[0], format, flags);
    PopItemWidth();

    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= SliderScalar("S", data_type, &v.y, &v_min[1], &v_max[1], format, flags);
    PopItemWidth();

    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= SliderScalar("V", data_type, &v.z, &v_min[2], &v_max[2], format, flags);
    PopItemWidth();

    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= SliderScalar("A", data_type, &v.w, &v_min[3], &v_max[3], format, flags);
    PopItemWidth();

    PopID(); // `label`

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}
}
inline bool ImGuiDrawACUColorPicker(const char* label, Vector4f& colorHSV)
{
    ImGuiCTX::PushID _id{ &colorHSV };
    return ImGui::HSVSliders(label, colorHSV);
}
