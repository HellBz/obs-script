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

#include "Utils/TypeNames.h"

#include <string>
#include <xtr1common>

namespace Details
{
    template <bool fits>
    struct Helper64
    {
        static void PutUnsigned(lua_State* L, const Script::uint64 value)
        {
            lua_pushinteger(L, value);
        }

        static void PutSigned(lua_State* L, const Script::int64 value)
        {
            lua_pushinteger(L, value);
        }

        static Script::uint64 GetUnsigned(lua_State* L, const Script::int32 index)
        {
            auto result = lua_tointeger(L, index);
            return static_cast<Script::uint64>(result);
        }

        static Script::int64 GetSigned(lua_State* L, const Script::int32 index)
        {
            auto result = lua_tointeger(L, index);
            return static_cast<Script::int64>(result);
        }
    };

    template <>
    struct Helper64<false>
    {
        static void PutUnsigned(lua_State* L, const Script::uint64 value)
        {
            const lua_Number numValue = static_cast<lua_Number>(value);
            lua_pushnumber(L, numValue);
        }

        static void PutSigned(lua_State* L, const Script::int64 value)
        {
            const lua_Number numValue = static_cast<lua_Number>(value);
            lua_pushnumber(L, numValue);
        }

        static Script::uint64 GetUnsigned(lua_State* L, const Script::int32 index)
        {
            auto result = lua_tonumber(L, index);
            return static_cast<Script::uint64>(result);
        }

        static Script::int64 GetSigned(lua_State* L, const Script::int32 index)
        {
            auto result = lua_tonumber(L, index);
            return static_cast<Script::int64>(result);
        }
    };

    template <typename T>
    struct Enabled : std::false_type
    {
    };

#define LUA_ENABLED(T)                 \
    template <>                        \
    struct Enabled<T> : std::true_type \
    {                                  \
    }

    LUA_ENABLED(Script::uint8);
    LUA_ENABLED(Script::int8);
    LUA_ENABLED(Script::uint16);
    LUA_ENABLED(Script::int16);
    LUA_ENABLED(Script::uint32);
    LUA_ENABLED(Script::int32);
    LUA_ENABLED(Script::uint64);
    LUA_ENABLED(Script::int64);
    LUA_ENABLED(bool);
    LUA_ENABLED(float);
    LUA_ENABLED(double);
    LUA_ENABLED(std::string);
    LUA_ENABLED(const char*);

    template <typename T>
    struct Enabled<T*> : std::true_type
    {
    };

    template <size_t size>
    struct Enabled<const char[ size ]> : std::true_type
    {
    };

#undef LUA_ENABLED
}

namespace Script
{
    namespace CoreUtils = Utils;

    namespace Lua
    {
        namespace Utils
        {
            template <typename T>
            constexpr bool IsSupportedType = ::Details::Enabled<T>::value;

            template <typename T>
            static int32 Write(lua_State* L, const T);

            template <typename T>
            static void Read(lua_State* L, const int32, T&);

            template <typename T>
            T Get(lua_State* L, const int32 index)
            {
                T result;
                Read(L, index, result);
                return result;
            }

            template <typename T>
            inline int32 Write(lua_State* L, const T)
            {
                static_assert(!sizeof(T), "unsupported type");
                return 1;
            }

            template <>
            inline int32 Write<uint8>(lua_State* L, const uint8 value)
            {
                lua_pushinteger(L, value);
                return 1;
            }

            template <>
            inline int32 Write<int8>(lua_State* L, const int8 value)
            {
                lua_pushinteger(L, value);
                return 1;
            }

            template <>
            inline int32 Write<uint16>(lua_State* L, const uint16 value)
            {
                lua_pushinteger(L, value);
                return 1;
            }

            template <>
            inline int32 Write<int16>(lua_State* L, const int16 value)
            {
                lua_pushinteger(L, value);
                return 1;
            }

            template <>
            inline int32 Write<uint32>(lua_State* L, const uint32 value)
            {
                lua_pushinteger(L, value);
                return 1;
            }

            template <>
            inline int32 Write<int32>(lua_State* L, const int32 value)
            {
                lua_pushinteger(L, value);
                return 1;
            }

            template <typename T>
            inline void Read(lua_State* L, const int32 index, T& result)
            {
                static_assert(!sizeof(T), "unsupported type");
            }

            template <>
            inline void Read<uint8>(lua_State* L, const int32 index, uint8& result)
            {
                assert(lua_isinteger(L, index));
                result = static_cast<uint8>(lua_tointeger(L, index));
            }

            template <>
            inline void Read<int8>(lua_State* L, const int32 index, int8& result)
            {
                assert(lua_isinteger(L, index));
                result = static_cast<int8>(lua_tointeger(L, index));
            }

            template <>
            inline void Read<uint16>(lua_State* L, const int32 index, uint16& result)
            {
                assert(lua_isinteger(L, index));
                result = static_cast<uint16>(lua_tointeger(L, index));
            }

            template <>
            inline void Read<int16>(lua_State* L, const int32 index, int16& result)
            {
                assert(lua_isinteger(L, index));
                result = static_cast<int16>(lua_tointeger(L, index));
            }

            template <>
            inline void Read<uint32>(lua_State* L, const int32 index, uint32& result)
            {
                assert(lua_isinteger(L, index));
                result = static_cast<uint32>(lua_tointeger(L, index));
            }

            template <>
            inline void Read<int32>(lua_State* L, const int32 index, int32& result)
            {
                assert(lua_isinteger(L, index));
                result = static_cast<int32>(lua_tointeger(L, index));
            }

            template <>
            inline int32 Write<uint64>(lua_State* L, const uint64 value)
            {
                constexpr bool fits = sizeof(lua_Integer) >= sizeof(uint64);
                ::Details::Helper64<fits>::PutSigned(L, value);
                return 1;
            }

            template <>
            inline void Read<uint64>(lua_State* L, const int32 index, uint64& result)
            {
                constexpr bool fits = sizeof(lua_Integer) >= sizeof(uint64);
                result              = ::Details::Helper64<fits>::GetUnsigned(L, index);
            }

            template <>
            inline int32 Write<int64>(lua_State* L, const int64 value)
            {
                constexpr bool fits = sizeof(lua_Integer) >= sizeof(int64);
                ::Details::Helper64<fits>::PutSigned(L, value);
                return 1;
            }

            template <>
            inline void Read<int64>(lua_State* L, const int32 index, int64& result)
            {
                constexpr bool fits = sizeof(lua_Integer) >= sizeof(int64);
                result              = ::Details::Helper64<fits>::GetUnsigned(L, index);
            }

            template <>
            inline int32 Write<bool>(lua_State* L, const bool value)
            {
                lua_pushboolean(L, value);
                return 1;
            }

            template <>
            inline void Read<bool>(lua_State* L, const int32 index, bool& result)
            {
                assert(lua_isboolean(L, index));
                result = !!lua_toboolean(L, index);
            }

            template <>
            inline int32 Write<float>(lua_State* L, const float value)
            {
                lua_pushnumber(L, value);
                return 1;
            }

            template <>
            inline void Read<float>(lua_State* L, const int32 index, float& result)
            {
                assert(lua_isnumber(L, index));

                result = static_cast<float>(lua_tonumber(L, index));
            }

            template <>
            inline int32 Write<double>(lua_State* L, const double value)
            {
                lua_pushnumber(L, value);
                return 1;
            }

            template <>
            inline void Read<double>(lua_State* L, const int32 index, double& result)
            {
                assert(lua_isnumber(L, index));

                result = static_cast<double>(lua_tonumber(L, index));
            }

            template <>
            inline int32 Write<std::string>(lua_State* L, const std::string value)
            {
                lua_pushstring(L, value.c_str());
                return 1;
            }

            template <>
            inline int32 Write<const char*>(lua_State* L, const char* const value)
            {
                lua_pushstring(L, value);
                return 1;
            }

            template <>
            inline void Read<const char*>(lua_State* L, const int32 index, const char*& result)
            {
                assert(lua_isstring(L, index));
                result = lua_tostring(L, index);
            }

            template <>
            inline void Read<std::string>(lua_State* L, const int32 index, std::string& result)
            {
                assert(lua_isstring(L, index));
                result = lua_tostring(L, index);
            }
        }
    }
}