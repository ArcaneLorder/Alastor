#include "Hardware.h"
#include "HardDriveSerialNumber.h"


#include <Windows.h>
#include <intrin.h>
#pragma comment(lib, "Netapi32.lib")

namespace Alastor
{
	String Hardware::cpuid()
	{
		char buf[32] = { 0 };
		std::string strCPUId;
		unsigned long s1=0, s2=0;

		
#if defined(_WIN64)
		// 64位下不支持内联汇编. 应使用__cpuid、__cpuidex等Intrinsics函数。
		int32_t Infobuf[4];
		__cpuid(Infobuf, 1);
		s1 = Infobuf[3];
		s2 = Infobuf[0];
#else
		__asm
		{
			pushad // 保存寄存器环境
			mov eax, 01h
			xor ecx, ecx
			xor edx, edx
			cpuid
			mov s1, edx
			mov s2, ecx
			popad // 恢复寄存器环境
		}
#endif
		if (s1)
		{
			memset(buf, 0, sizeof(buf));
			sprintf_s(buf, sizeof(buf), "%08X", s1);
			strCPUId += buf;
		}

		if (s2)
		{
			memset(buf, 0, sizeof(buf));
			sprintf_s(buf, sizeof(buf), "%08X", s2);
			strCPUId += buf;
		}


#if defined(_WIN64)
		// 64位下不支持内联汇编. 应使用__cpuid、__cpuidex等Intrinsics函数。
		__cpuid(Infobuf, 3);
		s1 = Infobuf[3];
		s2 = Infobuf[0];
#else
		__asm
		{
			pushad
			mov eax, 03h
			xor ecx, ecx
			xor edx, edx
			cpuid
			mov s1, edx
			mov s2, ecx
			popad
		}
#endif
		if (s1)
		{
			memset(buf, 0, sizeof(buf));
			sprintf_s(buf, sizeof(buf), "%08X", s1);
			strCPUId += buf;
		}

		if (s2)
		{
			memset(buf, 0, sizeof(buf));
			sprintf_s(buf, sizeof(buf), "%08X", s2);
			strCPUId += buf;
		}
		std::cout << strlen(strCPUId.c_str()) << std::endl;
		return strCPUId;

	}

	String Hardware::disk()
	{
		ByteArray buffer;
		buffer.resize(1024);
		MasterHardDiskSerial a;
		a.GetSerialNo(buffer.data());
		return buffer;
	}


	bool GetAdapterInfo(int adapterNum, String& macOUT)
	{
		NCB Ncb;
		memset(&Ncb, 0, sizeof(NCB));
		// 重置网卡，以便我们可以查询
		Ncb.ncb_command = NCBRESET; 
		Ncb.ncb_lana_num = adapterNum;
		if (Netbios(&Ncb) != NRC_GOODRET)
		{
			return false;
		}

		// 准备取得接口卡的状态块
		memset(&Ncb, sizeof(Ncb), 0);
		Ncb.ncb_command = NCBASTAT;
		Ncb.ncb_lana_num = adapterNum;
		strcpy_s((char*)Ncb.ncb_callname,16, "*");
		struct ASTAT
		{
			ADAPTER_STATUS adapt;
			NAME_BUFFER nameBuff[30];
		}adapter;
		memset(&adapter, sizeof(adapter), 0);
		Ncb.ncb_buffer = (unsigned char*)&adapter;
		Ncb.ncb_length = sizeof(adapter);
		if (Netbios(&Ncb) != 0)
		{
			return false;
		}

		char acMAC[32] = {0};
		sprintf_s(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
			int(adapter.adapt.adapter_address[0]),
			int(adapter.adapt.adapter_address[1]),
			int(adapter.adapt.adapter_address[2]),
			int(adapter.adapt.adapter_address[3]),
			int(adapter.adapt.adapter_address[4]),
			int(adapter.adapt.adapter_address[5]));
		macOUT = acMAC;
		return true;
	}


	String Hardware::mac()
	{
		String buffer;
		// 取得网卡列表
		LANA_ENUM adapterList;
		NCB Ncb;
		memset(&Ncb, 0, sizeof(NCB));
		Ncb.ncb_command = NCBENUM;
		Ncb.ncb_buffer = (unsigned char*)&adapterList;
		Ncb.ncb_length = sizeof(adapterList);
		Netbios(&Ncb);

		// 取得MAC
		for (int i = 0; i < adapterList.length; ++i)
		{
			if (GetAdapterInfo(adapterList.lana[i], buffer))
				return buffer;
		}

		return String();
	}

}
