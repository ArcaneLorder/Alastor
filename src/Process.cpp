#include "Process.h"
#include <shlobj_core.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <processenv.h>
#include <winternl.h>
#else
#include <dirent.h>  
//#include <cstring>  
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#endif

#include "Module.h"

#pragma comment(lib, "psapi.lib")




namespace Alastor
{
#ifdef _WIN32
	typedef enum _PROCESSINFOCLASS {
		ProcessBasicInformation = 0,
		ProcessDebugPort = 7,
		ProcessWow64Information = 26,
		ProcessImageFileName = 27,
		ProcessBreakOnTermination = 29
	} PROCESSINFOCLASS;

	typedef struct _PROCESS_BASIC_INFORMATION64 {
		NTSTATUS ExitStatus;
		UINT32 Reserved0;
		UINT64 PebBaseAddress;
		UINT64 AffinityMask;
		UINT32 BasePriority;
		UINT32 Reserved1;
		UINT64 UniqueProcessId;
		UINT64 InheritedFromUniqueProcessId;
	} PROCESS_BASIC_INFORMATION64;

	typedef struct _PROCESS_BASIC_INFORMATION32 {
		NTSTATUS ExitStatus;
		UINT32 PebBaseAddress;
		UINT32 AffinityMask;
		UINT32 BasePriority;
		UINT32 UniqueProcessId;
		UINT32 InheritedFromUniqueProcessId;
	} PROCESS_BASIC_INFORMATION32;

	typedef NTSTATUS(WINAPI* lpfnNtQueryInformationProcess)(
		HANDLE ProcessHandle,
		PROCESSINFOCLASS ProcessInformationClass,
		PVOID ProcessInformation,
		ULONG ProcessInformationLength,
		PULONG ReturnLength OPTIONAL
		);
	NTSTATUS NtQueryInformationProcess(
		HANDLE ProcessHandle,
		PROCESSINFOCLASS ProcessInformationClass,
		PVOID ProcessInformation,
		ULONG ProcessInformationLength,
		PULONG ReturnLength OPTIONAL)
	{
		HMODULE ntdll = LoadLibrary(TEXT("ntdll.dll"));
		if (ntdll == NULL) 
		{
			return STATUS_DLL_INIT_FAILED;
		}
		lpfnNtQueryInformationProcess fnNtQueryInformationProcess = (lpfnNtQueryInformationProcess)GetProcAddress(ntdll,"NtQueryInformationProcess");
		if (fnNtQueryInformationProcess == NULL)
		{
			return STATUS_INVALID_HANDLE;
		}
		return fnNtQueryInformationProcess(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
	}

	typedef BOOL(WINAPI* lpfnIsWow64Process) (HANDLE, BOOL*);
	bool IsWow64Process(
		HANDLE ProcessHandle,
		BOOL* Wow64Process
	)
	{
		HMODULE kernel32 = LoadLibrary(TEXT("kernel32.dll"));
		if (kernel32 == NULL)
		{
			return false;
		}
		lpfnIsWow64Process fnIsWow64Process = (lpfnIsWow64Process)GetProcAddress(kernel32, "IsWow64Process");
		if (fnIsWow64Process == NULL)
		{
			return false;
		}
		return fnIsWow64Process(ProcessHandle, Wow64Process);
	}

#endif
	Process::Process()
	{

	}

	Process::~Process()
	{
	}


	unsigned int Process::parentId(HANDLE processHandles)
	{
#ifdef _WIN32
		NTSTATUS status;
		PROCESS_BASIC_INFORMATION pbi;

		status = NtQueryInformationProcess(processHandles,
			ProcessBasicInformation,
			(PVOID)&pbi,
			sizeof(PROCESS_BASIC_INFORMATION),
			NULL
		);
		if (status == 0)
		{
			return (unsigned int)pbi.Reserved3;
		}
		return 0;
#else
#endif
	}



	std::vector<PROCESSENTRY32> Process::list() {
		std::vector<PROCESSENTRY32> processList;

#ifdef _WIN32  
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE) {
			// Handle error  
			return processList;
		}

		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &pe32)) {
			do {
				processList.push_back(pe32);
			} while (Process32Next(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);
#else  
		DIR* dir;
		struct dirent* entry;
		if ((dir = opendir("/proc")) != NULL) {
			while ((entry = readdir(dir)) != NULL) {
				if (entry->d_type == DT_DIR) {
					char path[1024];
					snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
					FILE* file = fopen(path, "r");
					if (file) {
						unsigned int pid;
						if (fscanf(file, "Pid:\t%u", &pid) == 1) {
							processList.push_back(pid);
						}
						fclose(file);
					}
				}
			}
			closedir(dir);
		}
#endif  

		return processList;
	}
	bool Process::isAdmin()
	{
		return IsUserAnAdmin() == TRUE;
	}

	bool Process::isWow64(HANDLE processHandle)
	{
		BOOL wowProcess;
		if (IsWow64Process(processHandle, &wowProcess))
		{
			return wowProcess == TRUE;
		}
		return false;
	}

	bool Process::exist(const String processName)
	{
#if defined(_MSC_VER)
		// �������̿���
		HANDLE snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapHandle == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(snapHandle, &pe32)) 
		{
			do {
				if (processName == pe32.szExeFile)
				{
					CloseHandle(snapHandle);
					return true;
				}
			} while (Process32Next(snapHandle, &pe32));
		}
		CloseHandle(snapHandle);
		return false;
#elif defined(__unix__)
		// �����ӽ���
		pid_t pid = fork(); 

		if (pid == -1) 
		{
			perror("fork");
			return false;
		}
		else if (pid > 0) 
		{
			int status;
			// �ȴ��ӽ��̽���
			wait(&status); 
			if (WIFEXITED(status)) 
			{
				// ���ط���ֵ��ʾ���̲�����
				return WEXITSTATUS(status) != 0; 
			}
			else 
			{
				// �����������Ϊ���̴���
				return true; 
			}
		}
		else 
		{
			// ����ϵͳ�����ѯָ�����ƵĽ���
			execlp("/bin/ps", "ps", "-p", processName.data(), nullptr); 
			// ���execlpʧ����˵�����̲�����
			return false;
		}
#else
#endif
	}

	DWORD Process::getId(const String processName)
	{
#if defined(_MSC_VER)
		// �������̿���
		HANDLE snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapHandle == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(snapHandle, &pe32))
		{
			do {
				if (processName == pe32.szExeFile)
				{
					CloseHandle(snapHandle);
					return pe32.th32ProcessID;
				}
			} while (Process32Next(snapHandle, &pe32));
		}
		CloseHandle(snapHandle);
		return NULL;
#elif defined(__unix__)
		DIR* procDir = opendir("/proc");
		if (procDir == nullptr) 
		{
			// perror("Failed to open /proc directory.");
			return NULL;
		}

		struct dirent* entry;
		while ((entry = readdir(procDir)) != nullptr) {
			int pid = atoi(entry->d_name);

			if (pid > 0 && entry->d_type == DT_DIR) {
				FILE* cmdlineFile = fopen(std::to_string(pid).append("/cmdline").c_str(), "r");

				if (cmdlineFile != nullptr) {
					char buffer[4096] = "";

					size_t bytesRead = fread(buffer, sizeof(char), sizeof(buffer) - 1, cmdlineFile);
					fclose(cmdlineFile);

					if (bytesRead > 0) {
						buffer[bytesRead - 1] = '\0';

						if (processName == buffer)
						{
							return pid;
							//std::cout << "Process ID found: " << pid << std::endl;
							//break;
						}
					}
				}
			}
		}

		closedir(procDir);

		return NULL;
#else
#endif
	}

	bool Process::kill(unsigned long processId)
	{
#ifdef _WIN32
		HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
		if (handle == NULL)
		{
			return false;
		}
		BOOL result = TerminateProcess(handle, 0);
		CloseHandle(handle);
		return result != FALSE;
#else
		return ::kill(processId, SIGTERM) == 0;
#endif
	}

	bool Process::kill(const String processName)
	{
		auto processId = getId(processName);
		if (processId)
		{
			return kill(processId);
		}
		return false;
	}

	void Process::openUrl(const String url)
	{
		ShellExecute(NULL, "open", url.data(), NULL, NULL, SW_SHOW);
	}

	bool Process::exec(std::string path, char* command, bool show, bool wait)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = show == true ? SW_SHOW : SW_HIDE;

		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcessA(
			path.c_str(),							// ʹ��ģ����
			command,                        // ������
			NULL,							// ���̾�����ɼ̳�
			NULL,							// �߳̾�����ɼ̳�
			FALSE,							// ���þ�����ɼ̳б�־
			0,								// �޴�����־
			NULL,							// ʹ�ø����̵Ļ�����
			NULL,							// ʹ�ø����̵���������Ŀ¼
			&si,							// ָ��STARTUPINFO�ṹ��ָ��
			&pi)							// ָ��PROCESS_INFORMATION�ṹ��ָ��
			)
		{
			return false;
		}
		// �ȴ��ӽ��̽���
		if (wait)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
		}

		// �رս��̾�����߳̾��
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return true;
	}







	/*
	* ���ã�
	���ý���Ȩ��
	* ���룺
	hToken    �õ�������
	lpszPrivilege    Ҫ�򿪻�رյ�Ȩ����
	bEnablePrivilege    �򿪻��ǹر�Ȩ��
	* �����
	ִ�н��BOOLֵ
	* ǰ��:
	�ǽ���(��ǰ�˻�)�߱���Ȩ�ޣ�ֻ�Ƿ���������û�����ø�Ȩ�ޡ�
	������̵ķ��������б����û�й�����Ȩ�ޣ���AdjustTokenPrivileges��������
	���᷵��ERROR_NOT_ALL_ASSIGNED��ֵΪ1300L���Ĵ����롣
	* ���������
	����ý��̾��и�Ȩ�ޣ�����ͨ����������塱���������ߡ��������ذ�ȫ���ԡ�
	�������ز��ԡ������û�Ȩ��ָ�ɡ����ý���Ȩ�޹�����ָ�����û�������û��ϡ�
	* �ο���ַ��
	https://msdn.microsoft.com/zh-cn/library/windows/desktop/aa446619(v=vs.85).aspx
	*/
	BOOL Process::setPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
	{
		TOKEN_PRIVILEGES tp; // �ýṹ����һ�����飬�������ÿ����ָ����Ȩ�޵����ͺ�Ҫ���еĲ���
		LUID luid;

		// ����
		if (!LookupPrivilegeValue(
			NULL,            // ϵͳ������,null,�ڱ���ϵͳ�ϲ���Ȩ�� lookup privilege on local system
			lpszPrivilege,   // Ҫ�ҵ�Ȩ���� privilege to lookup 
			&luid))        // ͨ��ָ�뷵��Ȩ�޵�LUID receives LUID of privilege
		{
			printf("LookupPrivilegeValue error: %u\n", GetLastError());
			return FALSE;
		}

		tp.PrivilegeCount = 1;    // Ҫ�޸ĵ���Ȩ��Ŀ
		tp.Privileges[0].Luid = luid;    // ����ͬ��Ȩ����
		if (bEnablePrivilege)
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		else
			tp.Privileges[0].Attributes = 0;

		// �����������Ƶ�Ȩ��    Enable the privilege or disable all privileges.
		if (!AdjustTokenPrivileges(
			hToken,// OpenProcessToken������ָ����������ķ������Ƶľ��
			FALSE, // �Ƿ�����������е���Ȩ
			&tp,    // ָ��Ҫ�޸ĵ�Ȩ��
			sizeof(TOKEN_PRIVILEGES),    // PreviousState�ĳ���
			NULL,   // ����޸�ǰ�ķ���Ȩ�޵���Ϣ���ɿ�
			NULL))    // ʵ��PreviousState�ṹ���صĴ�С
		{
			printf("AdjustTokenPrivileges error: %u\n", GetLastError());
			return FALSE;
		}

		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		{
			printf("������NewState������û���������ЩȨ�� \n");
			return FALSE;
		}

		return TRUE;
	}

	bool Process::setDebugPrivilege() {
		HANDLE hToken;
		BOOL bRet = OpenProcessToken(
			GetCurrentProcess(),    // ���̾��(��ǰ����)
			TOKEN_ALL_ACCESS,    // ȫȨ��������
			&hToken    // ���صĲ��� �������ƾ�� ������AdjustTokenPrivileges�ĵ�һ��������
		); // ��ȡ���̵����ƾ��
		if (bRet != TRUE)
			return false;
		BOOL set = setPrivilege(hToken, SE_DEBUG_NAME, TRUE);
		if (!set || GetLastError() != ERROR_SUCCESS)
			return false;
		return true;
	}

	HANDLE Process::open(DWORD processId)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	}

	bool Process::close(HANDLE processHandle)
	{
		return CloseHandle(processHandle) == TRUE;
	}

	std::string Process::path(DWORD processId)
	{

		TCHAR buffer[MAX_PATH] = { 0 };
		DWORD size = MAX_PATH;
		void* handle = Process::open(processId);
		if (handle)
		{
			QueryFullProcessImageName(handle, 0, buffer, &size);
			Process::close(handle);
		}
		return buffer;


		//// �������̿���
		//HANDLE snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		//if (snapHandle == INVALID_HANDLE_VALUE)
		//{
		//	return std::string();
		//}

		//PROCESSENTRY32 pe32 = { 0 };
		//pe32.dwSize = sizeof(PROCESSENTRY32);
		//if (Process32First(snapHandle, &pe32))
		//{
		//	do {
		//		if (processId == pe32.th32ProcessID)
		//		{

		//			std::vector<MODULEENTRY32> modules = Module::list(pe32.th32ProcessID);
		//			for (auto module : modules)
		//			{
		//				if (strcmp(module.szModule, pe32.szExeFile) == 0)
		//				{
		//					CloseHandle(snapHandle);
		//					return module.szExePath;
		//				}
		//			}
		//		}

		//	} while (Process32Next(snapHandle, &pe32));
		//}
		//CloseHandle(snapHandle);
		//return std::string();
	}



	std::string Process::command(HANDLE processHandle)
	{
		std::string commandLine;
		PEB peb;
		RTL_USER_PROCESS_PARAMETERS upps;
		PROCESS_BASIC_INFORMATION pbi;
		NTSTATUS isok = NtQueryInformationProcess(processHandle, ProcessBasicInformation, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), 0);
		if (BCRYPT_SUCCESS(isok))
		{
			if (ReadProcessMemory(processHandle, pbi.PebBaseAddress, &peb, sizeof(PEB), 0))
			{
				if (ReadProcessMemory(processHandle, peb.ProcessParameters, &upps, sizeof(RTL_USER_PROCESS_PARAMETERS), 0)) {
					WCHAR *buffer = new WCHAR[upps.CommandLine.Length + 1];
					ZeroMemory(buffer, (upps.CommandLine.Length + 1) * sizeof(WCHAR));
					ReadProcessMemory(processHandle, upps.CommandLine.Buffer, buffer, upps.CommandLine.Length, 0);
					commandLine = Codec::unicodeToAnsi(buffer);
					delete buffer;
					return commandLine;
				}
			}
		}
		return std::string();
	}
}