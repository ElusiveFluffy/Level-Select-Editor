#include "pch.h"
#include "LevelSelect.h"
#include "TygerFrameworkAPI.hpp"
#include "TyMemoryValues.h"
#include <fstream>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

using namespace TyMemoryValues;

void LevelSelect::Init()
{
    TyMemoryValues::TyBaseAddress = (DWORD)GetModuleHandle(0);

	if (LoadCfgFile())
		SetLevelIDs();
}

bool LevelSelect::LoadCfgFile()
{
	//Can't use the API here since it needs to be early initialized
#ifdef _DEBUG
	std::string cfgPath = "Debug Plugins\\Level Select IDs.cfg";
#endif // DEBUG
#ifdef NDEBUG
	std::string cfgPath = "Plugins\\Level Select IDs.cfg";
#endif // RELEASE

	if (!fs::exists(cfgPath)) {
		NoLevelIDsFile = true;
		return false;
	}

	std::ifstream cfgFileLevelIDs(cfgPath);
	std::string levelID;

	//Get all the IDs (1 per line)
	while (std::getline(cfgFileLevelIDs, levelID)) {
		//Too many IDs, the array currently is only 24 indexes
		if (LevelCount == 24) {
			TooManyIDs = true;
			break;
		}

		//+ 1 for the null terminator
		int length = levelID.length() + 1;

		//Dynamically allocate memory for them
		LevelIDs[LevelCount] = new char[length];
		strcpy_s(LevelIDs[LevelCount], length, levelID.c_str());

		LevelCount++;
	}

	return true;
}


void LevelSelect::SetLevelIDs() {
	DWORD oldProtection;
	//Change the memory access to ReadWrite to be able to change the hardcoded value (usually its read only)
	VirtualProtect(PtrToLevelIDs(), 4, PAGE_EXECUTE_READWRITE, &oldProtection);

	//Start
	*PtrToLevelIDs() = (int)&LevelIDs[0];

	//End
	*PtrToEndOfLevelIDs() = (int)&LevelIDs[LevelCount];

	//Set it back to the old access protection
	VirtualProtect(PtrToLevelIDs(), 4, oldProtection, &oldProtection);



	VirtualProtect(GetSelectedLevel(), 4, PAGE_EXECUTE_READWRITE, &oldProtection);

	//When a level has been selected to load into
	*GetSelectedLevel() = (int)&LevelIDs[0];

	//Set it back to the old access protection
	VirtualProtect(GetSelectedLevel(), 4, oldProtection, &oldProtection);



	//Edit the memory allocator to create the right amount of entries, so it doesn't crash
	VirtualProtect(TextPropertiesMallocCountPtr(), 1, PAGE_EXECUTE_READWRITE, &oldProtection);

	*TextPropertiesMallocCountPtr() = LevelCount;
	//The property array is 360 (0x168) bytes long
	*TextPropertiesArrayLengthPtr() = LevelCount * 0x168;

	VirtualProtect(TextPropertiesMallocCountPtr(), 1, oldProtection, &oldProtection);

}