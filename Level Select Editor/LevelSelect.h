#pragma once

namespace LevelSelect
{
	inline bool NoLevelIDsFile;

	inline char* LevelIDs[24];
	inline BYTE LevelCount;

	void Init();
	bool LoadCfgFile();
	void SetLevelIDs();
};

