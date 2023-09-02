#pragma once

#include "AutoAssemblerKinda/AutoAssemblerKinda.h"

extern bool g_EditableColorsets_IsEnabled;
struct EditableColorsets : AutoAssemblerCodeHolder_Base
{
    EditableColorsets();
    virtual void OnBeforeActivate() override { g_EditableColorsets_IsEnabled = true; }
    virtual void OnBeforeDeactivate() override { g_EditableColorsets_IsEnabled = false; }
};
void DrawColorsetsControls();
void EveryFrameUntilSuccess_FindAndUpdateColorsets();
