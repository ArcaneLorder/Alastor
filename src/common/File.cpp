
#include "File.h"
#include <fstream>
#include <string.h>
//#include <sys/stat.h>
#include <stdio.h>
#include <Windows.h>
#include <io.h>
#include <map>
#include <shellapi.h>
#include "MD5.h"


#include "CResourceExtractor.h"


#include "FileInfo.h"
#include "Codec.h"

namespace Alastor
{
	File::File()
	{
		m_file = 0;
	}

	File::~File()
	{
		// 析构自动关闭文件
		this->close();
	}

	bool File::open(const String& path, OPEN_TYPE type)
	{
		const char* access = nullptr;

		switch (type)
		{
		case OPEN_TYPE::ReadText:
			access = "r";
			break;
		case OPEN_TYPE::ReadAndWriteText:
			access = "r+";
			break;
		case OPEN_TYPE::ReadBytes:
			access = "rb";
			break;
		case OPEN_TYPE::ReadAndWriteBytes:
			access = "rb+";
			break;
		case OPEN_TYPE::CreateClearReadText:
			access = "w";
			break;
		case OPEN_TYPE::CreateClearReadAndWriteText:
			access = "w+";
			break;
		case OPEN_TYPE::CreateClearReadBytes:
			access = "wb";
			break;
		case OPEN_TYPE::CreateClearReadAndWriteBytes:
			access = "wb+";
			break;
		case OPEN_TYPE::CreateWriteTextTail:
			access = "a";
			break;
		case OPEN_TYPE::CreateWriteBytesTail:
			access = "ab";
			break;
		case OPEN_TYPE::CreateReadAndWriteBytesTail:
			access = "ab+";
			break;
		default:
			access = "ab+";
		}
		errno_t error = fopen_s(&this->m_file, path.data(), access);
		if (error != 0) // 成功返回0 失败返回错误代码
		{
			std::cout << "打开文件失败:" << error << std::endl;
			return false;
		}
		this->m_path = path;
		return true;
	}

	bool File::close()
	{

		if (m_file == NULL)
		{
			return true;
		}
			
		if (fclose(this->m_file) != 0)
		{
			this->m_file = NULL;
			return false;
		}

		this->m_file = NULL;
		this->m_path = NULL;
		return true;

	}

	bool File::flush()
	{
		if (m_file == nullptr)
		{
			return false;
		}
		return fflush(this->m_file) == 0;
	}

	bool File::clear()
	{
		// 返回0文件尾部
		return feof(m_file) == 0;
	}

	bool File::read(size_t index, size_t size, ByteArray& buffer) const
	{
		// 判断文件句柄是否打开
		if (this->m_file == NULL)
		{
			return false;
		}

		// 移动读写位置到文件头
#ifdef _WIN64
		if (_fseeki64(this->m_file, index, 0) != 0)
#else
		if (fseek(this->m_file, index, 0) != 0)
#endif
		{
			return false;
		}
		// 读取1个size的大小
		buffer.resize(size);
		if (fread_s(buffer.data(), SIZE_MAX, 1, size, this->m_file) != size)
		{
			return false;
		}
		return true;
	}

	bool File::write(const char* buffer, size_t size)
	{
		if (this->m_file == NULL)
		{
			return false;
		}
		if (!this->seek(0))
		{
			return false;
		}
		
		// 写入1个size的大小 失败
		//std::cout << "最终写入大小:" << buffer.size() << std::endl;
		if (fwrite(buffer, size, 1, this->m_file) == 1)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool File::write(const String& value)
	{
		// 缓冲区大小加上结尾空字符
		return write(value.data(), value.size() + sizeof('\0'));
	}

	bool File::write(const ByteArray& value)
	{
		return write(value.data(), (size_t)value.length());
	}

	bool File::seek(size_t index) const
	{
#ifdef _WIN64
		if (_fseeki64(m_file, index, SEEK_SET) == 0)
#else
		if (fseek(m_file, index, SEEK_SET) == 0)
#endif
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const String& File::filepath() const
	{
		return this->m_path;
	}

	char File::operator[](size_t index) const
	{

		if (!seek(index) == false)
		{
			return '\0';
		}
		ByteArray result;
		if (read(index, 1, result))
		{
			return result[0];
		}
		return '\0';
	}


	bool File::size(const String& filepath, size_t& size)
	{
		FILE* file;
		if (fopen_s(&file, filepath.data(), "rb+"))
		{
			// std::cout << "打开文件失败" << std::endl;
			return false;
		}
#ifdef _WIN64
		size = _filelengthi64(_fileno(file));
#else
		size = (size_t)_filelength(_fileno(file));
#endif
		fclose(file);
		return true;
	}

	bool File::exist(const String& filepath)
	{
#ifdef _WIN32
		return _access(filepath.toNativeSeparators().data(), 0) != -1;
#else
		return access(filepath.toNativeSeparators().data(), F_OK) == 0;
#endif
	}

	bool File::remove(const String& filepath)
	{
		if (::remove(filepath.toNativeSeparators().data()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool File::rename(const String& src, const String& dst)
	{
		return false;
	}

	 void File::list(const std::string& path, std::vector<std::string>& list, bool recursion) 
	 {
		// 文件句柄  
		intptr_t handle = 0;

		// 文件信息  
		struct _finddata_t fileinfo;

		std::string searchPattern = path;
		// 如果路径不以\结尾，则添加一个\  
		if (searchPattern.back() != '\\') {
			searchPattern += "\\";
		}
		searchPattern += "*";

		if ((handle = _findfirst(searchPattern.c_str(), &fileinfo)) != -1) {
			do {
				// 如果是目录，并且需要递归，则递归之  
				if ((fileinfo.attrib & _A_SUBDIR) != 0) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
						if (recursion) {
							std::string newPath = path;
							// 如果路径不以\结尾，则添加一个\  
							if (newPath.back() != '\\') {
								newPath += "\\";
							}
							newPath += fileinfo.name;
							File::list(newPath, list, recursion);
						}
					}
				}
				else {
					// 如果是文件，则添加到列表中  
					list.push_back(fileinfo.name);
				}
			} while (_findnext(handle, &fileinfo) == 0);

			_findclose(handle);
		}
	}

	bool File::write(const String& filepath, const char* data, size_t len)
	{

		String _filepath = filepath.toNativeSeparators();
		if (exist(_filepath))
		{
			if (!File::remove(_filepath.data()))
			{
				return false;
			}
		}
		File file;
		if (!file.open(_filepath, OPEN_TYPE::CreateClearReadBytes))
		{
			return false;
		}

		if (!file.write(data, len))
		{
			return false;
		}

		file.close();
		return true;
	}
	bool  File::write(const String& filepath, const ByteArray& data)
	{
		return File::write(filepath, data.data(), data.size());
	}
	bool File::write(const String& filepath, const String& data)
	{
		return File::write(filepath, data.data(), data.size() + sizeof('\0'));
	}
	bool File::append(const String& filepath, const ByteArray& data)
	{
		std::ofstream file(filepath, std::ios::binary | std::ios::app);

		// 检查文件流状态
		if (file.good())
		{
			// 输出结尾字符串
			//file << content << "\n";
			// 关闭文件流
			file.close();
			return true;
		}
		else
		{
			return false;
		}
	}
	bool  File::read(const String& filepath, ByteArray& data, OPEN_TYPE type)
	{
		String formatPath = filepath.toNativeSeparators();
		data.clear();
		if (formatPath.size() == 0)
		{
			return false;
		}
			
		File file;
		size_t size = 0;
		if (File::size(formatPath, size) == false)
		{
			return false;
		}
		data.resize(size);
		if (!file.open(formatPath, type))
		{
			return false;
		}

		if (size == 0)
		{
			file.close();
			return true;
		}
			
		if (!file.read(0, size, data))
		{
			file.close();
			return false;
		}
		file.close();
		return true;
	}

	//bool  File::copy_dir(const String& src, const String& dst, bool cover)
	//{
	//	File::create_dir(File::format_slash(dst));
	//	std::list<String> dirs;
	//	std::list<String> files;
	//	if (!File::ergodic_all(File::format_slash(src), dirs, files))
	//	{
	//		return false;
	//	}
	//	
	//	for(auto iter:dirs)
	//	{
	//		File::create_dir(File::format_slash(dst) + (iter).right(iter.size() - File::format_slash(src).size()));
	//	}
	//		
	//	for(auto iter:files)
	//	{
	//		File::copy_file(iter, File::format_slash(dst) + (iter).right(iter.size() - File::format_slash(src).size()), cover);
	//	}
	//		
	//	return true;
	//}
	//void File::ext(const String& path, String& ext)
	//{
	//	size_t idx = 0;
	//	if (!path.find('.', idx, 0, true))
	//	{
	//		return;
	//	}
	//	size_t sublen = path.size() - idx - 1;
	//	ext.append(path.data() + path.size() - sublen, sublen);
	//}
	bool  File::copy(const String& src, const String& dst, bool cover)
	{
		if (CopyFileA(src.data(), dst.data(), !cover))
		{
			return true;
		}
		return false;
	}
//	bool  File::remove_dir(const String& dirpath, bool recycle)
//	{
//		String _dirpath = File::format_slash(dirpath);
//		if (recycle == false)
//		{
//			std::list<String> dirs;
//			std::list<String> files;
//			if (!File::ergodic_all(_dirpath, dirs, files))
//			{
//				return false;
//			}
//
//			for (auto iter : files)
//			{
//				File::remove((iter));
//			}
//				
//			for (auto rit = dirs.rbegin(); rit != dirs.rend(); rit++)
//			{
//#ifdef WIN32
//				return RemoveDirectoryA(rit->data());
//#else
//#endif
//			}
//#ifdef WIN32
//			return RemoveDirectoryA(dirpath.data());
//#else
//#endif			
//		}
//		else
//		{
//#ifdef _WIN32
//			SHFILEOPSTRUCTA  shDelFile;
//			memset(&shDelFile, 0, sizeof(SHFILEOPSTRUCTA));
//			shDelFile.fFlags |= FOF_SILENT;
//			shDelFile.fFlags |= FOF_NOERRORUI;
//			shDelFile.fFlags |= FOF_NOCONFIRMATION;
//
//			shDelFile.wFunc = FO_DELETE;
//			ByteArray path;
//			path += dirpath;
//			path += '\0';
//			path += '\0';
//			shDelFile.pFrom = (LPCSTR)path.data();
//			shDelFile.pTo = NULL;
//			shDelFile.fFlags |= FOF_ALLOWUNDO;
//			BOOL bres = SHFileOperationA(&shDelFile);
//			return !bres;
//#else
//#endif
//		}
//	}

	String  File::directory(const String& path)
	{
		// 格式化为native格式斜杠路径
		String _path = path.toNativeSeparators();

		size_t idx = 0;
		
		size_t idx2 = 0;
		

		if (!_path.lastIndexOf("\\", idx))
		{
			if (!_path.lastIndexOf("/", idx))
			{
				return "";
			}
			else
				idx = idx2;
		}
		return _path.mid(0, idx);
	}



	String File::filename(const String& path)
	{
		String _path = path.toNativeSeparators();
		size_t index = 0;
		if (!_path.lastIndexOf('\\',index))
		{
			return path;
		}
		return _path.right(index);
	}

	std::string File::suffix(const std::string & path)
	{
		// 从最后一个点（'.'）开始反向搜索
		auto dot_pos = path.rfind('.');

		// 如果没有找到点，则没有后缀
		if (dot_pos == std::string::npos) {
			return "";
		}

		// 否则，返回点之后的部分作为后缀
		return path.substr(dot_pos + 1);
	}

	ByteArray File::icon(const std::string& exePath, int iconIndex)
	{
		CResourceExtractor res;
		res.Load(exePath);
		std::string tempPath = File::tempPath();
		res.ExtractIconToFile(0,0, tempPath);
		ByteArray buffer;
		File::read(tempPath,buffer);
		return buffer;
	}
	std::string File::tempPath()
	{
		char tempPath[MAX_PATH];
		GetTempPathA(MAX_PATH, tempPath);
		char tempFile[MAX_PATH];
		GetTempFileNameA(tempPath, "TMP", 0, tempFile);

		// 在这里，tempFile是一个存在的临时文件的路径。如果你只需要文件名，你可以使用PathFindFileName函数来提取它。  
		// 但是，请注意，这个函数创建了一个真实的临时文件，所以你可能需要删除它。  
		DeleteFileA(tempFile);
		return std::string(tempFile);
	}
	ByteArray File::resources(char* type, char* index)
	{

		HMODULE module = GetModuleHandle(NULL);//LoadLibrary(TEXT("C:\\Users\\fengmu\\Desktop\\Browser.exe"));//

		HRSRC resHandle = NULL;
		resHandle = FindResourceA(module,index,type);
		if (!resHandle)
		{
			//std::cout << "资源句柄失败" << std::endl;
			return ByteArray();
		}
		DWORD size = SizeofResource(module, resHandle);
		if (!size)
		{
			//std::cout << "资源大小失败" << std::endl;
			return ByteArray();
		}

		HGLOBAL res = LoadResource(module, resHandle);
		if (!res)
		{
			//std::cout << "资源加载失败" << std::endl;
			return ByteArray();
		}
		
		return ByteArray(reinterpret_cast<char*>(res), size);
	}
	std::string File::description(const std::string & path)
	{
		std::wstring description;
		if (FileInfoUtils::GetFileDescription(Codec::ansiToUnicode(path).data(), description))
		{
			return Codec::unicodeToUtf8(description);
		}
		
		return std::string();
	}
	std::string File::company(const std::string & path)
	{
		std::wstring description;
		if (FileInfoUtils::GetCompanyName(Codec::ansiToUnicode(path).data(), description))
		{
			return Codec::unicodeToUtf8(description);
		}
		return std::string();
	}
	bool File::move(const std::string & src, const std::string & dst)
	{
		// 将std::string转换为C风格的字符串（const char*）  
		const char* src_cstr = src.c_str();
		const char* dst_cstr = dst.c_str();

		// 调用MoveFileEx函数尝试移动文件  
		// 第三个参数用于指定移动选项，这里我们使用MOVEFILE_REPLACE_EXISTING来替换目标位置已存在的文件  
		if (MoveFileExA(src_cstr, dst_cstr, MOVEFILE_REPLACE_EXISTING) == TRUE) {
			return true; // 移动成功  
		}
		else {
			// 获取并保存最后的错误代码  
			DWORD error = GetLastError();
			// 可以选择抛出异常或记录错误，这里简单打印错误信息  
			std::cerr << "Failed to move file: " << src << " to " << dst << ". Error code: " << error << std::endl;
			return false; // 移动失败  
		}
	}
}


