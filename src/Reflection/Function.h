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

namespace Script
{
    namespace Reflection
    {
        class IFunction
        {
        public:
        };

        template <typename TReturn, typename TObject, typename... TArgs>
        class MemberFunction : public IFunction
        {
            using TFunction = TReturn (TObject::*)(TArgs...);

        public:
            MemberFunction(TFunction func)
                : m_funcPtr(func)
            {
            }

            TReturn Invoke(TObject* object, TArgs&&... args) const
            {
                return (object->m_funcPtr)(std::forward<TArgs>(args)...);
            }

        private:
            TFunction m_funcPtr;
        };

        template <typename TReturn, typename TObject, typename... TArgs>
        class CMemberFunction : public IFunction
        {
            using TFunction = TReturn (TObject::*)(TArgs...) const;

        public:
            CMemberFunction(TFunction func)
                : m_funcPtr(func)
            {
            }

            TReturn Invoke(const TObject* object, TArgs&&... args) const
            {
                return (object->m_funcPtr)(std::forward<TArgs>(args)...);
            }

        private:
            TFunction m_funcPtr;
        };

        template <typename TReturn, typename... TArgs>
        class StaticFunction : public IFunction
        {
            using TFunction = TReturn (*)(TArgs...);

        public:
            StaticFunction(TFunction func)
                : m_funcPtr(func)
            {
            }

            TReturn Invoke(TArgs&&... args) const
            {
                return (*m_funcPtr)(std::forward<TArgs>(args)...);
            }

        private:
            TFunction m_funcPtr;
        };
    }
}