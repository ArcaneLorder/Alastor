#include "Directory.h"
#include <Windows.h>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#include <shlobj_core.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define GetCurrentDir getcwd
#endif

#ifdef _WIN32
#pragma comment(lib,"shell32.lib")
#endif 

namespace Alastor 
{
	Directory::Directory()
	{
	}


	std::vector<std::string> Directory::drives()
	{
		std::vector<std::string> list;
		char driveBuffer[4];
		driveBuffer[3] = '\0';
		driveBuffer[2] = '\\';
		driveBuffer[1] = ':';

		// 遍历所有逻辑驱动器
		for (char letter = 'A'; letter <= 'Z'; ++letter) {
			driveBuffer[0] = letter;

			// 尝试打开驱动器根目录
			if (GetDriveType(driveBuffer) == DRIVE_FIXED) {
				list.push_back(driveBuffer);
			}
		}
		return list;
	}

	std::vector<std::string> Directory::sub(const std::string& path, bool recursion)
	{
#ifdef _WIN32
		std::vector<std::string> subDirs;
		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile((path + (path.back() == '\\' ? "*" : "\\*")).c_str(), &findData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					if (std::string(findData.cFileName) != "." && std::string(findData.cFileName) != "..")
					{
						subDirs.push_back(findData.cFileName);
						if (recursion)
						{
							std::string newPath = path + "\\" + findData.cFileName;
							auto childSubDirs = sub(newPath, recursion);
							subDirs.insert(subDirs.end(), childSubDirs.begin(), childSubDirs.end());
						}
					}
				}
			} while (FindNextFile(hFind, &findData));

			FindClose(hFind);
		}
		return subDirs;
#else
		std::vector<std::string> subDirs;
		DIR* dir;
		struct dirent* ent;

		if ((dir = opendir(path.c_str())) != NULL)
		{
			while ((ent = readdir(dir)) != NULL)
			{
				if (ent->d_type == DT_DIR)
				{
					if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
					{
						subDirs.push_back(ent->d_name);
						if (recursion)
						{
							std::string newPath = path + "/" + ent->d_name;
							auto childSubDirs = subDir(newPath, recursion);
							subDirs.insert(subDirs.end(), childSubDirs.begin(), childSubDirs.end());
						}
					}
				}
			}
			closedir(dir);
		}

		return subDirs;
#endif
	}

	std::string Directory::current()
	{
		char buffer[FILENAME_MAX];
		if (GetCurrentDir(buffer, sizeof(buffer)))
		{
			return std::string(buffer) + "\\";
		}
		return std::string();
	}

	String  Directory::parent(const String& path)
	{
		// 格式化为native格式斜杠路径
		String _path = path.toNativeSeparators();

		size_t idx = 0;

		size_t idx2 = 0;


		if (!_path.lastIndexOf("\\", idx, 0))
		{
			if (!_path.lastIndexOf("/", idx2, 0))
			{
				return "";
			}
			else
				idx = idx2;
		}
		return _path.mid(0, idx, true);
	}

	bool Directory::create(const String& filepath, bool recursion)
	{
		if (recursion == false)
		{
			return _mkdir(filepath.data());
		}
		else
		{
			std::vector<String> directorys;
			String td = filepath;
			directorys.push_back(td);
			while ((td = Directory::parent(td)) != "")
			{
				directorys.push_back(td);
			}
			for (size_t i = directorys.size(); i > 0; i--)
			{
				if (!Directory::create(directorys[i - 1]))
				{
					return false;
				}
			}
			return true;
		}
	}


	String  Directory::format(const String& path, bool tailSlash)
	{

		String result = path.toNativeSeparators();
		if (result.size() == 0)
		{
			return path;
		}
			
		if (tailSlash)
		{
			if (result[result.size() - 1] != '\\')
				result += '\\';
		}
		else
		{
			if (result[result.size() - 1] == '\\')
				result = result.left(result.size() - 1);
		}
		return result;
	}

	std::string Directory::temp()
	{
		char tempPath[MAX_PATH];
		GetTempPathA(MAX_PATH, tempPath);

		return std::string(tempPath);
	}

	bool Directory::remove(const std::string & path)
	{
		std::string searchPath = path;
		// 确保searchPath以斜杠结尾，除非它已经是根目录  
		if (!searchPath.empty() && searchPath.back() != '\\') {
			searchPath += "\\";
		}
		searchPath += "*"; // 添加通配符以搜索目录中的所有文件和子目录  

		WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE) {
			// 检查是否是“目录不存在”的错误  
			if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_NO_MORE_FILES) {
				return true; // 如果目录不存在，则视为成功删除  
			}
			return false; // 其他错误，返回失败  
		}

		do {
			if (std::strcmp(ffd.cFileName, ".") != 0 && std::strcmp(ffd.cFileName, "..") != 0) {
				std::string subPath = path;
				if (!subPath.empty() && subPath.back() != '\\') {
					subPath = subPath +  "\\" + ffd.cFileName;
				}
				else
				{
					subPath +=  ffd.cFileName;
				}
				//std::string subPath = path + "\\" + ffd.cFileName; // 总是添加斜杠，因为我们已经确保了path以斜杠结尾  

				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (!remove(subPath)) {
						FindClose(hFind);
						return false;
					}
				}
				else {
					if (!DeleteFileA(subPath.c_str())) {
						FindClose(hFind);
						return false;
					}
				}
			}
		} while (FindNextFileA(hFind, &ffd));

		FindClose(hFind);

		// 尝试删除目录本身，此时它应该为空  
		return RemoveDirectoryA(path.c_str()) != 0;
	}

	std::string Directory::desktop()
	{
		TCHAR buffer[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, buffer);
		return std::string(buffer) +"\\";
	}



}
