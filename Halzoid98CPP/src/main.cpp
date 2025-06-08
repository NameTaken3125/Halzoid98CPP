/*
main.cpp
Brings the app's components together.
*/

#include "pch.h"

#include "MyLog.h"
#include "MainConfig.h"

#include "Common_Plugins/Common_PluginSide.h"

#define LOG_FILENAME    THIS_DLL_PROJECT_NAME "-log.log"
#define CONFIG_FILENAME THIS_DLL_PROJECT_NAME "-config.json"


std::optional<MyLogFileLifetime> g_LogLifetime;
void ImGuiLayer_WhenMenuIsOpen();
void EveryFrameUntilSuccess_FindAndPatchSoftBodyData();
void EveryFrameUntilSuccess_FindAndUpdateColorsets();
void StartVariousFeatures();

class Halzoid98Plugin : public ACUPluginInterfaceVirtuals
{
public:
    virtual void EveryFrameWhenMenuIsOpen() override
    {
        ImGuiLayer_WhenMenuIsOpen();
    }
    virtual void EveryFrameEvenWhenMenuIsClosed() override
    {
        EveryFrameUntilSuccess_FindAndPatchSoftBodyData();
        EveryFrameUntilSuccess_FindAndUpdateColorsets();
    }
    virtual uint64 GetThisPluginVersion() override
    {
        return MAKE_VERSION_NUMBER_UINT64(0, 0, 1, 0);
    }
    virtual bool InitStage_WhenCodePatchesAreSafeToApply(ACUPluginLoaderInterface& pluginLoader) override
    {
        g_LogLifetime.emplace(AbsolutePathInThisDLLDirectory(LOG_FILENAME));
        MainConfig::FindAndLoadConfigFileOrCreateDefault(AbsolutePathInThisDLLDirectory(CONFIG_FILENAME));
        StartVariousFeatures();
        return true;
    }
} g_thisPlugin;
