// Copyright (c) 2008, Alexey Badalov
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// • Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
// • Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 
// • The name of foo_stalker may not be used to endorse or promote products derived from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "initquit.h"

#include "ltx_writer.h"
#include "play_callback.h"
#include "resource.h"


//-----------------------------
// static member initialization
//-----------------------------

bool foo_stalker::initquit::quit         = false;
bool foo_stalker::initquit::was_tracking = false;

//------------------------
// initquit implementation
//------------------------

foo_stalker::initquit::initquit()
{
}

void
foo_stalker::initquit::find_game_window
	( void *
	)
{
	// was_tracking is used to make sure tracking is not triggered
	// when the game just runs
	console::info("foo_stalker: passive mode");
	was_tracking = true;
	while (!quit)
	{
		//HWND hwnd(::FindWindow(NULL, _T("S.T.A.L.K.E.R.: Shadow Of Chernobyl")));
		HWND hwnd(::FindWindow(NULL, _T("Untitled - Notepad")));
		bool is_tracking = initquit::is_tracking();
		if
			(  NULL != hwnd
			&& hwnd == GetForegroundWindow()
			&& !was_tracking
			&& is_tracking
			)
		{
			monitor_key_state(hwnd);
			console::info("foo_stalker: passive mode");
			was_tracking = true;
		}
		else
		{
			was_tracking = is_tracking;
			::Sleep(1024);
		}
	}
}

void
foo_stalker::initquit::monitor_key_state
	( HWND hwnd
	)
{
	console::info("foo_stalker: active mode");

	ltx_writer & writer = ltx_writer::get_instance();

	writer.open(hwnd);

	static_api_ptr_t<main_thread_callback_manager> callback_manager;

	bool is_active = false;

	bool keys[0x100] = { false };

	while
		(  !quit
		&& hwnd == GetForegroundWindow()
		&& is_tracking()
		)
	{
		for (int i = 1; i != 0x100; ++i) 
		{
			if (i != VK_SCROLL)
			{
				const bool key_pressed = (0 != (0x8000 & GetAsyncKeyState(i)));
				if (key_pressed && !keys[i])
					callback_manager->add_callback(new service_impl_t<key_callback>(i));
				keys[i] = key_pressed;
			}
		}
		::Sleep(128);
	}

	writer.close();
}

void
foo_stalker::initquit::on_init
	()
{
	quit = false;

	if (-1L == _beginthread(find_game_window, 0, NULL))
		console::error("_beginthread(FindGameWindow) failed");

	static_api_ptr_t<play_callback_manager>()->register_callback
		( new service_impl_t<foo_stalker::play_callback>()
		, foo_stalker::play_callback::get_flags()
		, false
		);
}

void
foo_stalker::initquit::on_quit
	()
{
	quit = true;
}

bool
foo_stalker::initquit::is_tracking()
{
	return 0x1 & ::GetKeyState(VK_SCROLL);
}

//----------------------------
// key_callback implementation
//----------------------------

foo_stalker::key_callback::key_callback
	( WPARAM key
	)
	: key (key)
{
}

void
foo_stalker::key_callback::callback_run
	()
{
	service_enum_t<keyboard_shortcut_manager> e;
	service_ptr_t<keyboard_shortcut_manager> p;
	while (e.next(p))
		p->on_keydown_auto(key);
}
