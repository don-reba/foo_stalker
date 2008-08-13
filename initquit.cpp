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
#include "resource.h"

using foo_stalker;


initquit::initquit()
	: quit(false)
{
}

static void
initquit::find_game_window
	( void *
	)
{
	console::info("foo_stalker: passive mode");
	while (!quit)
	{
		HWND hwnd(::FindWindow(NULL, _T("S.T.A.L.K.E.R.: Shadow Of Chernobyl")));
		if (NULL != hwnd && hwnd == GetForegroundWindow())
		{
			MonitorKeyState(hwnd);
			console::info("foo_stalker: passive mode");
		}
		else
			::Sleep(1024);
	}
}

static void
initquit::monitor_key_state
	( HWND hwnd
	)
{
	console::info("foo_stalker: active mode");

	ltx_writer.open(hwnd);

	static_api_ptr_t<main_thread_callback_manager> callback_manager;

	bool is_active = false;

	bool keys[0x100] = { false };
	while (!quit && hwnd == GetForegroundWindow())
	{
		for (int i = 1; i != 0x100; ++i) 
		{
			const bool key_pressed = (0 != (0x8000 & GetAsyncKeyState(i)));
			if (key_pressed && !keys[i])
			{
				switch (i)
				{
				case VK_SCROLL:
					is_active = !is_active;
					if (is_active)
						console::info("STALKER key forwarding enabled");
					else
						console::info("STALKER key forwarding disabled");
					break;
				default:
					if (is_active)
						callback_manager->add_callback(new service_impl_t<key_callback>(i));
				}
			}
			keys[i] = key_pressed;
		}
		::Sleep(128);
	}

	ltx_writer.close();
}

void
initquit::on_init
	()
{
	quit = false;

	if (-1L == _beginthread(FindGameWindow, 0, NULL))
		console::error("_beginthread(FindGameWindow) failed");

	static_api_ptr_t<play_callback_manager>()->register_callback
		( new service_impl_t<play_callback_stalker>()
		, play_callback_stalker::flag_on_playback_new_track
		, false
		);
}

void
initquit::on_quit
	()
{
	quit = true;
}