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

#include "Interface/Class.h"
#include "Utils/TypeNames.h"

#include <lua.hpp>
#include <string>

namespace Script
{
    namespace CoreUtils = Utils;

    namespace Reflection
    {
        class ClassWalker;
    }

    namespace Lua
    {
        template <typename T>
        class Class : public Interface::Class
        {
        public:
            /**
             * Retrieves this class from the Lua stack
             *
             * @param L lua state
             * @param arg position to check
             */
            static T* Check(lua_State* L, int32 arg)
            {
                auto ptr = static_cast<T**>(luaL_checkudata(L, arg, CoreUtils::GetTypeName<T>()));
                if (!ptr)
                    return nullptr;

                return *ptr;
            }

            /**
             * Retrieves this class from the Lua stack
             *
             * @param L lua state
             * @param arg position to check
             */
            static T* LightCheck(lua_State* L, int32 arg)
            {
                auto ptr = static_cast<T**>(luaL_testudata(L, arg, CoreUtils::GetTypeName<T>()));
                if (!ptr)
                    return nullptr;

                return *ptr;
            }

            static void Register(lua_State* L, const char* nameSpace = nullptr)
            {
                Reflection::ClassRegistry::Register<T>();

                if (nameSpace && strlen(nameSpace))
                {
                    lua_getglobal(L, nameSpace);
                    if (lua_isnil(L, -1))
                    {
                        lua_newtable(L);
                        lua_pushvalue(L, -1);
                        lua_setglobal(L, nameSpace);
                    }

                    lua_pushcfunction(L, &Class<T>::New);
                    lua_setfield(L, -2, CoreUtils::GetTypeName<T>());
                    lua_pop(L, 1);
                }
                else
                {
                    lua_pushcfunction(L, &Class<T>::New);
                    lua_setglobal(L, CoreUtils::GetTypeName<T>());
                }

                const auto typeName = CoreUtils::GetTypeName<T>();
                luaL_newmetatable(L, typeName);
                int metatable = lua_gettop(L);

                Utils::Write(L, "__gc");
                lua_pushcfunction(L, &Class<T>::Collect);
                lua_settable(L, metatable);

                Utils::Write(L, "__tostring");
                lua_pushcfunction(L, &Class<T>::ToString);
                lua_settable(L, metatable);

                Utils::Write(L, "__eq");
                lua_pushcfunction(L, &Class<T>::Equal);
                lua_settable(L, metatable);

                Utils::Write(L, "__index");
                lua_pushcfunction(L, &Class<T>::Get);
                lua_settable(L, metatable);

                Utils::Write(L, "__newindex");
                lua_pushcfunction(L, &Class<T>::Set);
                lua_settable(L, metatable);

                const auto& outline = Reflection::ClassRegistry::Find<T>();
                for (const auto& iter : outline)
                {
                    Utils::Write(L, iter.first);
                    Utils::Write(L, iter.first);
                    lua_settable(L, metatable);
                }
            }

            static int32 New(lua_State* L)
            {
                auto ptr  = new T();
                auto dest = static_cast<T**>(lua_newuserdata(L, sizeof(T)));
                *dest     = ptr;

                luaL_getmetatable(L, CoreUtils::GetTypeName<T>());
                lua_setmetatable(L, -2);

                return 1;
            }

            static int32 Collect(lua_State* L)
            {
                if (auto ptr = Class<T>::Check(L, -1))
                {
                    delete ptr;
                }

                return 0;
            }

            static int32 ToString(lua_State* L)
            {
                if (auto ptr = Class<T>::LightCheck(L, -1))
                    lua_pushfstring(L, "%s (%p)", CoreUtils::GetTypeName<T>(), ptr);
                else
                    lua_pushfstring(L, "%s (nil)", CoreUtils::GetTypeName<T>());

                return 1;
            }

            static int32 Equal(lua_State* L)
            {
                auto lhs = Class<T>::LightCheck(L, -1);
                auto rhs = Class<T>::LightCheck(L, 1);

                lua_pushboolean(L, *lhs == *rhs);
                return 1;
            }

            static int32 Get(lua_State* L)
            {
                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_rawget(L, -2);

                if (lua_isstring(L, -1))
                {
                    std::string functionName;
                    Utils::Read(L, -1, functionName);

                    auto ptr = static_cast<T**>(lua_touserdata(L, 1));

                    lua_pushvalue(L, 3);

                    const auto& outline = Reflection::ClassRegistry::Find<T>();
                    if (outline.HasFunction(functionName))
                    {
                        Utils::Write(L, functionName);
                        lua_pushlightuserdata(L, ptr);
                        lua_pushcclosure(L, &Class<T>::Invoke, 2);
                        return 1;
                    }

                    lua_pop(L, 2);
                    lua_remove(L, 1);
                    lua_remove(L, 1);

                    return 0;
                }

                return 1;
            }

            static int32 Set(lua_State* L)
            {
                const auto className = CoreUtils::GetTypeName<T>();

                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_rawget(L, -2);

                if (lua_isstring(L, -1))
                {
                    std::string functionName;
                    Utils::Read(L, -1, functionName);

                    auto ptr = static_cast<T**>(lua_touserdata(L, 1));

                    if (!ptr)
                    {
                        luaL_error(L, "Error: failed to retrieve underlying pointer");
                        return 0;
                    }

                    const auto& outline = Reflection::ClassRegistry::Find<T>();
                    if (outline.HasFunction(functionName))
                    {
                        luaL_error(L,
                                   "Error: trying to set method %s of type %s",
                                   functionName,
                                   className);

                        return 0;
                    }

                    lua_pop(L, 2);
                    lua_remove(L, 1);
                    lua_remove(L, 1);

                    return 0;
                }

                return 0;
            }

            static int32 Invoke(lua_State* L)
            {
                auto functionName = lua_tostring(L, lua_upvalueindex(1));
                auto ptr          = static_cast<T**>(lua_touserdata(L, lua_upvalueindex(2)));

                const auto& outline = Reflection::ClassRegistry::Find<T>();
                if (const auto& method = outline.GetFunction(functionName))
                    return method->Invoke(L, *ptr);

                luaL_error(L,
                           "Error: unable to find method %s of type %s",
                           functionName,
                           CoreUtils::GetTypeName<T>());
                return 0;
            }
        };
    }
}