// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "LevelSelect.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LevelSelect::Init();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

EXTERN_C void TygerFrameworkPluginRequiredVersion(TygerFrameworkPluginVersion* version) {
    //Specifiy the version number defined in the API
    version->Major = TygerFrameworkPluginVersion_Major;
    version->Minor = TygerFrameworkPluginVersion_Minor;
    version->Patch = TygerFrameworkPluginVersion_Patch;

    version->CompatibleGames = { 1 };
}

EXTERN_C bool TygerFrameworkPluginInitialize(TygerFrameworkPluginInitializeParam* param) {

    API::Initialize(param);

    //Log any errors/warnings here, since the API wasn't initialized when setting up the level select values
    if (LevelSelect::NoLevelIDsFile) {
        API::LogPluginMessage("Failed to Set the Level Select IDs, Level IDs cfg File is Missing!");
        param->initErrorMessage = "Level IDs cfg File is Missing!";
        return false;
    }

    if (LevelSelect::TooManyIDs)
        API::LogPluginMessage("Only Successfully Set the First 24 Level Select IDs (24 is currently the max allowed)", Warning);
    else
        API::LogPluginMessage("Successfully Set the Level Select IDs");

    return true;
}