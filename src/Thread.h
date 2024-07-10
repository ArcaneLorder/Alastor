#pragma once
#include "Alastor.h"
#include <functional>
namespace Alastor
{
    class  ALASTOR_API Thread
    {
    public:
        Thread();
        ~Thread();
        bool start();
        bool state();
        void wait();
        bool stop();
    private:
        virtual bool run() = 0;
    public:
        static void* __thread_handle(void* param);
    public:
        unsigned int m_state;
        bool m_thread;
    public:
        static bool create(std::function<void(void*)> func, void* param, bool wait = false);


    };
    
}
