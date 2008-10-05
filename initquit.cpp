// Copyright (c) 2008, Alexey Badalov
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// • Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
// • Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 
// • The name of foo_stalker and the names of its contributors may not be used to endorse or promote products derived from this software without specific prior written permission. 
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

#include "cfg.h"
#include "event_buffer.h"
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

void
foo_stalker::initquit::find_game_window
	( void *
	)
{
	const TCHAR * const window_name(_T("S.T.A.L.K.E.R.: Shadow Of Chernobyl"));

	ltx_writer   & writer = ltx_writer::get_instance();
	event_buffer & events = event_buffer::get_instance();

	bool keys[0x100] = { false };

	bool can_track        (false);
	bool was_window_valid (false);
	bool was_tracking     (false);
	HWND wnd              (NULL);
	while (!quit)
	{
		int delay(cfg::get_idle_polling_delay());
		// verify current window validity
		HWND foreground_wnd(::GetForegroundWindow());
		bool is_window_valid(is_valid_window(wnd, foreground_wnd));
		// close the events file
		if (was_window_valid && !is_window_valid)
		{
			writer.close();
		}
		// search for the window
		if (!is_window_valid)
		{
			wnd = ::FindWindow(NULL, window_name);
			is_window_valid = is_valid_window(wnd, foreground_wnd);
		}
		// open the events file
		if (!was_window_valid && is_window_valid)
		{
			writer.open(wnd);
			events.clear();
			event_buffer::get_instance().add_event(cfg::get_init_message(), "init");
		}
		// track keyboard input
		if (is_window_valid)
		{
			events.set_polling_delay(cfg::get_slow_game_polling_delay());
			delay = cfg::get_window_polling_delay();
			if (is_tracking())
			{
				if (can_track)
				{
					if (!was_tracking)
						events.add_event(cfg::get_control_on_message(), "control_on");
					was_tracking = true;

					events.set_polling_delay(cfg::get_fast_game_polling_delay());
					delay = cfg::get_tracking_polling_delay();

					monitor_key_state(keys);
				}
			}
			else
			{
				if (was_tracking)
					events.add_event(cfg::get_control_off_message(), "control_off");
				was_tracking = false;

				can_track = true;
			}
		}
		was_window_valid = is_window_valid;
		Sleep(delay);
	}
}

void
foo_stalker::initquit::monitor_key_state
	( bool * keys // key state saved between calls
	)
{
	static_api_ptr_t<main_thread_callback_manager> callback_manager;
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

bool
foo_stalker::initquit::is_valid_window
	( HWND window
	, HWND foreground_window
	)
{
	return window != NULL && window == foreground_window;
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
