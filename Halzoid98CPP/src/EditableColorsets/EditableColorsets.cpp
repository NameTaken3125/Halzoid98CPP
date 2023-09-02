#include "pch.h"

#include "EditableColorsets.h"

#include "Common_Plugins/Common_PluginSide.h"
#include "ImGuiCTX.h"

#include "vmath/vmath.h"
#include "ACU/PlayerCustomizationWorker.h"
#include "ACU/BuildTable.h"
#include "ACU/AvatarGearManager.h"
#include "ACU/GearPage.h"
#include "ACU/MenuManager.h"

#include "ACU_SharedPtrs.h"
#include "Experimental_StrongPtr.h"

#include "ColorsetDescriptions.h"
#include "ACUColorPicker.h"

bool g_EditableColorsets_IsEnabled = false;
bool g_EditableColorsets_IsUpdated = false;
constexpr uint64 handle_BuildTable_colorsets = 130934825362;        // CN_P_Arno\CN_P_Avatars_ColorSets.BuildTable
std::optional<ACUSharedPtr_Strong> g_EditableColorsets_ColorsetsBuildtable;
bool g_EditableColorsets_isAlreadyUpdatedFromSavedSettings = false;
fs::path GetColorPresetsFilepath()
{
    const char* g_EditableColorsets_SavedPresetsFilename = "Halzoid98/EditableColorsets-SavedColors.json";
    return AbsolutePathInThisDLLDirectory(g_EditableColorsets_SavedPresetsFilename);
}








GearPage* FindGearPageIfOpened()
{
    MenuManager* menuMan = MenuManager::GetSingleton();
    if (!menuMan) { return nullptr; }
    MenuManager_30* menuPagesStack = menuMan->menuPagesStack_mb;
    if (!menuPagesStack) { return nullptr; }
    for (auto& hasMenuPage : menuPagesStack->arrHasMenuPages)
    {
        if (MenuPage* menuPage = hasMenuPage->shared_MenuPage->GetPtr())
        {
            uint64 menuPageVtbl = *(uint64*)menuPage;
            constexpr uint64 vtbl_GearPage = 0x142EDF810;
            if (menuPageVtbl == vtbl_GearPage)
            {
                return static_cast<GearPage*>(menuPage);
            }
        }
    }
    return nullptr;
}
const ColorsetDescription* FindCurrentColorset()
{
    GearPage* gearPage = FindGearPageIfOpened();
    if (!gearPage) { return nullptr; }
    uint64 handle_currentColorset = gearPage->customizationWorker.currentCustomizationItems.shared_invItemSett_Colorset->handle;
    for (const ColorsetDescription& colorsetDesc : g_ColorsetsDescriptions)
    {
        if (colorsetDesc.m_InventoryItemHandle == handle_currentColorset)
        {
            return &colorsetDesc;
        }
    }
    return nullptr;
}
BuildRow* GetColorsetRow(BuildTable& colorsetsBuildTable, const ColorsetDescription& whichColorset)
{
    if (whichColorset.m_IndexInColorsetsBuildtable >= colorsetsBuildTable.buildRows.size)
    {
        return nullptr;
    }
    return &colorsetsBuildTable.buildRows[whichColorset.m_IndexInColorsetsBuildtable];
}
std::optional<int> FindActualIndexInDataArrayForColorsetValue(BuildRow& colorsetRow, const ColorsetValueDescription& valueDescription)
{
    std::optional<int> valueIndexInActualArrayOfDynamicProperties;
    for (int i = 0; i < colorsetRow.Components->cpnts.size; i++)
    {
        auto& dynamicPropertyDescription = colorsetRow.Components->cpnts[i];
        if (dynamicPropertyDescription.Index_orNameHash == valueDescription.IndexCorrespondingToTheBuildColumn)
        {
            valueIndexInActualArrayOfDynamicProperties = i;
            break;
        }
    }
    return valueIndexInActualArrayOfDynamicProperties;
}
void* GetDataForColorsetValue(BuildRow& colorsetRow, const ColorsetValueDescription& valueDesc)
{
    std::optional<int> dataIdx = FindActualIndexInDataArrayForColorsetValue(colorsetRow, valueDesc);
    if (!dataIdx) { return nullptr; }
    return (void*)((uint64)colorsetRow.vectorValuesForColorSets + *dataIdx * 0x10);
}
MyColorPresetData ColorBuildRowToCStruct(BuildRow& colorRow)
{
    MyColorPresetData resultPreset;
    for (const ColorsetValueDescription& valueDesc : g_ColorsetValueDescriptions)
    {
        void* dataMemberInPreset = valueDesc.MapToCStructMember(resultPreset);
        void* dataInBuildrow = GetDataForColorsetValue(colorRow, valueDesc);
        if (!dataInBuildrow) { continue; }
        if (valueDesc.ValueType == ColorsetValueType::SingleFloat) {
            *(float*)dataMemberInPreset = *(float*)dataInBuildrow;
        }
        else if (valueDesc.ValueType == ColorsetValueType::Vector4) {
            *(Vector4f*)dataMemberInPreset = *(Vector4f*)dataInBuildrow;
        }
    }
    return resultPreset;
}
void ApplyColorPreset(BuildRow& targetColorBuildRow, const MyColorPresetData& newColors)
{
    for (const ColorsetValueDescription& valueDesc : g_ColorsetValueDescriptions)
    {
        void* dataMemberInPreset = valueDesc.MapToCStructMember(const_cast<MyColorPresetData&>(newColors));
        void* dataInBuildrow = GetDataForColorsetValue(targetColorBuildRow, valueDesc);
        if (!dataInBuildrow) { continue; }
        if (valueDesc.ValueType == ColorsetValueType::SingleFloat) {
            *(float*)dataInBuildrow = *(float*)dataMemberInPreset;
        }
        else if (valueDesc.ValueType == ColorsetValueType::Vector4) {
            *(Vector4f*)dataInBuildrow = *(Vector4f*)dataMemberInPreset;
        }
    }
    g_EditableColorsets_IsUpdated = true;
}
JSON AllColorsToJSON(BuildTable& colorsetsTable)
{
    JSON result = json::Object();
    for (const ColorsetDescription& colorsetDesc : g_ColorsetsDescriptions)
    {
        BuildRow* colorRow = GetColorsetRow(colorsetsTable, colorsetDesc);
        if (!colorRow) { continue; }
        result[colorsetDesc.m_Name] = ColorBuildRowToCStruct(*colorRow).ToJSON();
    }
    return result;
}
void AllColorsFromJSON(BuildTable& colorsetsTable, const std::string& dumpedJSON)
{
    JSON j = JSON::Load(dumpedJSON);
    if (j.JSONType() != JSON::Class::Object) { return; }
    for (const ColorsetDescription& colorsetDesc : g_ColorsetsDescriptions)
    {
        j.hasKey(colorsetDesc.m_Name);
        JSON* presetAsJSON = j.FindByKey(colorsetDesc.m_Name);
        if (!presetAsJSON) {
            continue;
        }
        MyColorPresetData preset;
        preset.FromJSON(*presetAsJSON);
        BuildRow* colorRow = GetColorsetRow(colorsetsTable, colorsetDesc);
        if (!colorRow) { continue; }
        ApplyColorPreset(*colorRow, preset);
    }
}
void ColorsetsToText_WasUsedToGenerateTheDefaultPresets(BuildTable& colorsetsBuildTable)
{
    std::vector<MyColorPresetData> resultPresets;
    resultPresets.reserve(g_ColorsetsDescriptions.size());
    for (const ColorsetDescription& colorsetDesc : g_ColorsetsDescriptions)
    {
        if (colorsetDesc.m_IndexInColorsetsBuildtable >= colorsetsBuildTable.buildRows.size)
        {
            continue;
        }
        BuildRow& colorRow = colorsetsBuildTable.buildRows[colorsetDesc.m_IndexInColorsetsBuildtable];
        auto* dynamicPropsDesc = colorRow.Components;
        auto* dynamicPropsData = colorRow.vectorValuesForColorSets;
        if (!dynamicPropsDesc || !dynamicPropsData) {
            continue;
        }
        resultPresets.push_back(ColorBuildRowToCStruct(colorRow));
    }
    auto ImGuiTextVec4f = [](const Vector4f& vec)
    {
        ImGui::LogText("\n    Vector4f(%.3ff, %.3ff, %.3ff, %.3ff),", vec.x, vec.y, vec.z, vec.w);
    };
    ImGui::LogToClipboard();
    for (const MyColorPresetData& preset : resultPresets)
    {
        ImGui::LogText(
            "\nMyColorPresetData("
        );
        ImGuiTextVec4f(preset.PrimaryColor);
        ImGuiTextVec4f(preset.SecondaryColor);
        ImGuiTextVec4f(preset.TertiaryColor);
        ImGuiTextVec4f(preset.Color4);
        ImGuiTextVec4f(preset.Color5);
        ImGuiTextVec4f(preset.Color6);
        ImGuiTextVec4f(preset.LeatherColor);
        ImGui::LogText("\n    %.3ff", preset.Velvet);
        ImGui::LogText("\n);");
    }
    ImGui::LogFinish();
}

void LoadPresetsFromFile(BuildTable& colorsetsBuildTable)
{
    std::ifstream savedPresetsFile(GetColorPresetsFilepath());
    if (!savedPresetsFile)
    {
        return;
    }
    std::stringstream presetsAsText;
    presetsAsText << savedPresetsFile.rdbuf();
    AllColorsFromJSON(colorsetsBuildTable, presetsAsText.str());
}
void SavePresetsToFile(BuildTable& colorsetsBuildTable)
{
    std::ofstream outfile(GetColorPresetsFilepath());
    JSON presetsAsJSON = AllColorsToJSON(colorsetsBuildTable);
    outfile << presetsAsJSON;
}
void ResetAllColorsetsToDefault(BuildTable& colorsetsBuildTable)
{
    for (const ColorsetDescription& colorsetDesc : g_ColorsetsDescriptions)
    {
        BuildRow* colorRow = GetColorsetRow(colorsetsBuildTable, colorsetDesc);
        if (!colorRow) { continue; }
        ApplyColorPreset(*colorRow, colorsetDesc.m_DefaultColors);
    }
}
bool DrawColorsetValue(BuildRow& row, ColorsetValueDescription& valueDescription)
{
    bool isChanged = false;
    if (!row.Components) { return isChanged; }
    std::optional<int> valueIndexInActualArrayOfDynamicProperties;
    for (int i = 0; i < row.Components->cpnts.size; i++)
    {
        auto& dynamicPropertyDescription = row.Components->cpnts[i];
        if (dynamicPropertyDescription.Index_orNameHash == valueDescription.IndexCorrespondingToTheBuildColumn)
        {
            valueIndexInActualArrayOfDynamicProperties = i;
            break;
        }
    }
    if (!valueIndexInActualArrayOfDynamicProperties) { return isChanged; }
    void* dataForTheValue = (void*)((uint64)row.vectorValuesForColorSets + *valueIndexInActualArrayOfDynamicProperties * 0x10);
    if (valueDescription.ValueType == ColorsetValueType::SingleFloat)
    {
        isChanged = ImGui::SliderFloat(valueDescription.Role, (float*)dataForTheValue, 0, 1);
    }
    else if (valueDescription.ValueType == ColorsetValueType::Vector4)
    {
        isChanged = ImGuiDrawACUColorPicker(valueDescription.Role, *(Vector4f*)dataForTheValue);
    }
    return isChanged;
}
bool DrawColorsetSliders(BuildRow& row)
{
    bool isChanged = false;
    for (ColorsetValueDescription& valueDescription : g_ColorsetValueDescriptions)
    {
        isChanged |= DrawColorsetValue(row, valueDescription);
    }
    return isChanged;
}
void DrawColorsetsControls(BuildTable& colorsetsBuildTable)
{
    if (ImGui::Button("Save all to file"))
    {
        SavePresetsToFile(colorsetsBuildTable);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load all from file"))
    {
        LoadPresetsFromFile(colorsetsBuildTable);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset all to default"))
    {
        ResetAllColorsetsToDefault(colorsetsBuildTable);
    }
    const ColorsetDescription* currentColorset = FindCurrentColorset();
    if (!currentColorset)
    {
        ImGui::Text("Cannot detect current colorset. Open the Equipment Customization Menu.");
        return;
    }
    ImGui::Text("Current color: %s", currentColorset->m_Name);
    BuildRow* currentColorRow = GetColorsetRow(colorsetsBuildTable, *currentColorset);
    if (!currentColorRow)
    {
        ImGui::Text("Cannot edit current color");
        return;
    }
    if (ImGui::Button("Reset this colorset to default"))
    {
        ApplyColorPreset(*currentColorRow, currentColorset->m_DefaultColors);
    }
    bool isChanged = DrawColorsetSliders(*currentColorRow);
    if (isChanged)
    {
        g_EditableColorsets_IsUpdated = true;
    }
}
void DrawColorsetsControls()
{
    if (!g_EditableColorsets_IsEnabled) { return; }
    BuildTable* colorsetsBuildTable =
        g_EditableColorsets_ColorsetsBuildtable
        ? g_EditableColorsets_ColorsetsBuildtable->GetPtr<BuildTable>()
        : nullptr;
    if (colorsetsBuildTable)
    {
        DrawColorsetsControls(*colorsetsBuildTable);
    }
    else
    {
        ImGui::Text(
            "The Colorsets BuildTable isn't loaded yet."
            "\nTry taking off the Outfit (like Altair's) if you have one equipped"
            "\nthen enter the Colors section of the customization menu."
        );
    }
}
void EveryFrameUntilSuccess_FindAndUpdateColorsets()
{
    if (g_EditableColorsets_isAlreadyUpdatedFromSavedSettings)
    {
        return;
    }
    if (!g_EditableColorsets_ColorsetsBuildtable)
    {
        g_EditableColorsets_ColorsetsBuildtable.emplace(handle_BuildTable_colorsets);
    }
    if (BuildTable* colorsetsTable = g_EditableColorsets_ColorsetsBuildtable->GetPtr<BuildTable>())
    {
        LoadPresetsFromFile(*colorsetsTable);
        g_EditableColorsets_isAlreadyUpdatedFromSavedSettings = true;
    }
}



void WhenCheckingIfCurrentlyHighlightedAvatarGearChanged_AllowToUpdateIfColorsWereEdited(AllRegisters* params)
{
    PlayerCustomizationWorker* customizationWorker = (PlayerCustomizationWorker*)params->rbx_;
    if (g_EditableColorsets_IsUpdated)
    {
        g_EditableColorsets_IsUpdated = false;
        AvatarGear* fakeGearSoThatTheGameThinksPlayersVisualsChangedAndNeedToBeRebuilt = (AvatarGear*)1;
        customizationWorker->previouslyHighlightedGear = fakeGearSoThatTheGameThinksPlayersVisualsChangedAndNeedToBeRebuilt;
    }
}
EditableColorsets::EditableColorsets()
{
    uintptr_t whenCheckingIfCurrentlyHighlightedAvatarGearChanged = 0x14111F269;
    PresetScript_CCodeInTheMiddle(whenCheckingIfCurrentlyHighlightedAvatarGearChanged, 7,
        WhenCheckingIfCurrentlyHighlightedAvatarGearChanged_AllowToUpdateIfColorsWereEdited, RETURN_TO_RIGHT_AFTER_STOLEN_BYTES, true);
}
