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

#include <lua.hpp>
#include <util/base.h>

#include "types.h"

#include "Lua/Class.h"
#include "Lua/Context.h"
#include "Lua/Script.h"

#include "Exposed/Source.h"
#include "Exposed/Text.h"

namespace Script
{
    namespace Lua
    {
        static int32 Print(lua_State* L)
        {
            const auto argCount = lua_gettop(L);

            for (auto i = 0; i < argCount; ++i)
            {
                if (lua_isstring(L, i))
                {
                    const auto str = lua_tostring(L, i);
                    blog(LOG_DEBUG, "[Script]: %s", str);
                }
                else
                {
                }
            }

            return 0;
        }

        static const luaL_Reg skPrintOverride[] = {{"print", Print}, {nullptr, nullptr}};

        static void OverridePrint(lua_State* L)
        {
            lua_getglobal(L, "_G");
            luaL_setfuncs(L, skPrintOverride, 0);
            lua_pop(L, 1);
        }

        bool Context::Open()
        {
            m_state = luaL_newstate();

            return true;
        }

        void Context::Init()
        {
            luaL_openlibs(m_state);
            OverridePrint(m_state);

            Class<Source>::Register(m_state);
            Class<TextSource>::Register(m_state);
        }

        void Context::Close() { lua_close(m_state); }

        lua_State* Context::GetState() const { return m_state; }

        std::shared_ptr<Interface::Script> Context::NewScript(const std::string& file)
        {
            auto result = std::make_shared<Lua::Script>();
            result->SetFile(file);

            return result;
        }
    }
}