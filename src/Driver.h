#pragma once
#include "Alastor.h"


#ifdef _WIN32
#include <winternl.h>
#endif

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY {
	HANDLE Section;
	PVOID MappedBase;
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT PathLength;
	CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG Count;	//ģ���ܸ��� �����ȫ������SYSTEM_MODULE_INFORMATION_ENTRY�ṹ ϵͳ������е�ģ����Ϣ��д���������������
	SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;


namespace Alastor
{
	class ALASTOR_API Driver {
	public:

		Driver();
		~Driver();


		bool load(
			std::string path,
			std::string deviceName = "",
			std::string symbolName = "",
			unsigned long startOptions = SERVICE_DEMAND_START
		);

		bool unload();

		int getControlCode(int function, int deviceType = FILE_DEVICE_UNKNOWN, int method = METHOD_BUFFERED, int access = FILE_ANY_ACCESS);

		bool control(int code, void* input, int inputLength, void* output, int outputLength);

		std::string lasterror();

	public:
		static std::vector<SYSTEM_MODULE_INFORMATION_ENTRY> list();

	private:
		std::string m_path;
		std::string m_deviceName;
		std::string m_symbolName;
		std::string m_serviceName;
		HANDLE m_deviceHandle;
		int m_error;
	};
}

