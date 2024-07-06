#pragma once
#include "Alastor.h"





namespace Alastor {

	class ALASTOR_API System {
	public:
		static bool isWow64();
		static std::string version();
		//static std::string hardwareSign();
	};
}