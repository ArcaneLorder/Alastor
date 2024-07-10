#pragma once
#include "Alastor.h"
#include <atomic>

namespace Alastor
{
    class ALASTOR_API SpinLock {

    public:
        SpinLock();
        void lock();
        void unlock();
    private:
        std::atomic<bool> flag_;
    };
}
