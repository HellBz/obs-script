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

#include <string.h>

namespace Script
{
    namespace Detail
    {
        constexpr size_t GetPrefixSize()
        {
            return sizeof("Script::Detail::GetTypeNameHelper<") - 1u;
        }

        constexpr size_t GetPostfixSize() { return sizeof(">::GetTypeName") - 1u; }

        template <typename T>
        struct GetTypeNameHelper
        {
            static const char* GetTypeName()
            {
                static constexpr size_t size =
                    sizeof(__FUNCTION__) - GetPrefixSize() - GetPostfixSize();
                static char typeName[ size ] = {};
                memcpy(typeName, __FUNCTION__ + GetPrefixSize(), size - 1u);
                return typeName;
            }
        };
    }

    namespace Utils
    {
        const char* CleanTypeName(const char* const toClean);

        template <typename T>
        const char* GetTypeName()
        {
            return CleanTypeName(Detail::GetTypeNameHelper<T>::GetTypeName());
        }
    }
}