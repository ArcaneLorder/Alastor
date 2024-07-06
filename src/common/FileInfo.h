#pragma once
#include <string>
class FileInfoUtils
{
public:
	static bool GetFileDescription(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"FileDescription", szModuleName, RetStr); }
	static bool	GetFileVersion(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"FileVersion", szModuleName, RetStr); }
	static bool	GetInternalName(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"InternalName", szModuleName, RetStr); }
	static bool	GetCompanyName(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"CompanyName", szModuleName, RetStr); }
	static bool	GetLegalCopyright(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"LegalCopyright", szModuleName, RetStr); }
	static bool	GetOriginalFilename(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"OriginalFilename", szModuleName, RetStr); }
	static bool	GetProductName(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"ProductName", szModuleName, RetStr); }
	static bool	GetProductVersion(const std::wstring& szModuleName, std::wstring& RetStr) { return QueryValue(L"ProductVersion", szModuleName, RetStr); }

private:
	static bool QueryValue(const std::wstring wsValueName, const std::wstring wsModuleName, std::wstring &wsRetStr);
};
