// Copyright � Samantha James

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the �Software�), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <atomic>

namespace Script
{
    class RefCounted
    {
    public:
        virtual void AddRef()
        {
            std::atomic_fetch_add_explicit(&m_refCount, 1u, std::memory_order_relaxed);
        }

        virtual void ReleaseRef()
        {
            if (std::atomic_fetch_sub_explicit(&m_refCount, 1u, std::memory_order_release) == 1)
            {
                std::atomic_thread_fence(std::memory_order_acquire);
                DeleteSelf();
            }
        }

        uint32 RefCount() const { return m_refCount.load(); }

    protected:
        virtual void DeleteSelf() = 0;

        std::atomic<uint32> m_refCount;
    };
}