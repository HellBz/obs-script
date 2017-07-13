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

#if HAS_LUA
#include "Lua\Function.h"
#else
#include "Interface\Function.h"
#endif

namespace Script
{
    namespace Reflection
    {
#if HAS_LUA
        template <typename... TArgs>
        using IFunction = Script::Lua::Function<TArgs...>;
#else
        template <typename... TArgs>
        class IFunction : public Interface::Function
        {
        public:
            int32 Invoke(void*, void*) const override
            {
                UNIMPLEMENTED();
                return 0;
            }

            const char* ToString() const override
            {
                UNIMPLEMENTED();
                return nullptr;
            }
        };
#endif

        template <typename TReturn, typename TObject, typename... TArgs>
        class MemberFunction : public IFunction<MemberFunction<TReturn, TObject, TArgs...>,
                                                TReturn,
                                                TObject,
                                                TArgs...>
        {
            using TFunction = TReturn (TObject::*)(TArgs...);

        public:
            MemberFunction(TFunction func)
                : m_funcPtr(func)
            {
            }

            TReturn operator()(TObject* object, TArgs&&... args) const
            {
                return (object->*m_funcPtr)(std::forward<TArgs>(args)...);
            }

        private:
            TFunction m_funcPtr;
        };

        template <typename TReturn, typename TObject, typename... TArgs>
        class CMemberFunction : public IFunction<CMemberFunction<TReturn, TObject, TArgs...>,
                                                 TReturn,
                                                 TObject,
                                                 TArgs...>
        {
            using TFunction = TReturn (TObject::*)(TArgs...) const;

        public:
            CMemberFunction(TFunction func)
                : m_funcPtr(func)
            {
            }

            TReturn operator()(const TObject* object, TArgs&&... args) const
            {
                return (object->*m_funcPtr)(std::forward<TArgs>(args)...);
            }

        private:
            TFunction m_funcPtr;
        };

        template <typename TReturn, typename... TArgs>
        class StaticFunction
            : public IFunction<StaticFunction<TReturn, TArgs...>, TReturn, null, TArgs...>
        {
            using TFunction = TReturn (*)(TArgs...);

        public:
            StaticFunction(TFunction func)
                : m_funcPtr(func)
            {
            }

            TReturn operator()(TArgs&&... args) const
            {
                return (*m_funcPtr)(std::forward<TArgs>(args)...);
            }

        private:
            TFunction m_funcPtr;
        };
    }
}