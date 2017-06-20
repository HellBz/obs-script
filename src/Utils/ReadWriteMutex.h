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

#pragma once

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <mutex>
#endif

namespace Detail
{
    class RWLockImpl
    {
    public:
        RWLockImpl();

        RWLockImpl(const RWLockImpl&) = delete;
        RWLockImpl& operator=(const RWLockImpl&) = delete;

        void LockShared();
        void LockExclusive();
        void ReleaseShared();
        void ReleaseExclusive();

    private:
#if _WIN32
        SRWLOCK m_lock;
#else
        std::mutex m_lock;
#endif
    };
}

namespace Script
{
    namespace Utils
    {
        using ReadWriteMutex = ::Detail::RWLockImpl;

        class ReadLock
        {
        public:
            ReadLock(ReadWriteMutex& mutex);
            ~ReadLock();

        private:
            ReadWriteMutex& m_mutex;
        };

        class WriteLock
        {
        public:
            WriteLock(ReadWriteMutex& mutex);
            ~WriteLock();

        private:
            ReadWriteMutex& m_mutex;
        };
    }
}
