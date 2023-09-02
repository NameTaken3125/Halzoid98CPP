#include "pch.h"

#include "FPV.h"


#include "ACU/ACUPlayerCameraComponent.h"
#include "ACU/Entity.h"
#include "ACU/SkeletonComponent.h"
#include "ACU/Skeleton.h"
#include "ACU/RenderValuesHolder.h"

bool g_CameraOverride_UseRendercameraMethod = true;
bool g_CameraOverride_UsePlayerCameraMethod = false;

void DoSomethingWithTheSkeletonComponent(Entity& player)
{
    // This section does nothing, I just figured that the "wobble bone" you mentioned
    // can perhaps be found in the player's `SkeletonComponent`, but I didn't check.
    constexpr uint64 vtbl_SkeletonComponent = 0x142E76630;
    SkeletonComponent* skeletonCpnt = static_cast<SkeletonComponent*>(player.FindComponentByVTBL(vtbl_SkeletonComponent));
    if (skeletonCpnt)
    {
        return;
    }
    uint64 baseSkeletonHandle_mb = 0x6A522DA4D; // Game Bootstrap Settings/ACU_BaseSkeleton_Male.Skeleton
    if (skeletonCpnt->shared_BaseSkeletonMale->handle != baseSkeletonHandle_mb)
    {
        return;
    }
    Skeleton* baseSkeleton = skeletonCpnt->shared_BaseSkeletonMale->GetPtr();
    if (!baseSkeleton)
    {
        return;
    }
    Bone* faceBone_mb = baseSkeleton->bones[61]; // Seems to be the highest one in this skeleton
    if (!faceBone_mb)
    {
        return;
    }
}
Vector3f CalculateNewCameraPosition(Entity& player)
{
    // The camera will be just above the player's head.
    return player.GetPosition() + Vector3f{ 0, 0, 2 };
}
namespace PlayerCameraMethod
{
void JustAfterFinalCameraPositionIsWrittenForTheFrame_OverrideCameraPosition(AllRegisters* params)
{
    if (!g_CameraOverride_UsePlayerCameraMethod)
    {
        return;
    }
    ACUPlayerCameraComponent* camera = (ACUPlayerCameraComponent*)params->r14_;
    Entity* player = camera->entity;
    DoSomethingWithTheSkeletonComponent(*player);
    Vector3f newCameraPosition = CalculateNewCameraPosition(*player);
    (Vector3f&)camera->positionLookFrom = newCameraPosition;
    camera->positionLookFrom.w = 0;
}
void JustAfterFinalCameraRotationIsWrittenForTheFrame_OverrideCameraRotation(AllRegisters* params)
{
    if (!g_CameraOverride_UsePlayerCameraMethod)
    {
        return;
    }
    ACUPlayerCameraComponent* camera = (ACUPlayerCameraComponent*)params->r14_;
    Entity* player = camera->entity;
    // The rotation is set to exactly the player's rotation.
    (Quaternion<float>&)camera->quaternion_mb = Quaternion<float>::fromMatrix(player->GetTransform().getRotation());
}
}
namespace RendercameraMethod
{
void JustAfterRendercameraPositionIsWritten_OverrideRendercameraPosition(AllRegisters* params)
{
    if (!g_CameraOverride_UseRendercameraMethod)
    {
        return;
    }
    ACUPlayerCameraComponent* camera = (ACUPlayerCameraComponent*)params->rdi_;
    Vector3f& renderCamPos = *(Vector3f*)(params->GetRSP() + 0x30);

    renderCamPos = CalculateNewCameraPosition(*camera->entity);
}
}
ExampleCameraOverride::ExampleCameraOverride()
{
    uintptr_t justAfterFinalCameraPositionIsWrittenForTheFrame = 0x141F3F9AF;
    PresetScript_CCodeInTheMiddle(justAfterFinalCameraPositionIsWrittenForTheFrame, 8,
        PlayerCameraMethod::JustAfterFinalCameraPositionIsWrittenForTheFrame_OverrideCameraPosition, RETURN_TO_RIGHT_AFTER_STOLEN_BYTES, true);
    uintptr_t justAfterFinalCameraRotationIsWrittenForTheFrame = 0x141F3FE18;
    PresetScript_CCodeInTheMiddle(justAfterFinalCameraRotationIsWrittenForTheFrame, 9,
        PlayerCameraMethod::JustAfterFinalCameraRotationIsWrittenForTheFrame_OverrideCameraRotation, RETURN_TO_RIGHT_AFTER_STOLEN_BYTES, true);
    uintptr_t justAfterRendercameraPositionIsWritten = 0x141F0B69E;
    PresetScript_CCodeInTheMiddle(justAfterRendercameraPositionIsWritten, 6,
        RendercameraMethod::JustAfterRendercameraPositionIsWritten_OverrideRendercameraPosition, RETURN_TO_RIGHT_AFTER_STOLEN_BYTES, true);
}
void DrawFPVPatchControls()
{
    ImGui::Checkbox("Rendercamera method", &g_CameraOverride_UseRendercameraMethod);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(
            "Seems to be closer to your result. My guess is that the lights are flickering"
            "\nbecause when this value is overridden, the geometry/light has already been culled."
        );
    }
    ImGui::Checkbox("Player Camera method", &g_CameraOverride_UsePlayerCameraMethod);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(
            "Not playable method, but one which could perhaps lead to the solution for flickering lights."
            "\nInstead of overriding just the camera position used in rendering,"
            "\nthis method overrides the position and rotation within the `ACUPlayerCameraComponent`"
            "\n(which are only later duplicated in the rendercamera)."
            "\nOverriding just the position forces the camera to look at the player's center"
            "\n(so basically at his feet), so I also override the rotation to match"
            "\nthe rotation of player's entity."
            "\nThis makes the camera look straight ahead, so still pretty much unplayable."
        );
    }
}
