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

#include "ScriptMan.h"

#include "Interface/IContext.h"
#include "Interface/IScript.h"

namespace Script
{

	Manager::Manager()
		: m_context(nullptr)
	{

	}

	void Manager::Initialize()
	{
		if ( !m_context )
			return;

		m_context->Open();
		m_context->Init();

		for ( const auto& iter : m_scripts )
		{
			iter->SetFile(std::string(""));
			iter->Load( m_context );
		}
	}

	void Manager::Finalize()
	{
		if ( !m_context )
			return;

		m_context->Close();
		m_context = nullptr;
	}

	void Manager::SetContext( const std::shared_ptr<IContext>& context )
	{
		m_context = context;
	}



}