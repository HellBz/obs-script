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

#include "types.h"

#include "RefCounted.h"
#include "Reflection/ClassRegistry.h"

using namespace Script;
using namespace Script::Reflection;

typedef struct obs_source obs_source_t;
typedef struct obs_data obs_data_t;

class Source
{
public:
    Source();
    ~Source();

    const char* GetId() const;
    const char* GetName() const;

    uint32 GetWidth() const;
    uint32 GetHeight() const;

    void Show();
    void Hide();

    bool IsActive() const;
    bool IsShowing() const;

    void Mute();
    void Unmute();
    bool IsMuted() const;

    bool operator==(const Source& other) const;

protected:
    void _CreateSource(const char* id, const char* name);
    obs_source_t* m_source;
    obs_data_t* m_data;
};

template <>
struct RegisterClass<Source>
{
    static void Register(ClassWalker& walker)
    {
        walker.AddFunction("GetId", &Source::GetId);
        walker.AddFunction("GetName", &Source::GetName);
        walker.AddFunction("GetWidth", &Source::GetWidth);
        walker.AddFunction("GetHeight", &Source::GetHeight);
        walker.AddFunction("Show", &Source::Show);
        walker.AddFunction("Hide", &Source::Hide);
        walker.AddFunction("IsActive", &Source::IsActive);
        walker.AddFunction("IsShowing", &Source::IsShowing);
        walker.AddFunction("Mute", &Source::Mute);
        walker.AddFunction("Unmute", &Source::Unmute);
        walker.AddFunction("IsMuted", &Source::IsMuted);
    }
};