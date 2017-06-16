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

#pragma once

#include "Reflection/ClassRegistry.h"

#include "Exposed/Source.h"

using namespace Script;
using namespace Script::Reflection;

// these aren't nested in a namespace to avoid annoying class names for reflection.
class TextSource : public Source
{
public:
    static TextSource* New();

    void SetText(const std::string& text);
    const std::string& GetText() const;

private:
    std::string m_text;
};

template <>
struct RegisterClass<TextSource>
{
    static void Register(ClassWalker& walker)
    {
        RegisterClass<Source>::Register(walker);

        walker.AddFunction("New", &TextSource::New);
        walker.AddFunction("SetText", &TextSource::SetText);
        walker.AddFunction("GetText", &TextSource::GetText);
    }
};