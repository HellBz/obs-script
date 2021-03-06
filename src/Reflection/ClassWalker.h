﻿// Copyright © Samantha James

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

#include <memory>
#include <string>
#include <unordered_map>

#include "Reflection/Function.h"
#include "Utils/Misc.h"

namespace Script
{
    namespace Reflection
    {

        class ClassWalker
        {
        public:
            template <typename TReturn, typename TObject, typename... TArgs>
            void AddFunction(std::string&& name, TReturn (TObject::*func)(TArgs...))
            {
                auto& entry = m_functions[ name ];
                if (!entry)
                    entry = std::make_shared<MemberFunction<TReturn, TObject, TArgs...>>(func);
            }

            template <typename TReturn, typename TObject, typename... TArgs>
            void AddFunction(std::string&& name, TReturn (TObject::*func)(TArgs...) const)
            {
                auto& entry = m_functions[ name ];

                if (!entry)
                    entry = std::make_shared<CMemberFunction<TReturn, TObject, TArgs...>>(func);
            }

            template <typename TReturn, typename... TArgs>
            void AddFunction(std::string&& name, TReturn (*func)(TArgs...))
            {
                auto& entry = m_functions[ name ];

                if (!entry)
                    entry = std::make_shared<StaticFunction<TReturn, TArgs...>>(func);
            }

            bool HasFunction(const std::string& name) const
            {
                return m_functions.find(name) != m_functions.end();
            }

            const std::shared_ptr<Interface::Function>& GetFunction(const char* const name) const
            {
                const auto iter = m_functions.find(name);
                if (iter != m_functions.end())
                    return (*iter).second;

                return Utils::Misc::InvalidFunction();
            }

        private:
            std::unordered_map<std::string, std::shared_ptr<Interface::Function>> m_functions;

            friend auto begin(const ClassWalker& src) -> decltype(src.m_functions.begin())
            {
                return src.m_functions.begin();
            }

            friend auto begin(ClassWalker& src) -> decltype(src.m_functions.begin())
            {
                return src.m_functions.begin();
            }

            friend auto end(const ClassWalker& src) -> decltype(src.m_functions.end())
            {
                return src.m_functions.end();
            }

            friend auto end(ClassWalker& src) -> decltype(src.m_functions.end())
            {
                return src.m_functions.end();
            }
        };
    }
}