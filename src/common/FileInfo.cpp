#include "FileInfo.h"

#include <windows.h>
#include <iostream>

bool FileInfoUtils::QueryValue(const std::wstring wsValueName, const std::wstring wsModuleName, std::wstring &wsRetStr)
{
	bool bSuccess = FALSE;
	BYTE *lpVersionData = NULL;
	DWORD  dwLangCharset = 0;
	wchar_t *pStr = NULL;

	do
	{
		if (wsValueName.empty() || wsModuleName.empty())
			break;

		DWORD dwHandle;
		// 判断系统能否检索到指定文件的版本信息
		DWORD dwDataSize = ::GetFileVersionInfoSizeW((LPCWSTR)wsModuleName.c_str(), &dwHandle);
		if (dwDataSize == 0)
			break;

		lpVersionData = new (std::nothrow) BYTE[dwDataSize];// 分配缓冲区
		if (NULL == lpVersionData)
			break;

		// 检索信息
		if (!::GetFileVersionInfoW((LPCWSTR)wsModuleName.c_str(), dwHandle, dwDataSize, (void*)lpVersionData))
			break;

		UINT nQuerySize;
		DWORD *pTransTable;
		// 设置语言
		if (!::VerQueryValueW(lpVersionData, L"\\VarFileInfo\\Translation", (void **)&pTransTable, &nQuerySize))
			break;

		dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
		if (lpVersionData == NULL)
			break;

		pStr = new (std::nothrow) wchar_t[128];// 分配缓冲区
		if (NULL == pStr)
			break;

		//wchar_t str2[MAX_PATH];
		//wcscpy_s(str2, MAX_PATH, wsValueName.c_str());
		swprintf_s(pStr, 128, L"\\StringFileInfo\\%08lx\\%s", dwLangCharset, wsValueName.c_str());

		LPVOID lpData;

		// 调用此函数查询前需要先依次调用函数GetFileVersionInfoSize和GetFileVersionInfo
		if (::VerQueryValueW((void *)lpVersionData, pStr, &lpData, &nQuerySize))
			wsRetStr = (wchar_t*)lpData;

		bSuccess = TRUE;

	} while (FALSE);

	// 销毁缓冲区
	if (lpVersionData)
	{
		delete[] lpVersionData;
		lpVersionData = NULL;
	}
	if (pStr)
	{
		delete[] pStr;
		pStr = NULL;
	}

	return bSuccess;
}
