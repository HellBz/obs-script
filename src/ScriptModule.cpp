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

#include "types.h"

#include <cstdlib>
#include <ctime>

#include "ScriptModule.h"

#include "Lua/Context.h"

#include "Reflection/ClassRegistry.h"

#include "Exposed/Source.h"
#include "Exposed/Text.h"

namespace Script
{
    bool Module::OnLoad()
    {
        const auto time = std::time(0);
        std::srand(static_cast<uint32>(time));
        return true;
    }

    void Module::OnUnload() { m_manager.Finalize(); }

    void Module::OnPostLoad()
    {
        m_manager.SetContext(std::make_shared<Lua::Context>());
        m_manager.Initialize();
        m_manager.AddScript("X:\\obs-studio\\plugins\\obs-script\\test\\test.lua");
    }

    /*static*/ Module& Module::Get()
    {
        static Module Instance;

        return Instance;
    }
}