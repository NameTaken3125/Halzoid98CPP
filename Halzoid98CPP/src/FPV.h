#pragma once

#include "AutoAssemblerKinda/AutoAssemblerKinda.h"

struct ExampleCameraOverride : AutoAssemblerCodeHolder_Base
{
    ExampleCameraOverride();
};

extern bool g_CameraOverride_UseRendercameraMethod;
extern bool g_CameraOverride_UsePlayerCameraMethod;
void DrawFPVPatchControls();
