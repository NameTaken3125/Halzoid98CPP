#pragma once

#include "Common_Plugins/Enum_BindableKeyCode.h"

namespace MainConfig {

void FindAndLoadConfigFileOrCreateDefault(const std::filesystem::path& filename);
void WriteToFile();

} // namespace MainConfig

#include "OLYAFSer/OLYAFSer.h"
#include "Serialization/EnumAdapter.h"
#include "Serialization/NumericAdapters.h"

#define ACM(varName, VarType, AdapterType, optionalDefaultValue) ADD_CONFIG_MEMBER(varName, VarType, AdapterType, optionalDefaultValue)
#define YACSTOR(SubclsName) YACONFIGSECTION_SUBCLASS_CTOR(SubclsName)
struct ConfigTop : YAConfigSection {
    YACSTOR(ConfigTop);
    struct YACS_Features : YAConfigSection {
        YACSTOR(YACS_Features);
        ACM(editableColorsets, bool, BooleanAdapter, true);
        ACM(applyClothPhysicsPatch, bool, BooleanAdapter, true);
    };
    ACM(features, YACS_Features, YAConfigSectionAdapter, );
};
extern ConfigTop g_Config;

#undef ACM
#undef YACSTOR
