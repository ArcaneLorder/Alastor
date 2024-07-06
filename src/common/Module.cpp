#include "Module.h"


namespace Alastor
{
	Module::Module()
	{
	}

	std::vector<MODULEENTRY32> Module::list(DWORD processId)
	{

		std::vector<MODULEENTRY32> list;

		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };

		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
		if (hModuleSnap == INVALID_HANDLE_VALUE)
		{
			return list;
		}

		if (!Module32First(hModuleSnap, &me32))
		{
			CloseHandle(hModuleSnap);
			return list;
		}

		do {
			list.push_back(me32);
		} while (Module32Next(hModuleSnap, &me32));
		CloseHandle(hModuleSnap);
		return list;
	}

}
