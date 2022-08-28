#include <SADXModLoader.h>
#include <IniFile.hpp>
#include "mod.h"

const HelperFunctions* gHelperFunctions;

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		gHelperFunctions = &helperFunctions;

		// Load config
		const std::string s_path(path);
		const std::string s_config_ini(s_path + "\\config.ini");
		const IniFile* const config = new IniFile(s_config_ini);
		bool EnableNowLoading = config->getBool("Options", "EnableNowLoading", true);
		bool EnableTGSMenus = config->getBool("Options", "EnableTGSMenus", true);

		// Run code
		if (EnableNowLoading)
			NowLoading_Init();
		if (EnableTGSMenus)
			TGSMenus_Init();
	}

	extern "C" __declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}