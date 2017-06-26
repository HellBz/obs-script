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

#include <lua.hpp>
#include <string>

namespace Script
{
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
            static void Register(lua_State* L, const char* Namespace = nullptr)
            {
                Reflection::ClassRegistry::Register<T>();

                const auto typeName = Utils::GetTypeName<T>();

                if (Namespace && strlen(Namespace))
                {
                    lua_getglobal(L, Namespace);
                    if (lua_isnil(L, -1))
                    {
                        lua_newtable(L);
                        lua_pushvalue(L, -1);
                        lua_setglobal(L, Namespace);
                    }

                    lua_pushcfunction(L, &Class<T>::New);
                    lua_setfield(L, -2, typeName);
                    lua_pop(L, 1);
                }
                else
                {
                    lua_pushcfunction(L, &Class<T>::New);
                    lua_setglobal(L, typeName);
                }

                luaL_newmetatable(L, typeName);
                const auto table = lua_gettop(L);

                lua_pushstring(L, "__gc");
                lua_pushcfunction(L, &Class<T>::Collect);
                lua_settable(L, table);

                lua_pushstring(L, "__tostring");
                lua_pushcfunction(L, &Class<T>::ToString);
                lua_settable(L, table);

                lua_pushstring(L, "__eq");
                lua_pushcfunction(L, &Class<T>::Equal);
                lua_settable(L, table);

                lua_pushstring(L, "__index");
                lua_pushcfunction(L, &Class<T>::Get);
                lua_settable(L, table);

                lua_pushstring(L, "__newindex");
                lua_pushcfunction(L, &Class<T>::Set);
                lua_settable(L, table);

                const auto& outline = Reflection::ClassRegistry::Find(typeName);
                for (const auto& iter : outline)
                {
                    lua_pushstring(L, iter.first.c_str());
                    lua_pushstring(L, iter.first.c_str());
                    lua_settable(L, table);
                }
            }

            static int32 New(lua_State* L)
            {
                auto userData = lua_newuserdata(L, sizeof(T));
                new (userData) T();

                luaL_getmetatable(L, Utils::GetTypeName<T>());
                lua_setmetatable(L, -2);

                return 1;
            }

            static int32 Collect(lua_State* L)
            {
                if (auto ptr = static_cast<const T*>(lua_touserdata(L, -1)))
                {
                    delete ptr;
                }

                return 0;
            }

            static int32 ToString(lua_State* L)
            {
                if (auto ptr = static_cast<const T*>(lua_touserdata(L, -1)))
                    lua_pushfstring(L, "%s (%p)", Utils::GetTypeName<T>(), ptr);
                else
                    lua_pushfstring(L, "%s (nil)", Utils::GetTypeName<T>());

                return 1;
            }

            static int32 Equal(lua_State* L)
            {
                auto lhs = static_cast<const T*>(lua_touserdata(L, -1));
                auto rhs = static_cast<const T*>(lua_touserdata(L, 1));

                lua_pushboolean(L, *lhs == *rhs);
                return 1;
            }

            static int32 Get(lua_State* L)
            {
                const auto className = Utils::GetTypeName<T>();

                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_rawget(L, -2);

                if (lua_isstring(L, -1))
                {
                    const auto functionName = lua_tostring(L, -1);

                    auto ptr = static_cast<T*>(lua_touserdata(L, 1));

                    if (!ptr)
                    {
                        luaL_error(L, "Error: failed to retrieve underlying pointer");
                        return 0;
                    }

                    lua_pushvalue(L, 3);

                    const auto& outline = Reflection::ClassRegistry::Find(className);
                    if (outline.HasFunction(functionName))
                    {
                        lua_pushstring(L, functionName);
                        lua_pushlightuserdata(L, static_cast<void*>(ptr));
                        lua_pushcclosure(L, &Class<T>::Invoke, 2);
                        return 1;
                    }
                    else
                    {
                        lua_pop(L, 2);
                        lua_remove(L, 1);
                        lua_remove(L, 1);

                        return 0;
                    }
                }

                return 1;
            }

            static int32 Set(lua_State* L)
            {
                const auto className = Utils::GetTypeName<T>();

                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_rawget(L, -2);

                if (lua_isstring(L, -1))
                {
                    const auto functionName = lua_tostring(L, -1);

                    auto ptr = static_cast<const T*>(lua_touserdata(L, 1));

                    if (!ptr)
                    {
                        luaL_error(L, "Error: failed to retrieve underlying pointer");
                        return 0;
                    }

                    const auto& outline = Reflection::ClassRegistry::Find(className);
                    if (outline.HasFunction(functionName))
                    {
                        luaL_error(L,
                                   "Error: trying to set method %s of type %s",
                                   functionName,
                                   className);

                        return 0;
                    }
                    else
                    {
                        lua_pop(L, 2);
                        lua_remove(L, 1);
                        lua_remove(L, 1);

                        return 0;
                    }
                }

                return 0;
            }

            static int32 Invoke(lua_State* L)
            {
                const auto className = Utils::GetTypeName<T>();

                auto functionName = lua_tostring(L, lua_upvalueindex(1));
                auto ptr          = static_cast<T*>(lua_touserdata(L, lua_upvalueindex(2)));

                const auto& outline = Reflection::ClassRegistry::Find(className);

                if (outline.HasFunction(functionName))
                {
                    std::shared_ptr<IFunction> method = outline.GetFunction(functionName);
                    if (method)
                    {
                        // return method->Invoke();
                    }
                }

                luaL_error(
                    L, "Error: unable to find method %s of type %s", functionName, className);
                return 0;
            }
        };
    }
}