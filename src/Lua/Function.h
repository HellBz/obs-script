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

#include <lua.hpp>

#include "Interface/Function.h"
#include "Lua/Utils.h"

#include <memory>
#include <string>
#include <type_traits>

namespace Script
{
    namespace Lua
    {
        template <typename T>
        using remove_cr = typename std::remove_const_t<std::remove_reference_t<T>>;

        template <typename TChild, typename TReturn, typename TObject, typename... TArgs>
        class Function : public Interface::Function
        {
            using ArgsType = std::tuple<TArgs...>;

        public:
            int32 Invoke(void* state, void* data) const override
            {
                auto L      = reinterpret_cast<lua_State*>(state);
                auto result = InternalCall(L, data, std::make_index_sequence<sizeof...(TArgs)>{});
                Utils::Writer<remove_cr<TReturn>>::Write(L, result);

                return 1;
            }

            const char* ToString() const override { return m_name.c_str(); }

        private:
            template <size_t... Index>
            TReturn InternalCall(lua_State* L, void* data, std::index_sequence<Index...>) const
            {
                (void)(L);

                auto self = reinterpret_cast<const TChild*>(this);
                return self->operator()(
                    static_cast<TObject*>(data),
                    Utils::Reader<remove_cr<std::tuple_element_t<Index, ArgsType>>>::Read(
                        L, Index)...);
            }

            std::string m_name;
        };

        template <typename TChild, typename TObject, typename... TArgs>
        class Function<TChild, void, TObject, TArgs...> : public Interface::Function
        {
            using ArgsType = std::tuple<TArgs...>;

        public:
            int32 Invoke(void* state, void* data) const override
            {
                auto L = reinterpret_cast<lua_State*>(state);
                InternalCall(L, data, std::make_index_sequence<sizeof...(TArgs)>{});

                return 0;
            }

            const char* ToString() const override { return m_name.c_str(); }

        private:
            template <size_t... Index>
            void InternalCall(lua_State* L, void* data, std::index_sequence<Index...>) const
            {
                (void)(L);

                auto self = reinterpret_cast<const TChild*>(this);
                self->operator()(
                    static_cast<TObject*>(data),
                    Utils::Reader<remove_cr<std::tuple_element_t<Index, ArgsType>>>::Read(
                        L, Index)...);
            }

            std::string m_name;
        };

        template <typename TChild, typename TResult, typename... TArgs>
        class Function<TChild, TResult, null, TArgs...> : public Interface::Function
        {
            using ArgsType = std::tuple<TArgs...>;

        public:
            int32 Invoke(void* state, void* data) const override
            {
                (void)(data);

                auto L      = reinterpret_cast<lua_State*>(state);
                auto result = InternalCall(L, std::make_index_sequence<sizeof...(TArgs)>{});
                Utils::Writer<remove_cr<TResult>>::Write(L, result);

                return 1;
            }

            const char* ToString() const override { return m_name.c_str(); }

        private:
            template <size_t... Index>
            TResult InternalCall(lua_State* L, std::index_sequence<Index...>) const
            {
                (void)(L);

                auto self = reinterpret_cast<const TChild*>(this);
                return self->operator()(
                    Utils::Reader<remove_cr<std::tuple_element_t<Index, ArgsType>>>::Read(
                        L, Index)...);
            }

            std::string m_name;
        };

        template <typename TChild, typename... TArgs>
        class Function<TChild, void, null, TArgs...> : public Interface::Function
        {
            using ArgsType = std::tuple<TArgs...>;

        public:
            int32 Invoke(void* state, void* data) const override
            {
                (void)(data);

                auto L = reinterpret_cast<lua_State*>(state);
                InternalCall(L, std::make_index_sequence<sizeof...(TArgs)>{});
                return 0;
            }

            const char* ToString() const override { return m_name.c_str(); }

        private:
            template <size_t... Index>
            void InternalCall(lua_State* L, std::index_sequence<Index...>) const
            {
                (void)(L);

                auto self = reinterpret_cast<const TChild*>(this);
                self->operator()(
                    Utils::Reader<remove_cr<std::tuple_element_t<Index, ArgsType>>>::Read(
                        L, Index)...);
            }

            std::string m_name;
        };
    }
}