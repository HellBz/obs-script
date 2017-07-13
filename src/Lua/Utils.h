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

#include <string>
#include <xtr1common>

namespace Script
{
    namespace Lua
    {
        namespace Utils
        {
            template <typename T>
            struct Enabled : std::false_type
            {
            };

#define LUA_ENABLED(T)                 \
    template <>                        \
    struct Enabled<T> : std::true_type \
    {                                  \
    }

            LUA_ENABLED(uint8);
            LUA_ENABLED(int8);
            LUA_ENABLED(uint16);
            LUA_ENABLED(int16);
            LUA_ENABLED(uint32);
            LUA_ENABLED(int32);
            LUA_ENABLED(uint64);
            LUA_ENABLED(int64);
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

            namespace Details
            {
                template <bool fits>
                struct Helper64
                {
                    static void PutUnsigned(lua_State* L, const uint64 value)
                    {
                        lua_pushinteger(L, value);
                    }

                    static void PutSigned(lua_State* L, const int64 value)
                    {
                        lua_pushinteger(L, value);
                    }

                    static uint64 GetUnsigned(lua_State* L, const int32 index)
                    {
                        auto result = lua_tointeger(L, index);
                        return static_cast<uint64>(result);
                    }

                    static int64 GetSigned(lua_State* L, const int32 index)
                    {
                        auto result = lua_tointeger(L, index);
                        return static_cast<int64>(result);
                    }
                };

                template <>
                struct Helper64<false>
                {
                    static void PutUnsigned(lua_State* L, const uint64 value)
                    {
                        const lua_Number numValue = static_cast<lua_Number>(value);
                        lua_pushnumber(L, numValue);
                    }

                    static void PutSigned(lua_State* L, const int64 value)
                    {
                        const lua_Number numValue = static_cast<lua_Number>(value);
                        lua_pushnumber(L, numValue);
                    }

                    static uint64 GetUnsigned(lua_State* L, const int32 index)
                    {
                        auto result = lua_tonumber(L, index);
                        return static_cast<uint64>(result);
                    }

                    static int64 GetSigned(lua_State* L, const int32 index)
                    {
                        auto result = lua_tonumber(L, index);
                        return static_cast<int64>(result);
                    }
                };
            }

            template <typename T>
            struct Writer
            {
                static int32 Write(lua_State* L, T)
                {
                    static_assert(!sizeof(T), "unsupported type");
                    return 0;
                }
            };

            template <typename T>
            struct Reader
            {
                static T Read(lua_State* L, const int32)
                {
                    static_assert(!sizeof(T), "unsupported type");
                    return T();
                }
            };

            template <>
            struct Writer<uint8>
            {
                static int32 Write(lua_State* L, const uint8 value)
                {
                    lua_pushinteger(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<uint8>
            {
                static uint8 Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tointeger(L, index);
                    return static_cast<uint8>(result);
                }
            };

            template <>
            struct Writer<uint16>
            {
                static int32 Write(lua_State* L, const uint16 value)
                {
                    lua_pushinteger(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<uint16>
            {
                static uint16 Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tointeger(L, index);
                    return static_cast<uint16>(result);
                }
            };

            template <>
            struct Writer<uint32>
            {
                static int32 Write(lua_State* L, const uint32 value)
                {
                    lua_pushinteger(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<uint32>
            {
                static uint32 Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tointeger(L, index);
                    return static_cast<uint32>(result);
                }
            };

            template <>
            struct Writer<uint64>
            {
                static int32 Write(lua_State* L, const uint64 value)
                {
                    Details::Helper64<sizeof(lua_Integer) >= sizeof(uint64)>::PutUnsigned(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<uint64>
            {
                static uint64 Read(lua_State* L, const int32 index)
                {
                    return Details::Helper64<sizeof(lua_Integer) >= sizeof(uint64)>::GetUnsigned(
                        L, index);
                }
            };

            template <>
            struct Writer<int8>
            {
                static int32 Write(lua_State* L, const int8 value)
                {
                    lua_pushinteger(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<int8>
            {
                static int8 Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tointeger(L, index);
                    return static_cast<int8>(result);
                }
            };

            template <>
            struct Writer<int16>
            {
                static int32 Write(lua_State* L, const int16 value)
                {
                    lua_pushinteger(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<int16>
            {
                static int16 Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tointeger(L, index);
                    return static_cast<int16>(result);
                }
            };

            template <>
            struct Writer<int32>
            {
                static int32 Write(lua_State* L, const int32 value)
                {
                    lua_pushinteger(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<int32>
            {
                static int32 Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tointeger(L, index);
                    return static_cast<int32>(result);
                }
            };

            template <>
            struct Writer<int64>
            {
                static int32 Write(lua_State* L, const int64 value)
                {
                    Details::Helper64<sizeof(lua_Integer) >= sizeof(uint64)>::PutSigned(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<int64>
            {
                static int64 Read(lua_State* L, const int32 index)
                {
                    return Details::Helper64<sizeof(lua_Integer) >= sizeof(int64)>::GetUnsigned(
                        L, index);
                }
            };

            template <>
            struct Writer<bool>
            {
                static int32 Write(lua_State* L, const bool value)
                {
                    lua_pushboolean(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<bool>
            {
                static bool Read(lua_State* L, const int32 index)
                {
                    auto result = lua_toboolean(L, index);
                    return !!result;
                }
            };

            template <>
            struct Writer<float>
            {
                static int32 Write(lua_State* L, const float value)
                {
                    lua_pushnumber(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<float>
            {
                static float Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tonumber(L, index);
                    return static_cast<float>(result);
                }
            };

            template <>
            struct Writer<double>
            {
                static int32 Write(lua_State* L, const double value)
                {
                    lua_pushnumber(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<double>
            {
                static double Read(lua_State* L, const int32 index)
                {
                    auto result = lua_tonumber(L, index);
                    return static_cast<double>(result);
                }
            };

            template <>
            struct Writer<std::string>
            {
                static int32 Write(lua_State* L, const std::string& value)
                {
                    lua_pushstring(L, value.c_str());
                    return 1;
                }
            };

            template <>
            struct Reader<std::string>
            {
                static std::string Read(lua_State* L, const int32 index)
                {
                    std::string result = lua_tostring(L, index);
                    return result;
                }
            };

            template <>
            struct Writer<const char*>
            {
                static int32 Write(lua_State* L, const char* const value)
                {
                    lua_pushstring(L, value);
                    return 1;
                }
            };

            template <>
            struct Reader<const char*>
            {
                static const char* Read(lua_State* L, const int32 index)
                {
                    return lua_tostring(L, index);
                }
            };

            template <size_t size>
            struct Writer<const char[ size ]>
            {
                static int32 Write(lua_State* L, const char (&value)[ size ])
                {
                    lua_pushstring(L, value);
                    return 1;
                }
            };

            template <typename T>
            struct Writer<T*>
            {
                static int32 Write(lua_State* L, T* value)
                {
                    auto dest = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
                    *dest     = value;

                    luaL_getmetatable(L, ::Script::Utils::GetTypeName<T>());
                    lua_setmetatable(L, -2);

                    return 1;
                }
            };

            template <typename T>
            struct Reader<T*>
            {
                static T* Read(lua_State* L, const int32 index)
                {
                    T** result = static_cast<T**>(
                        luaL_checkudata(L, index, ::Script::Utils::GetTypeName<T>()));
                    if (result)
                        return *result;

                    return nullptr;
                }
            };
        }
    }
}