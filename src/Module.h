#pragma once
#include "Alastor.h"

namespace Alastor
{
	class ALASTOR_API Module
	{
	public:
		explicit Module();

	public:
		static std::vector<MODULEENTRY32> list(DWORD processId);
	};
}
