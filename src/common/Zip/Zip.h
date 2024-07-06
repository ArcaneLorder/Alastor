#pragma once
#ifndef ZIP_H
#define ZIP_H

#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#ifndef ZIP_SHARED
#define ZIP_EXPORT
#else
#ifdef _WIN32
#ifdef ZIP_BUILD_SHARED
#define ZIP_EXPORT __declspec(dllexport)
#else
#define ZIP_EXPORT __declspec(dllimport)
#endif
#else
#define ZIP_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
	/*
		默认压缩级别
	*/
#define ZIP_DEFAULT_COMPRESSION_LEVEL 6


	/*
		错误代码
	*/
#define ZIP_ENOINIT -1      // 未初始化
#define ZIP_EINVENTNAME -2  // 无效输入名称
#define ZIP_ENOENT -3       // 未找到输出
#define ZIP_EINVMODE -4     // 无效压缩模式
#define ZIP_EINVLVL -5      // 无效压缩级别
#define ZIP_ENOSUP64 -6     // 不支持 Zip64
#define ZIP_EMEMSET -7      // 内存初始化失败
#define ZIP_EWRTENT -8      // 无法写入数据
#define ZIP_ETDEFLINIT -9   // 无法初始化 tdefl 压缩器
#define ZIP_EINVIDX -10     // 无效索引
#define ZIP_ENOHDR -11      // 头部未找到
#define ZIP_ETDEFLBUF -12   // 无法刷新 tdefl 缓存
#define ZIP_ECRTHDR -13     // 无法创建输入头
#define ZIP_EWRTHDR -14     // 无法写入输入头
#define ZIP_EWRTDIR -15     // 无法写入中心目录
#define ZIP_EOPNFILE -16    // 无法打开文件
#define ZIP_EINVENTTYPE -17 // 无效输入类型
#define ZIP_EMEMNOALLOC -18 // 不使用内存分配提取数据
#define ZIP_ENOFILE -19     // 文件未找到
#define ZIP_ENOPERM -20     // 权限不足
#define ZIP_EOOMEM -21      // 内存不足
#define ZIP_EINVZIPNAME -22 // 无效 Zip 存档名称
#define ZIP_EMKDIR -23      // 创建目录错误
#define ZIP_ESYMLINK -24    // 符号链接错误
#define ZIP_ECLSZIP -25     // 关闭存档错误
#define ZIP_ECAPSIZE -26    // 容量太小
#define ZIP_EFSEEK -27      // 文件指针移动错误
#define ZIP_EFREAD -28      // 文件读取错误
#define ZIP_EFWRITE -29     // 文件写入错误

	// 此数据结构用于在整个库中表示 zip 存档
	struct zip_t;

	/// <summary>
	/// 查找错误代码对应的错误信息字符串
	/// </summary>
	/// <param name="errnum">错误代码</param>
	/// <returns>错误代码有效返回错误信息字符串,无效则返回NULL.</returns>
	extern ZIP_EXPORT const char* zip_strerror(int errnum);

	/// <summary>
	/// 使用指定模式和压缩级别打开 Zip 存档
	/// </summary>
	/// <param name="zipname">Zip 存档文件名</param>
	/// <param name="level">压缩级别(0-9是标准的zlib风格级别)</param>
	/// <param name="mode">文件方式模式 "-r":打开一个文件进行读取(该文件必须存在) "-w":创建一个用于写入的空文件 "-a":追加到现有存档中</param>
	/// <returns>成功返回 Zip 存档句柄 失败返回NULL</returns>
	extern ZIP_EXPORT struct zip_t* zip_open(const char* zipname, int level, char mode);

	/// <summary>
	/// 关闭 Zip 文档
	/// </summary>
	/// <param name="zip">Zip 文档句柄</param>
	extern ZIP_EXPORT void zip_close(struct zip_t* zip);

	/// <summary>
	/// 确定存档文件中心目录头是否有 Zip64 结尾
	/// </summary>
	/// <param name="zip">Zip 文档句柄</param>
	/// <returns>返回1(true)则存在 返回0(false)则不存在</returns>
	extern ZIP_EXPORT int zip_is64(struct zip_t* zip);


	/// <summary>
	/// 打开条目名称
	/// 对于以 'w' 或 'a' 模式打开的zip存档,函数将附加新条目.在只读模式下,函数尝试在全局字典中定位条目(区分大小写).
	/// </summary>
	/// <param name="zip">Zip 文档句柄</param>
	/// <param name="entryname">条目名称</param>
	/// <returns>成功返回0 失败返回负数</returns>
	extern ZIP_EXPORT int zip_entry_open(struct zip_t* zip, const char* entryname);

	/// <summary>
	/// 在Zip存档中按名称打开条目
	/// 对于以 'w' 或 'a' 模式打开的zip存档,函数将附加新条目.在只读模式下,函数尝试在全局字典中定位条目(区分大小写).
	/// </summary>
	/// <param name="zip">Zip 文件句柄</param>
	/// <param name="entryname">本地字典中的条目名称</param>
	/// <returns>成功返回0,失败返回负数</returns>
	extern ZIP_EXPORT int zip_entry_opencasesensitive(struct zip_t* zip, const char* entryname);

	/// <summary>
	/// 在 Zip 存档中按索引打开一个新条目
	/// </summary>
	/// <param name="zip">Zip 文件句柄</param>
	/// <param name="index">本地字典中的索引</param>
	/// <returns>成功返回0,失败返回负数.</returns>
	extern ZIP_EXPORT int zip_entry_openbyindex(struct zip_t* zip, size_t index);

	/// <summary>
	/// 关闭 Zip 条目,刷新缓冲区并释放资源.
	/// </summary>
	/// <param name="zip">Zip 文件句柄</param>
	/// <returns>成功返回0,失败返回负数.</returns>
	extern ZIP_EXPORT int zip_entry_close(struct zip_t* zip);

	/**
	 * Returns a local name of the current zip entry.
	 *
	 * The main difference between user's entry name and local entry name
	 * is optional relative path.
	 * Following .ZIP File Format Specification - the path stored MUST not contain
	 * a drive or device letter, or a leading slash.
	 * All slashes MUST be forward slashes '/' as opposed to backwards slashes '\'
	 * for compatibility with Amiga and UNIX file systems etc.
	 *
	 * @param zip: zip archive handler.
	 *
	 * @return the pointer to the current zip entry name, or NULL on error.
	 */

	/// <summary>
	/// 返回当前 Zip 条目的本地名称
	/// 用户条目名称和本地条目名称之间的主要区别
	/// </summary>
	/// <param name="zip">Zip 文件句柄</param>
	/// <returns></returns>
	extern ZIP_EXPORT const char* zip_entry_name(struct zip_t* zip);

	/**
	 * Returns an index of the current zip entry.
	 *
	 * @param zip zip archive handler.
	 *
	 * @return the index on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT size_t zip_entry_index(struct zip_t* zip);

	/**
	 * Determines if the current zip entry is a directory entry.
	 *
	 * @param zip zip archive handler.
	 *
	 * @return the return code - 1 (true), 0 (false), negative number (< 0) on
	 *         error.
	 */
	extern ZIP_EXPORT int zip_entry_isdir(struct zip_t* zip);

	/**
	 * Returns the uncompressed size of the current zip entry.
	 * Alias for zip_entry_uncomp_size (for backward compatibility).
	 *
	 * @param zip zip archive handler.
	 *
	 * @return the uncompressed size in bytes.
	 */
	extern ZIP_EXPORT unsigned long long zip_entry_size(struct zip_t* zip);

	/**
	 * Returns the uncompressed size of the current zip entry.
	 *
	 * @param zip zip archive handler.
	 *
	 * @return the uncompressed size in bytes.
	 */
	extern ZIP_EXPORT unsigned long long zip_entry_uncomp_size(struct zip_t* zip);

	/**
	 * Returns the compressed size of the current zip entry.
	 *
	 * @param zip zip archive handler.
	 *
	 * @return the compressed size in bytes.
	 */
	extern ZIP_EXPORT unsigned long long zip_entry_comp_size(struct zip_t* zip);

	/**
	 * Returns CRC-32 checksum of the current zip entry.
	 *
	 * @param zip zip archive handler.
	 *
	 * @return the CRC-32 checksum.
	 */
	extern ZIP_EXPORT unsigned int zip_entry_crc32(struct zip_t* zip);

	/**
	 * Compresses an input buffer for the current zip entry.
	 *
	 * @param zip zip archive handler.
	 * @param buf input buffer.
	 * @param bufsize input buffer size (in bytes).
	 *
	 * @return the return code - 0 on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT int zip_entry_write(struct zip_t* zip, const void* buf,
		size_t bufsize);

	/**
	 * Compresses a file for the current zip entry.
	 *
	 * @param zip zip archive handler.
	 * @param filename input file.
	 *
	 * @return the return code - 0 on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT int zip_entry_fwrite(struct zip_t* zip, const char* filename);

	/**
	 * Extracts the current zip entry into output buffer.
	 *
	 * The function allocates sufficient memory for a output buffer.
	 *
	 * @param zip zip archive handler.
	 * @param buf output buffer.
	 * @param bufsize output buffer size (in bytes).
	 *
	 * @note remember to release memory allocated for a output buffer.
	 *       for large entries, please take a look at zip_entry_extract function.
	 *
	 * @return the return code - the number of bytes actually read on success.
	 *         Otherwise a negative number (< 0) on error.
	 */
	extern ZIP_EXPORT size_t zip_entry_read(struct zip_t* zip, void** buf,
		size_t* bufsize);

	/**
	 * Extracts the current zip entry into a memory buffer using no memory
	 * allocation.
	 *
	 * @param zip zip archive handler.
	 * @param buf preallocated output buffer.
	 * @param bufsize output buffer size (in bytes).
	 *
	 * @note ensure supplied output buffer is large enough.
	 *       zip_entry_size function (returns uncompressed size for the current
	 *       entry) can be handy to estimate how big buffer is needed.
	 *       For large entries, please take a look at zip_entry_extract function.
	 *
	 * @return the return code - the number of bytes actually read on success.
	 *         Otherwise a negative number (< 0) on error (e.g. bufsize is not large
	 * enough).
	 */
	extern ZIP_EXPORT size_t zip_entry_noallocread(struct zip_t* zip, void* buf,
		size_t bufsize);

	/**
	 * Extracts the current zip entry into output file.
	 *
	 * @param zip zip archive handler.
	 * @param filename output file.
	 *
	 * @return the return code - 0 on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT int zip_entry_fread(struct zip_t* zip, const char* filename);

	/**
	 * Extracts the current zip entry using a callback function (on_extract).
	 *
	 * @param zip zip archive handler.
	 * @param on_extract callback function.
	 * @param arg opaque pointer (optional argument, which you can pass to the
	 *        on_extract callback)
	 *
	 * @return the return code - 0 on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT int
		zip_entry_extract(struct zip_t* zip,
			size_t(*on_extract)(void* arg, uint64_t offset,
				const void* data, size_t size),
			void* arg);

	/**
	 * Returns the number of all entries (files and directories) in the zip archive.
	 *
	 * @param zip zip archive handler.
	 *
	 * @return the return code - the number of entries on success, negative number
	 *         (< 0) on error.
	 */
	extern ZIP_EXPORT size_t zip_entries_total(struct zip_t* zip);

	/**
	 * Deletes zip archive entries.
	 *
	 * @param zip zip archive handler.
	 * @param entries array of zip archive entries to be deleted.
	 * @param len the number of entries to be deleted.
	 * @return the number of deleted entries, or negative number (< 0) on error.
	 */
	extern ZIP_EXPORT size_t zip_entries_delete(struct zip_t* zip,
		char* const entries[], size_t len);

	/**
	 * Extracts a zip archive Stream into directory.
	 *
	 * If on_extract is not NULL, the callback will be called after
	 * successfully extracted each zip entry.
	 * Returning a negative value from the callback will cause abort and return an
	 * error. The last argument (void *arg) is optional, which you can use to pass
	 * data to the on_extract callback.
	 *
	 * @param Stream zip archive Stream.
	 * @param size Stream size.
	 * @param dir output directory.
	 * @param on_extract on extract callback.
	 * @param arg opaque pointer.
	 *
	 * @return the return code - 0 on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT int
		zip_Stream_extract(const char* Stream, size_t size, const char* dir,
			int (*on_extract)(const char* filename, void* arg),
			void* arg);

	/**
	 * Opens zip archive Stream into memory.
	 *
	 * @param Stream zip archive Stream.
	 * @param size Stream size.
	 *
	 * @return the zip archive handler or NULL on error
	 */
	extern ZIP_EXPORT struct zip_t* zip_Stream_open(const char* Stream, size_t size,
		int level, char mode);

	/**
	 * Copy zip archive Stream output buffer.
	 *
	 * @param zip zip archive handler.
	 * @param buf output buffer. User should free buf.
	 * @param bufsize output buffer size (in bytes).
	 *
	 * @return copy size
	 */
	extern ZIP_EXPORT size_t zip_Stream_copy(struct zip_t* zip, void** buf,
		size_t* bufsize);

	/**
	 * Close zip archive releases resources.
	 *
	 * @param zip zip archive handler.
	 *
	 * @return
	 */
	extern ZIP_EXPORT void zip_Stream_close(struct zip_t* zip);

	/**
	 * Creates a new archive and puts files into a single zip archive.
	 *
	 * @param zipname zip archive file.
	 * @param filenames input files.
	 * @param len: number of input files.
	 *
	 * @return the return code - 0 on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT int zip_create(const char* zipname, const char* filenames[],
		size_t len);

	/**
	 * Extracts a zip archive file into directory.
	 *
	 * If on_extract_entry is not NULL, the callback will be called after
	 * successfully extracted each zip entry.
	 * Returning a negative value from the callback will cause abort and return an
	 * error. The last argument (void *arg) is optional, which you can use to pass
	 * data to the on_extract_entry callback.
	 *
	 * @param zipname zip archive file.
	 * @param dir output directory.
	 * @param on_extract_entry on extract callback.
	 * @param arg opaque pointer.
	 *
	 * @return the return code - 0 on success, negative number (< 0) on error.
	 */
	extern ZIP_EXPORT int zip_extract(const char* zipname, const char* dir,
		int (*on_extract_entry)(const char* filename,
			void* arg),
		void* arg);
	/** @} */
#ifdef __cplusplus
}
#endif

#endif
