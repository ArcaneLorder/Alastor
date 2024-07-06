#pragma once
#include "Common.h"

#ifdef _WIN32
#include <windows.h>
namespace Alastor{

	class ALASTOR_API Imitate {
	public:
		explicit Imitate();
	public:
		static void textMessage();
		static void mouseMessage(HWND hWnd, int x, int y, int mouseButton, int mouseAction, bool wait = false);
	};
}

#endif