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

#include "Source.h"

extern "C" {
#include <obs.h>
}

/*static*/ Source* Source::New(const char* id, const char* name, obs_data_t* settings,
                               obs_data_t* hotkeys)
{
    auto obsSource = obs_source_create(id, name, settings, hotkeys);

    auto result = new Source(obsSource);
    // TODO: add ref?
    return result;
}

Source::Source()
    : m_source(nullptr)
{
}

Source::Source(obs_source_t* source)
    : m_source(source)
{
}

Source::~Source()
{
    m_source = nullptr;
}

void Source::AddRef()
{
    obs_source_addref(m_source);
}

void Source::ReleaseRef()
{
    obs_source_release(m_source);
}

const char* Source::GetId() const
{
    if (!m_source)
        return nullptr;

    return obs_source_get_id(m_source);
}

const char* Source::GetName() const
{
    if (!m_source)
        return nullptr;

    return obs_source_get_name(m_source);
}

uint32 Source::GetWidth() const
{
    if (!m_source)
        return 0;

    return obs_source_get_width(m_source);
}

uint32 Source::GetHeight() const
{
    if (!m_source)
        return 0;

    return obs_source_get_height(m_source);
}

void Source::Show()
{
    UNIMPLEMENTED();
}

void Source::Hide()
{
    UNIMPLEMENTED();
}

bool Source::IsActive() const
{
    if (!m_source)
        return false;

    return obs_source_active(m_source);
}

bool Source::IsShowing() const
{
    if (!m_source)
        return false;

    return obs_source_showing(m_source);
}

void Source::Mute()
{
    if (!m_source)
        return;

    const auto mute = true;
    obs_source_set_muted(m_source, mute);
}

void Source::Unmute()
{
    if (!m_source)
        return;

    const auto mute = false;
    obs_source_set_muted(m_source, mute);
}

bool Source::IsMuted() const
{
    if (!m_source)
        return true;

    return obs_source_muted(m_source);
}

void Source::DeleteSelf()
{
    delete this;
}
