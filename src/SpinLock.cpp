#include "SpinLock.h"

namespace Alastor
{
    SpinLock::SpinLock(): flag_(false)
    {

    }
    void SpinLock::lock()
    {
        {
            bool expect = false;
            while (!flag_.compare_exchange_weak(expect, true))
            {
                //这里一定要将expect复原，执行失败时expect结果是未定的
                expect = false;
            }
        }
    }

    void SpinLock::unlock()
    {
        flag_.store(false);
    }

}
