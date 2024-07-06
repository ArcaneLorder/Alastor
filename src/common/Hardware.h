#pragma once
#include "Alastor.h"
#include "String.h"



namespace Alastor {

	class ALASTOR_API Hardware {
	public:
		 static String cpuid();
		 static String disk();
		 static String mac();
	};
}