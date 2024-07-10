#include "Thread.h"
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif
namespace Alastor
{



	void newobj_thread_thread(void* lpParam)
	{
		auto t = (Alastor::Thread*)lpParam;
		t->__thread_handle(lpParam);
	}

	Thread::Thread()
	{
		m_state = 2;
		m_thread = false;
	}

	Thread::~Thread()
	{

	}

	bool Thread::start()
	{
		if (this->m_state == 0)
		{
			return false;
		}
			
		else if (this->m_state == 1)
		{
			return false;
		}
			
		m_thread = true;
		
		Alastor::Thread::create(newobj_thread_thread, this, false);

		this->m_state = 0;
		return true;
	}

	bool Thread::state()
	{
		if (this->m_state == 2)
			return false;
		return true;
	}
	void Thread::wait()
	{
		while (m_thread == true)
		{
			Sleep(100);
		}
		this->m_state = 2;
		Sleep(100);
	}
	bool Thread::stop()
	{
		this->m_state = 1;
		return true;
	}
	void* Thread::__thread_handle(void* param)
	{
		Alastor::Thread* t = (Alastor::Thread*)param;
		t->m_thread = true;
		while (true)
		{
			if (t->m_state == 1)
			{
				t->m_state = 2;
				break;
			}
			else
			{
				if (t->run() == false)
				{
					t->m_state = 2;
					break;
				}
			}
		}
		t->m_thread = false;
		return NULL;
	}



	struct FrowardParameter
	{
		std::function<void(void*)> function;
		void* param;
	};

	DWORD WINAPI stdcallForward(LPVOID lpParam)
	{
		FrowardParameter* forward = (FrowardParameter*)lpParam;
		forward->function(forward->param);
		delete forward;
		return 0;
	}

	bool Thread::create(std::function<void(void*)> func, void* param, bool wait)
	{
#ifdef _WIN32
		FrowardParameter* forward = new FrowardParameter;
		forward->function = func;
		forward->param = param;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)stdcallForward, (LPVOID)forward, 0, 0);
		if (hThread == NULL)
		{
			delete forward;
			return false;
		}
		if (wait)
		{
			WaitForSingleObject(hThread, 0xFFFFFFFF);
		}
			
		CloseHandle(hThread);
		return true;
#endif
	}


}

