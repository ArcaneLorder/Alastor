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
		Ĭ��ѹ������
	*/
#define ZIP_DEFAULT_COMPRESSION_LEVEL 6


	/*
		�������
	*/
#define ZIP_ENOINIT -1      // δ��ʼ��
#define ZIP_EINVENTNAME -2  // ��Ч��������
#define ZIP_ENOENT -3       // δ�ҵ����
#define ZIP_EINVMODE -4     // ��Чѹ��ģʽ
#define ZIP_EINVLVL -5      // ��Чѹ������
#define ZIP_ENOSUP64 -6     // ��֧�� Zip64
#define ZIP_EMEMSET -7      // �ڴ��ʼ��ʧ��
#define ZIP_EWRTENT -8      // �޷�д������
#define ZIP_ETDEFLINIT -9   // �޷���ʼ�� tdefl ѹ����
#define ZIP_EINVIDX -10     // ��Ч����
#define ZIP_ENOHDR -11      // ͷ��δ�ҵ�
#define ZIP_ETDEFLBUF -12   // �޷�ˢ�� tdefl ����
#define ZIP_ECRTHDR -13     // �޷���������ͷ
#define ZIP_EWRTHDR -14     // �޷�д������ͷ
#define ZIP_EWRTDIR -15     // �޷�д������Ŀ¼
#define ZIP_EOPNFILE -16    // �޷����ļ�
#define ZIP_EINVENTTYPE -17 // ��Ч��������
#define ZIP_EMEMNOALLOC -18 // ��ʹ���ڴ������ȡ����
#define ZIP_ENOFILE -19     // �ļ�δ�ҵ�
#define ZIP_ENOPERM -20     // Ȩ�޲���
#define ZIP_EOOMEM -21      // �ڴ治��
#define ZIP_EINVZIPNAME -22 // ��Ч Zip �浵����
#define ZIP_EMKDIR -23      // ����Ŀ¼����
#define ZIP_ESYMLINK -24    // �������Ӵ���
#define ZIP_ECLSZIP -25     // �رմ浵����
#define ZIP_ECAPSIZE -26    // ����̫С
#define ZIP_EFSEEK -27      // �ļ�ָ���ƶ�����
#define ZIP_EFREAD -28      // �ļ���ȡ����
#define ZIP_EFWRITE -29     // �ļ�д�����

	// �����ݽṹ�������������б�ʾ zip �浵
	struct zip_t;

	/// <summary>
	/// ���Ҵ�������Ӧ�Ĵ�����Ϣ�ַ���
	/// </summary>
	/// <param name="errnum">�������</param>
	/// <returns>���������Ч���ش�����Ϣ�ַ���,��Ч�򷵻�NULL.</returns>
	extern ZIP_EXPORT const char* zip_strerror(int errnum);

	/// <summary>
	/// ʹ��ָ��ģʽ��ѹ������� Zip �浵
	/// </summary>
	/// <param name="zipname">Zip �浵�ļ���</param>
	/// <param name="level">ѹ������(0-9�Ǳ�׼��zlib��񼶱�)</param>
	/// <param name="mode">�ļ���ʽģʽ "-r":��һ���ļ����ж�ȡ(���ļ��������) "-w":����һ������д��Ŀ��ļ� "-a":׷�ӵ����д浵��</param>
	/// <returns>�ɹ����� Zip �浵��� ʧ�ܷ���NULL</returns>
	extern ZIP_EXPORT struct zip_t* zip_open(const char* zipname, int level, char mode);

	/// <summary>
	/// �ر� Zip �ĵ�
	/// </summary>
	/// <param name="zip">Zip �ĵ����</param>
	extern ZIP_EXPORT void zip_close(struct zip_t* zip);

	/// <summary>
	/// ȷ���浵�ļ�����Ŀ¼ͷ�Ƿ��� Zip64 ��β
	/// </summary>
	/// <param name="zip">Zip �ĵ����</param>
	/// <returns>����1(true)����� ����0(false)�򲻴���</returns>
	extern ZIP_EXPORT int zip_is64(struct zip_t* zip);


	/// <summary>
	/// ����Ŀ����
	/// ������ 'w' �� 'a' ģʽ�򿪵�zip�浵,��������������Ŀ.��ֻ��ģʽ��,����������ȫ���ֵ��ж�λ��Ŀ(���ִ�Сд).
	/// </summary>
	/// <param name="zip">Zip �ĵ����</param>
	/// <param name="entryname">��Ŀ����</param>
	/// <returns>�ɹ�����0 ʧ�ܷ��ظ���</returns>
	extern ZIP_EXPORT int zip_entry_open(struct zip_t* zip, const char* entryname);

	/// <summary>
	/// ��Zip�浵�а����ƴ���Ŀ
	/// ������ 'w' �� 'a' ģʽ�򿪵�zip�浵,��������������Ŀ.��ֻ��ģʽ��,����������ȫ���ֵ��ж�λ��Ŀ(���ִ�Сд).
	/// </summary>
	/// <param name="zip">Zip �ļ����</param>
	/// <param name="entryname">�����ֵ��е���Ŀ����</param>
	/// <returns>�ɹ�����0,ʧ�ܷ��ظ���</returns>
	extern ZIP_EXPORT int zip_entry_opencasesensitive(struct zip_t* zip, const char* entryname);

	/// <summary>
	/// �� Zip �浵�а�������һ������Ŀ
	/// </summary>
	/// <param name="zip">Zip �ļ����</param>
	/// <param name="index">�����ֵ��е�����</param>
	/// <returns>�ɹ�����0,ʧ�ܷ��ظ���.</returns>
	extern ZIP_EXPORT int zip_entry_openbyindex(struct zip_t* zip, size_t index);

	/// <summary>
	/// �ر� Zip ��Ŀ,ˢ�»��������ͷ���Դ.
	/// </summary>
	/// <param name="zip">Zip �ļ����</param>
	/// <returns>�ɹ�����0,ʧ�ܷ��ظ���.</returns>
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
	/// ���ص�ǰ Zip ��Ŀ�ı�������
	/// �û���Ŀ���ƺͱ�����Ŀ����֮�����Ҫ����
	/// </summary>
	/// <param name="zip">Zip �ļ����</param>
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
