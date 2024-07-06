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
                //����һ��Ҫ��expect��ԭ��ִ��ʧ��ʱexpect�����δ����
                expect = false;
            }
        }
    }

    void SpinLock::unlock()
    {
        flag_.store(false);
    }

}
