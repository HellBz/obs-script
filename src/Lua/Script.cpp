// Copyright � Samantha James

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the �Software�), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <lua.hpp>

#include "types.h"

#include "Lua/Script.h"

#include "Lua/Context.h"

namespace Script
{
    namespace Lua
    {
        void Script::SetFile(const std::string& file) { m_filePath = file; }

        bool Script::Load(const std::shared_ptr<Interface::Context>& context) const
        {
            if (!context)
                return false;

            if (m_filePath.empty() || m_filePath.length() == 0)
                return false;

            const std::shared_ptr<Context>& luaContext = std::static_pointer_cast<Context>(context);

            auto result = luaL_loadfile(luaContext->GetState(), m_filePath.c_str());
            if (result != LUA_OK)
            {
                luaL_error(
                    luaContext->GetState(), "Error: failed to load file %s", m_filePath.c_str());
                return false;
            }

            result = lua_pcall(luaContext->GetState(), 0, LUA_MULTRET, 0);
            if (result != LUA_OK)
                luaL_error(
                    luaContext->GetState(), "Error: failed to execute %s", m_filePath.c_str());

            return result == LUA_OK;
        }
    }
}