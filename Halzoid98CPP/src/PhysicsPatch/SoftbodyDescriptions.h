#pragma once

#include <vector>
#include "imgui/imgui.h"
#include "ACU/SoftBodySettings.h"

void Draw_SoftBodySettings_Data(SoftBodySettings_Data& sbsData)
{
    ImGui::Text(
        "Apparently, some settings can be edited in real time (like `EnablePhysics`, `UseWind`),"
        "\nbut changes to things like `ClothGravity` will only take effect after the outfit"
        "\nis \"rebuilt\". It should be enough to just go to Equipment Customization Menu,"
        "\nselect a different coat, then select the previous one."
    );
#define SBS_SLIDER(source, val, min_, max_) ImGui::SliderFloat(#val, &(source).val, min_, max_);
#define SBS_CHECKBOX(source, val) {\
    bool t = (source).val;\
    if (ImGui::Checkbox(#val, &t)) { (source).val = t; } \
    }

    SBS_SLIDER(sbsData, WindRandomFactor, 0, 1);
    SBS_CHECKBOX(sbsData, EnablePhysics);
    SBS_CHECKBOX(sbsData, UseWind);
    SBS_CHECKBOX(sbsData, UseGravityBone);
    //NormalsUpdateFrequency = 1;
    //GravityBoneID = 4294967295;
    SBS_SLIDER(sbsData, GravityBoneScale, 0, 10);
    SBS_SLIDER(sbsData, SkinningLinearVelocityFactor, 0, 30);
    SBS_SLIDER(sbsData, SkinningAngularVelocityFactor, 0, 30);
    SBS_SLIDER(sbsData, SkinningMaxFadeoutSpeed, 0, 1);
    SBS_SLIDER(sbsData, StructuralStiffness, 0, 10);
    SBS_SLIDER(sbsData, StructuralConstraintMinCorrectionFactor, -5, 0);
    SBS_SLIDER(sbsData, StructuralConstraintMaxCorrectionFactor, 0, 5);
    SBS_SLIDER(sbsData, ShearingStiffness, 0, 10);
    SBS_SLIDER(sbsData, ShearingConstraintMinCorrectionFactor, 0, 5);
    SBS_SLIDER(sbsData, ShearingConstraintMaxCorrectionFactor, 0, 10);
    SBS_SLIDER(sbsData, BendingStiffness, 0, 10);
    SBS_SLIDER(sbsData, BendingConstraintMinCorrectionFactor, -5, 0);
    SBS_SLIDER(sbsData, BendingConstraintMaxCorrectionFactor, 0, 5);
    SBS_CHECKBOX(sbsData, StretchConstraintEnabled);
    SBS_SLIDER(sbsData, StretchConstraintFactor, 0, 10);
    SBS_CHECKBOX(sbsData, MaxDistanceEnabled);
    SBS_SLIDER(sbsData, ScaleMaxDistance, 0, 10);
    SBS_CHECKBOX(sbsData, BackStopEnabled);
    SBS_SLIDER(sbsData, ScaleBackstop, 0, 1);
    SBS_CHECKBOX(sbsData, PerVertexGravityEnabled);
    SBS_CHECKBOX(sbsData, PerVertexDampingEnabled);
    SBS_CHECKBOX(sbsData, PerVertexSkinWidthEnabled);
    SBS_CHECKBOX(sbsData, PerVertexFrictionEnabled);
    SBS_CHECKBOX(sbsData, CollisionAgainstPrimitive);
    SBS_CHECKBOX(sbsData, CollisionAgainstCloth);
    //PriorityForClothClothCollision = 128;
    SBS_CHECKBOX(sbsData, ContinuousCollisionDetection);
    SBS_SLIDER(sbsData, Friction, 0, 1);
    SBS_SLIDER(sbsData, ClothSkinWidth, 0, 0.2f);
    SBS_CHECKBOX(sbsData, UseAdditionalVertices);
    SBS_SLIDER(sbsData, ResolutionForAdditionalVertices, 0, 1);
    SBS_SLIDER(sbsData, Damping, 0, 0.15f);
    //NumIterations = 4;
    ImGui::SliderFloat3("ClothGravity", sbsData.ClothGravity, -20, 20);
    SBS_SLIDER(sbsData, EnvironmentDensity, 0, 100);
    SBS_SLIDER(sbsData, ClothMaterialDensity, 0, 50);
    SBS_CHECKBOX(sbsData, Structural);
    SBS_CHECKBOX(sbsData, Shearing);
    SBS_CHECKBOX(sbsData, Bending);

#undef SBS_SLIDER
#undef SBS_CHECKBOX
}
const SoftBodySettings_Data GetSBD_Original_ArnoFlap()
{
    SoftBodySettings_Data result;
    result.WindRandomFactor = 0.3f;
    result.EnablePhysics = true;
    result.UseWind = false;
    result.UseGravityBone = false;
    result.NormalsUpdateFrequency = 1;
    result.GravityBoneID = 4294967295;
    result.GravityBoneScale = 1;
    result.SkinningLinearVelocityFactor = 10;
    result.SkinningAngularVelocityFactor = 10;
    result.SkinningMaxFadeoutSpeed = 0.1f;
    result.StructuralStiffness = 1;
    result.StructuralConstraintMinCorrectionFactor = -2;
    result.StructuralConstraintMaxCorrectionFactor = 2;
    result.ShearingStiffness = 1;
    result.ShearingConstraintMinCorrectionFactor = 0;
    result.ShearingConstraintMaxCorrectionFactor = 2;
    result.BendingStiffness = 1.5f;
    result.BendingConstraintMinCorrectionFactor = -2;
    result.BendingConstraintMaxCorrectionFactor = 0;
    result.StretchConstraintEnabled = true;
    result.StretchConstraintFactor = 1.1f;
    result.MaxDistanceEnabled = true;
    result.ScaleMaxDistance = 1;
    result.BackStopEnabled = true;
    result.ScaleBackstop = 0.2f;
    result.PerVertexGravityEnabled = true;
    result.PerVertexDampingEnabled = false;
    result.PerVertexSkinWidthEnabled = true;
    result.PerVertexFrictionEnabled = false;
    result.CollisionAgainstPrimitive = true;
    result.CollisionAgainstCloth = false;
    result.PriorityForClothClothCollision = 128;
    result.ContinuousCollisionDetection = false;
    result.Friction = 0.2f;
    result.ClothSkinWidth = 0.05f;
    result.UseAdditionalVertices = false;
    result.ResolutionForAdditionalVertices = 0.2f;
    result.Damping = 0;
    result.NumIterations = 4;
    result.ClothGravity = { 0, 0, -20 };
    result.EnvironmentDensity = 30;
    result.ClothMaterialDensity = 15;
    result.Structural = true;
    result.Shearing = true;
    result.Bending = true;
    return result;
}
const SoftBodySettings_Data GetSBD_Original_NoBackstop()
{
    SoftBodySettings_Data result;
    result.WindRandomFactor = 0.3f;
    result.EnablePhysics = true;
    result.UseWind = false;
    result.UseGravityBone = false;
    result.NormalsUpdateFrequency = 1;
    result.GravityBoneID = 4294967295;
    result.GravityBoneScale = 1;
    result.SkinningLinearVelocityFactor = 10;
    result.SkinningAngularVelocityFactor = 10;
    result.SkinningMaxFadeoutSpeed = 0.1f;
    result.StructuralStiffness = 1;
    result.StructuralConstraintMinCorrectionFactor = -2;
    result.StructuralConstraintMaxCorrectionFactor = 2;
    result.ShearingStiffness = 1;
    result.ShearingConstraintMinCorrectionFactor = 0;
    result.ShearingConstraintMaxCorrectionFactor = 2;
    result.BendingStiffness = 1;
    result.BendingConstraintMinCorrectionFactor = -2;
    result.BendingConstraintMaxCorrectionFactor = 0;
    result.StretchConstraintEnabled = true;
    result.StretchConstraintFactor = 1.1f;
    result.MaxDistanceEnabled = true;
    result.ScaleMaxDistance = 1;
    result.BackStopEnabled = false;
    result.ScaleBackstop = 0.2f;
    result.PerVertexGravityEnabled = false;
    result.PerVertexDampingEnabled = false;
    result.PerVertexSkinWidthEnabled = true;
    result.PerVertexFrictionEnabled = false;
    result.CollisionAgainstPrimitive = true;
    result.CollisionAgainstCloth = false;
    result.PriorityForClothClothCollision = 128;
    result.ContinuousCollisionDetection = false;
    result.Friction = 0.2f;
    result.ClothSkinWidth = 0.05f;
    result.UseAdditionalVertices = false;
    result.ResolutionForAdditionalVertices = 0.2f;
    result.Damping = 0;
    result.NumIterations = 2;
    result.ClothGravity = { 0, 0, -20 };
    result.EnvironmentDensity = 1;
    result.ClothMaterialDensity = 10;
    result.Structural = true;
    result.Shearing = true;
    result.Bending = true;
    return result;
}
const SoftBodySettings_Data GetSBD_Original_SDNCloak()
{
    SoftBodySettings_Data result;
    result.WindRandomFactor = 0.3f;
    result.EnablePhysics = true;
    result.UseWind = false;
    result.UseGravityBone = false;
    result.NormalsUpdateFrequency = 1;
    result.GravityBoneID = 4294967295;
    result.GravityBoneScale = 4;
    result.SkinningLinearVelocityFactor = 10;
    result.SkinningAngularVelocityFactor = 10;
    result.SkinningMaxFadeoutSpeed = 0.1f;
    result.StructuralStiffness = 1;
    result.StructuralConstraintMinCorrectionFactor = -2;
    result.StructuralConstraintMaxCorrectionFactor = 2;
    result.ShearingStiffness = 1;
    result.ShearingConstraintMinCorrectionFactor = 0;
    result.ShearingConstraintMaxCorrectionFactor = 2;
    result.BendingStiffness = 1.5f;
    result.BendingConstraintMinCorrectionFactor = -2;
    result.BendingConstraintMaxCorrectionFactor = 0;
    result.StretchConstraintEnabled = true;
    result.StretchConstraintFactor = 1.1f;
    result.MaxDistanceEnabled = true;
    result.ScaleMaxDistance = 1;
    result.BackStopEnabled = true;
    result.ScaleBackstop = 0.75f;
    result.PerVertexGravityEnabled = true;
    result.PerVertexDampingEnabled = false;
    result.PerVertexSkinWidthEnabled = true;
    result.PerVertexFrictionEnabled = false;
    result.CollisionAgainstPrimitive = true;
    result.CollisionAgainstCloth = false;
    result.PriorityForClothClothCollision = 128;
    result.ContinuousCollisionDetection = false;
    result.Friction = 0.05f;
    result.ClothSkinWidth = 0.03f;
    result.UseAdditionalVertices = false;
    result.ResolutionForAdditionalVertices = 0.2f;
    result.Damping = 0;
    result.NumIterations = 4;
    result.ClothGravity = { 0, 0, -20 };
    result.EnvironmentDensity = 30;
    result.ClothMaterialDensity = 50;
    result.Structural = true;
    result.Shearing = true;
    result.Bending = true;
    return result;
}
const SoftBodySettings_Data GetSBD_Original_ArnoV1Scarf()
{
    SoftBodySettings_Data result;
    result.WindRandomFactor = 0.3f;
    result.EnablePhysics = true;
    result.UseWind = false;
    result.UseGravityBone = false;
    result.NormalsUpdateFrequency = 1;
    result.GravityBoneID = 4294967295;
    result.GravityBoneScale = 1;
    result.SkinningLinearVelocityFactor = 10;
    result.SkinningAngularVelocityFactor = 10;
    result.SkinningMaxFadeoutSpeed = 0.1f;
    result.StructuralStiffness = 1;
    result.StructuralConstraintMinCorrectionFactor = -2;
    result.StructuralConstraintMaxCorrectionFactor = 2;
    result.ShearingStiffness = 1;
    result.ShearingConstraintMinCorrectionFactor = 0;
    result.ShearingConstraintMaxCorrectionFactor = 2;
    result.BendingStiffness = 1;
    result.BendingConstraintMinCorrectionFactor = -2;
    result.BendingConstraintMaxCorrectionFactor = 0;
    result.StretchConstraintEnabled = true;
    result.StretchConstraintFactor = 1.1f;
    result.MaxDistanceEnabled = true;
    result.ScaleMaxDistance = 1;
    result.BackStopEnabled = false;
    result.ScaleBackstop = 0.2f;
    result.PerVertexGravityEnabled = false;
    result.PerVertexDampingEnabled = false;
    result.PerVertexSkinWidthEnabled = true;
    result.PerVertexFrictionEnabled = false;
    result.CollisionAgainstPrimitive = true;
    result.CollisionAgainstCloth = false;
    result.PriorityForClothClothCollision = 128;
    result.ContinuousCollisionDetection = false;
    result.Friction = 0.2f;
    result.ClothSkinWidth = 0.05f;
    result.UseAdditionalVertices = false;
    result.ResolutionForAdditionalVertices = 0.2f;
    result.Damping = 0;
    result.NumIterations = 2;
    result.ClothGravity = { 0, 0, -9.8f };
    result.EnvironmentDensity = 1;
    result.ClothMaterialDensity = 10;
    result.Structural = true;
    result.Shearing = true;
    result.Bending = true;
    return result;
}
const SoftBodySettings_Data GetSBD_HalzoidPatch_ArnoFlap()
{
    SoftBodySettings_Data result;
    result.WindRandomFactor = 0.3f;
    result.EnablePhysics = true;
    result.UseWind = true;
    result.UseGravityBone = false;
    result.NormalsUpdateFrequency = 1;
    result.GravityBoneID = 4294967295;
    result.GravityBoneScale = 1;
    result.SkinningLinearVelocityFactor = 10;
    result.SkinningAngularVelocityFactor = 10;
    result.SkinningMaxFadeoutSpeed = 0.1f;
    result.StructuralStiffness = 1;
    result.StructuralConstraintMinCorrectionFactor = -2;
    result.StructuralConstraintMaxCorrectionFactor = 2;
    result.ShearingStiffness = 1;
    result.ShearingConstraintMinCorrectionFactor = 0;
    result.ShearingConstraintMaxCorrectionFactor = 2;
    result.BendingStiffness = 1.5f;
    result.BendingConstraintMinCorrectionFactor = -2;
    result.BendingConstraintMaxCorrectionFactor = 0;
    result.StretchConstraintEnabled = true;
    result.StretchConstraintFactor = 1.1f;
    result.MaxDistanceEnabled = true;
    result.ScaleMaxDistance = 1;
    result.BackStopEnabled = true;
    result.ScaleBackstop = 0.2f;
    result.PerVertexGravityEnabled = true;
    result.PerVertexDampingEnabled = false;
    result.PerVertexSkinWidthEnabled = true;
    result.PerVertexFrictionEnabled = false;
    result.CollisionAgainstPrimitive = true;
    result.CollisionAgainstCloth = false;
    result.PriorityForClothClothCollision = 128;
    result.ContinuousCollisionDetection = false;
    result.Friction = 0.2f;
    result.ClothSkinWidth = 0.05f;
    result.UseAdditionalVertices = false;
    result.ResolutionForAdditionalVertices = 0.2f;
    result.Damping = 0.02f;
    result.NumIterations = 4;
    result.ClothGravity = { 0, 0, -6 };
    result.EnvironmentDensity = 30;
    result.ClothMaterialDensity = 15;
    result.Structural = true;
    result.Shearing = true;
    result.Bending = true;
    return result;
}
const SoftBodySettings_Data GetSBD_HalzoidPatch_NoBackstop()
{
    SoftBodySettings_Data result;
    result.WindRandomFactor = 0.3f;
    result.EnablePhysics = true;
    result.UseWind = true;
    result.UseGravityBone = false;
    result.NormalsUpdateFrequency = 1;
    result.GravityBoneID = 4294967295;
    result.GravityBoneScale = 1;
    result.SkinningLinearVelocityFactor = 10;
    result.SkinningAngularVelocityFactor = 10;
    result.SkinningMaxFadeoutSpeed = 0.1f;
    result.StructuralStiffness = 1;
    result.StructuralConstraintMinCorrectionFactor = -2;
    result.StructuralConstraintMaxCorrectionFactor = 2;
    result.ShearingStiffness = 1;
    result.ShearingConstraintMinCorrectionFactor = 0;
    result.ShearingConstraintMaxCorrectionFactor = 2;
    result.BendingStiffness = 1;
    result.BendingConstraintMinCorrectionFactor = -2;
    result.BendingConstraintMaxCorrectionFactor = 0;
    result.StretchConstraintEnabled = true;
    result.StretchConstraintFactor = 1.1f;
    result.MaxDistanceEnabled = true;
    result.ScaleMaxDistance = 1;
    result.BackStopEnabled = false;
    result.ScaleBackstop = 0.2f;
    result.PerVertexGravityEnabled = false;
    result.PerVertexDampingEnabled = false;
    result.PerVertexSkinWidthEnabled = true;
    result.PerVertexFrictionEnabled = false;
    result.CollisionAgainstPrimitive = true;
    result.CollisionAgainstCloth = false;
    result.PriorityForClothClothCollision = 128;
    result.ContinuousCollisionDetection = false;
    result.Friction = 0.2f;
    result.ClothSkinWidth = 0.05f;
    result.UseAdditionalVertices = false;
    result.ResolutionForAdditionalVertices = 0.2f;
    result.Damping = 0.02f;
    result.NumIterations = 2;
    result.ClothGravity = { 0, 0, -6 };
    result.EnvironmentDensity = 1;
    result.ClothMaterialDensity = 10;
    result.Structural = true;
    result.Shearing = true;
    result.Bending = true;
    return result;
}
const SoftBodySettings_Data GetSBD_HalzoidPatch_SDNCloak()
{
    SoftBodySettings_Data result;
    result.WindRandomFactor = 0.3f;
    result.EnablePhysics = true;
    result.UseWind = true;
    result.UseGravityBone = false;
    result.NormalsUpdateFrequency = 1;
    result.GravityBoneID = 4294967295;
    result.GravityBoneScale = 1;
    result.SkinningLinearVelocityFactor = 10;
    result.SkinningAngularVelocityFactor = 10;
    result.SkinningMaxFadeoutSpeed = 0.1f;
    result.StructuralStiffness = 1;
    result.StructuralConstraintMinCorrectionFactor = -2;
    result.StructuralConstraintMaxCorrectionFactor = 2;
    result.ShearingStiffness = 1;
    result.ShearingConstraintMinCorrectionFactor = 0;
    result.ShearingConstraintMaxCorrectionFactor = 2;
    result.BendingStiffness = 1.5f;
    result.BendingConstraintMinCorrectionFactor = -2;
    result.BendingConstraintMaxCorrectionFactor = 0;
    result.StretchConstraintEnabled = true;
    result.StretchConstraintFactor = 1.1f;
    result.MaxDistanceEnabled = true;
    result.ScaleMaxDistance = 1;
    result.BackStopEnabled = true;
    result.ScaleBackstop = 0.2f;
    result.PerVertexGravityEnabled = true;
    result.PerVertexDampingEnabled = false;
    result.PerVertexSkinWidthEnabled = true;
    result.PerVertexFrictionEnabled = false;
    result.CollisionAgainstPrimitive = true;
    result.CollisionAgainstCloth = false;
    result.PriorityForClothClothCollision = 128;
    result.ContinuousCollisionDetection = false;
    result.Friction = 0.2f;
    result.ClothSkinWidth = 0.05f;
    result.UseAdditionalVertices = false;
    result.ResolutionForAdditionalVertices = 0.2f;
    result.Damping = 0.02f;
    result.NumIterations = 4;
    result.ClothGravity = { 0, 0, -6 };
    result.EnvironmentDensity = 30;
    result.ClothMaterialDensity = 15;
    result.Structural = true;
    result.Shearing = true;
    result.Bending = true;
    return result;
}


uint64 handle_SoftBodySettings_ArnoV1CoatFlap = 0x16c61d53a1; // Softbody_CN_P_Flap_ArnaudV1/ClothSettings_Standard_Avatars.SoftBodySettings
uint64 handle_SoftBodySettings_ArnoV1Scarf = 0xFB9CE333D; // Softbody_CN_P_Scarf_ArnaudV1.data/1_-_ClothSettings_Standard_NoBackstop.SoftBodySettings
uint64 handle_SoftBodySettings_NapoleonianFlapV4 = 0x18a5a1d395; // Softbody_CN_P_Flap_Napoleonian_V4/ClothSettings_StandardAvatar_NoBackstop.SoftBodySettings
uint64 handle_SoftBodySettings_FranciadeCloak = 0x3da29afaf0; // NEW_SDN_Softbody_CN_U_Legacy_StDenis_Cloak/NEW_SDN_ClothSettings_Standard_Avatars_01.SoftBodySettings
struct SoftbodyDescription
{
    uint64 m_Handle;
    const char* m_Name;
    const SoftBodySettings_Data m_DefaultValues;
    const SoftBodySettings_Data m_HalzoidPatchValues;
};
std::vector<SoftbodyDescription> g_SoftbodyDescriptions = {
    {handle_SoftBodySettings_ArnoV1CoatFlap,    "Softbody_CN_P_Flap_ArnaudV1/ClothSettings_Standard_Avatars.SoftBodySettings",                              GetSBD_Original_ArnoFlap(),     GetSBD_HalzoidPatch_ArnoFlap()},
    {handle_SoftBodySettings_ArnoV1Scarf,       "Softbody_CN_P_Scarf_ArnaudV1.data/1_-_ClothSettings_Standard_NoBackstop.SoftBodySettings",                 GetSBD_Original_ArnoV1Scarf(),  GetSBD_HalzoidPatch_NoBackstop()},
    {handle_SoftBodySettings_NapoleonianFlapV4, "Softbody_CN_P_Flap_Napoleonian_V4/ClothSettings_StandardAvatar_NoBackstop.SoftBodySettings",               GetSBD_Original_NoBackstop(),   GetSBD_HalzoidPatch_NoBackstop()},
    {handle_SoftBodySettings_FranciadeCloak,    "NEW_SDN_Softbody_CN_U_Legacy_StDenis_Cloak/NEW_SDN_ClothSettings_Standard_Avatars_01.SoftBodySettings",    GetSBD_Original_SDNCloak(),     GetSBD_HalzoidPatch_SDNCloak()},
};
