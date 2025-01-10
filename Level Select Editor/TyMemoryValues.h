#pragma once
#include "LevelSelect.h"

namespace TyMemoryValues {

	DWORD TyBaseAddress;

	//When looping through the text to draw the UI
	inline int* PtrToLevelIDs() { return (int*)(TyBaseAddress + 0xE388D); };
	//When looping through the text to draw the UI
	inline int* PtrToEndOfLevelIDs() { return (int*)(TyBaseAddress + 0xE38D5); };

	//When a level has been selected to load into
	inline int* GetSelectedLevel() { return (int*)(TyBaseAddress + 0xE4DA8); };


	inline BYTE* TextPropertiesMallocCountPtr() { return (BYTE*)(TyBaseAddress + 0xE4965); };
	inline int* TextPropertiesArrayLength() { return (int*)(TyBaseAddress + 0xE49D8); };
}