
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
		// �����Զ��ر��ļ�
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
		if (error != 0) // �ɹ�����0 ʧ�ܷ��ش������
		{
			std::cout << "���ļ�ʧ��:" << error << std::endl;
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
		// ����0�ļ�β��
		return feof(m_file) == 0;
	}

	bool File::read(size_t index, size_t size, ByteArray& buffer) const
	{
		// �ж��ļ�����Ƿ��
		if (this->m_file == NULL)
		{
			return false;
		}

		// �ƶ���дλ�õ��ļ�ͷ
#ifdef _WIN64
		if (_fseeki64(this->m_file, index, 0) != 0)
#else
		if (fseek(this->m_file, index, 0) != 0)
#endif
		{
			return false;
		}
		// ��ȡ1��size�Ĵ�С
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
		
		// д��1��size�Ĵ�С ʧ��
		//std::cout << "����д���С:" << buffer.size() << std::endl;
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
		// ��������С���Ͻ�β���ַ�
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
			// std::cout << "���ļ�ʧ��" << std::endl;
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
		// �ļ����  
		intptr_t handle = 0;

		// �ļ���Ϣ  
		struct _finddata_t fileinfo;

		std::string searchPattern = path;
		// ���·������\��β�������һ��\  
		if (searchPattern.back() != '\\') {
			searchPattern += "\\";
		}
		searchPattern += "*";

		if ((handle = _findfirst(searchPattern.c_str(), &fileinfo)) != -1) {
			do {
				// �����Ŀ¼��������Ҫ�ݹ飬��ݹ�֮  
				if ((fileinfo.attrib & _A_SUBDIR) != 0) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
						if (recursion) {
							std::string newPath = path;
							// ���·������\��β�������һ��\  
							if (newPath.back() != '\\') {
								newPath += "\\";
							}
							newPath += fileinfo.name;
							File::list(newPath, list, recursion);
						}
					}
				}
				else {
					// ������ļ�������ӵ��б���  
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

		// ����ļ���״̬
		if (file.good())
		{
			// �����β�ַ���
			//file << content << "\n";
			// �ر��ļ���
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
		// ��ʽ��Ϊnative��ʽб��·��
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
		// �����һ���㣨'.'����ʼ��������
		auto dot_pos = path.rfind('.');

		// ���û���ҵ��㣬��û�к�׺
		if (dot_pos == std::string::npos) {
			return "";
		}

		// ���򣬷��ص�֮��Ĳ�����Ϊ��׺
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

		// �����tempFile��һ�����ڵ���ʱ�ļ���·���������ֻ��Ҫ�ļ����������ʹ��PathFindFileName��������ȡ����  
		// ���ǣ���ע�⣬�������������һ����ʵ����ʱ�ļ��������������Ҫɾ������  
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
			//std::cout << "��Դ���ʧ��" << std::endl;
			return ByteArray();
		}
		DWORD size = SizeofResource(module, resHandle);
		if (!size)
		{
			//std::cout << "��Դ��Сʧ��" << std::endl;
			return ByteArray();
		}

		HGLOBAL res = LoadResource(module, resHandle);
		if (!res)
		{
			//std::cout << "��Դ����ʧ��" << std::endl;
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
		// ��std::stringת��ΪC�����ַ�����const char*��  
		const char* src_cstr = src.c_str();
		const char* dst_cstr = dst.c_str();

		// ����MoveFileEx���������ƶ��ļ�  
		// ��������������ָ���ƶ�ѡ���������ʹ��MOVEFILE_REPLACE_EXISTING���滻Ŀ��λ���Ѵ��ڵ��ļ�  
		if (MoveFileExA(src_cstr, dst_cstr, MOVEFILE_REPLACE_EXISTING) == TRUE) {
			return true; // �ƶ��ɹ�  
		}
		else {
			// ��ȡ���������Ĵ������  
			DWORD error = GetLastError();
			// ����ѡ���׳��쳣���¼��������򵥴�ӡ������Ϣ  
			std::cerr << "Failed to move file: " << src << " to " << dst << ". Error code: " << error << std::endl;
			return false; // �ƶ�ʧ��  
		}
	}
}


