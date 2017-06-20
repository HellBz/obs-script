// Copyright © Samantha James

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "ReadWriteMutex.h"

namespace Detail
{
    RWLockImpl::RWLockImpl()
    {
#if _WIN32
        InitializeSRWLock(&m_lock);
#endif
    }

    void RWLockImpl::LockShared()
    {
#if _WIN32
        AcquireSRWLockShared(&m_lock);
#else
        m_lock.lock();
#endif
    }

    void RWLockImpl::LockExclusive()
    {
#if _WIN32
        AcquireSRWLockExclusive(&m_lock);
#else
        m_lock.lock();
#endif
    }

    void RWLockImpl::ReleaseShared()
    {
#if _WIN32
        ReleaseSRWLockShared(&m_lock);
#else
        m_lock.unlock();
#endif
    }

    void RWLockImpl::ReleaseExclusive()
    {
#if _WIN32
        ReleaseSRWLockExclusive(&m_lock);
#else
        m_lock.unlock();
#endif
    }
}

namespace Script
{
    namespace Utils
    {
        ReadLock::ReadLock(ReadWriteMutex& mutex)
            : m_mutex(mutex)
        {
            m_mutex.LockShared();
        }

        ReadLock::~ReadLock() { m_mutex.ReleaseShared(); }

        WriteLock::WriteLock(ReadWriteMutex& mutex)
            : m_mutex(mutex)
        {
            m_mutex.LockExclusive();
        }

        WriteLock::~WriteLock() { m_mutex.ReleaseExclusive(); }
    }
}
