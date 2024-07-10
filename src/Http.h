#pragma once
#include "Common.h"
#include "String.h"
#include <map>
#include <mutex>
#include <functional> // C++11 or higher

namespace Alastor
{
	
	class ALASTOR_API HttpClient
	{
		using progress = std::function<bool(uint64_t current, uint64_t total)>;
	public:
		explicit HttpClient();
	public:
		void setKeepAlive(bool keepAlive);
		void setRequestHeader(std::string param, std::string value);
		void setRequestHeaders(std::multimap<std::string, std::string> headers);
	public:
		bool get(std::string url,std::string path, progress callback = nullptr);
		bool post(std::string url, std::string path,std::string body, std::string contentType);
	public:
		std::multimap<std::string, std::string> responseHeaders();
		std::string body();
		std::string error();
	private:
		bool m_keepAlive;
		std::multimap<std::string, std::string> m_requestHeaders;
		std::multimap<std::string, std::string> m_responseHeaders;
		std::string m_error;
		std::string m_body;
		std::mutex m_mutex;
	};
}

