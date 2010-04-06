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


#include "preferences.h"

#include "cfg.h"
#include "resource.h"
#include <windowsx.h>

using namespace foo_stalker;

//---------------------------
// preferences implementation
//---------------------------

const char *
preferences::get_name
	()
{
    return "S.T.A.L.K.E.R. Link";
}

GUID
preferences::get_guid
	()
{
    return pfc::GUID_from_text("1d07f088-7b6f-4c11-abb0-72e71b72415b");
}

GUID
preferences::get_parent_guid
	()
{
    return preferences_page::guid_tools;
}

preferences_page_instance::ptr
preferences::instantiate
	( HWND parent
	, preferences_page_callback::ptr callback
	)
{
	return new service_impl_t<preferences_instance>(parent, callback);
}

//------------------------------------
// preferences_instance implementation
//------------------------------------

preferences_instance::preferences_instance
	( HWND parent
	, preferences_page_callback::ptr callback
	)
	: callback    (callback)
	, has_changed (false)
{
	wnd = uCreateDialog
		( IDD_S_CONFIGVIEW
		, parent
		, config_proc
		, reinterpret_cast<LPARAM>(this)
		);
	init_wnd();
}

t_uint32
preferences_instance::get_state
	()
{
	t_uint32 state = preferences_state::resettable;
	if (has_changed)
		state |= preferences_state::changed;
	return state;
}

HWND
preferences_instance::get_wnd
	()
{
	return wnd;
}

void
preferences_instance::apply
	()
{
	BOOL translated;
	int  result;

	translated = FALSE;
	result = ::uGetDlgItemInt(wnd, IDC_FAST_GAME_POLLING_DELAY, &translated, false);
	if (translated)
		cfg::get_fast_game_polling_delay() = result;

	translated = FALSE;
	result = ::uGetDlgItemInt(wnd, IDC_SLOW_GAME_POLLING_DELAY, &translated, false);
	if (translated)
		cfg::get_slow_game_polling_delay() = result;

	::uGetDlgItemText(wnd, IDC_PAUSE_MESSAGE,        cfg::get_pause_message());
	::uGetDlgItemText(wnd, IDC_SEEK_MESSAGE,         cfg::get_seek_message());
	::uGetDlgItemText(wnd, IDC_STOP_MESSAGE,         cfg::get_stop_message());
	::uGetDlgItemText(wnd, IDC_TRACK_CHANGE_MESSAGE, cfg::get_track_change_message());
	::uGetDlgItemText(wnd, IDC_VOLUME_MESSAGE,       cfg::get_volume_message());

	cfg::get_pause_switch()         = Button_GetCheck(::GetDlgItem(wnd, IDC_PAUSE_SWITCH))         == BST_CHECKED;
	cfg::get_seek_switch()          = Button_GetCheck(::GetDlgItem(wnd, IDC_SEEK_SWITCH))          == BST_CHECKED;
	cfg::get_stop_switch()          = Button_GetCheck(::GetDlgItem(wnd, IDC_STOP_SWITCH))          == BST_CHECKED;
	cfg::get_suspend_input_switch() = Button_GetCheck(::GetDlgItem(wnd, IDC_SUSPEND_INPUT_SWITCH)) == BST_CHECKED;
	cfg::get_track_change_switch()  = Button_GetCheck(::GetDlgItem(wnd, IDC_TRACK_CHANGE_SWITCH))  == BST_CHECKED;
	cfg::get_volume_switch()        = Button_GetCheck(::GetDlgItem(wnd, IDC_VOLUME_SWITCH))        == BST_CHECKED;

	has_changed = false;
}

void
preferences_instance::reset
	()
{
	cfg::get_suspend_input_switch() = cfg::get_suspend_input_switch_default();

	cfg::get_slow_game_polling_delay() = cfg::get_slow_game_polling_delay_default();
	cfg::get_fast_game_polling_delay() = cfg::get_fast_game_polling_delay_default();

	cfg::get_track_change_switch()  = cfg::get_track_change_switch_default();
	cfg::get_track_change_message() = cfg::get_track_change_message_default();

	cfg::get_pause_switch()  = cfg::get_pause_switch_default();
	cfg::get_pause_message() = cfg::get_pause_message_default();

	cfg::get_stop_switch()  = cfg::get_stop_switch_default();
	cfg::get_stop_message() = cfg::get_stop_message_default();

	cfg::get_seek_switch()  = cfg::get_seek_switch_default();
	cfg::get_seek_message() = cfg::get_seek_message_default();

	cfg::get_volume_switch()  = cfg::get_volume_switch_default();
	cfg::get_volume_message() = cfg::get_volume_message_default();

	has_changed = false;

	init_wnd();
}

BOOL CALLBACK
preferences_instance::config_proc
	( HWND   wnd
	, UINT   msg
	, WPARAM wp
	, LPARAM lp
	)
{
	preferences_instance * obj
		= reinterpret_cast<preferences_instance*>(::GetWindowLong(wnd, GWL_USERDATA));
	switch(msg)
	{
	case WM_INITDIALOG:
		::SetWindowLong(wnd, GWL_USERDATA, lp);
		obj = reinterpret_cast<preferences_instance*>(lp);
		break;
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case IDC_FAST_GAME_POLLING_DELAY: if (HIWORD(wp) == EN_UPDATE)  obj->has_changed = true; break;
		case IDC_PAUSE_MESSAGE:           if (HIWORD(wp) == EN_UPDATE)  obj->has_changed = true; break;
		case IDC_PAUSE_SWITCH:            if (HIWORD(wp) == BN_CLICKED) obj->has_changed = true; break;
		case IDC_SEEK_MESSAGE:            if (HIWORD(wp) == EN_UPDATE)  obj->has_changed = true; break;
		case IDC_SEEK_SWITCH:             if (HIWORD(wp) == BN_CLICKED) obj->has_changed = true; break;
		case IDC_SLOW_GAME_POLLING_DELAY: if (HIWORD(wp) == EN_UPDATE)  obj->has_changed = true; break;
		case IDC_STOP_MESSAGE:            if (HIWORD(wp) == EN_UPDATE)  obj->has_changed = true; break;
		case IDC_STOP_SWITCH:             if (HIWORD(wp) == BN_CLICKED) obj->has_changed = true; break;
		case IDC_SUSPEND_INPUT_SWITCH:    if (HIWORD(wp) == BN_CLICKED) obj->has_changed = true; break;
		case IDC_TRACK_CHANGE_MESSAGE:    if (HIWORD(wp) == EN_UPDATE)  obj->has_changed = true; break;
		case IDC_TRACK_CHANGE_SWITCH:     if (HIWORD(wp) == BN_CLICKED) obj->has_changed = true; break;
		case IDC_VOLUME_MESSAGE:          if (HIWORD(wp) == EN_UPDATE)  obj->has_changed = true; break;
		case IDC_VOLUME_SWITCH:           if (HIWORD(wp) == BN_CLICKED) obj->has_changed = true; break;
		}
		break;
	}
	if (obj && obj->has_changed)
		obj->callback->on_state_changed();
	return 0;
}

void
preferences_instance::init_wnd()
{
	::SendDlgItemMessage(wnd, IDC_SUSPEND_INPUT_SWITCH, BM_SETCHECK, cfg::get_suspend_input_switch() ? BST_CHECKED : BST_UNCHECKED, 0);

	::uSetDlgItemInt(wnd, IDC_SLOW_GAME_POLLING_DELAY, cfg::get_slow_game_polling_delay(), FALSE);
	::uSetDlgItemInt(wnd, IDC_FAST_GAME_POLLING_DELAY, cfg::get_fast_game_polling_delay(), FALSE);

	::uSetDlgItemText(wnd, IDC_TRACK_CHANGE_MESSAGE, cfg::get_track_change_message());
	::uSetDlgItemText(wnd, IDC_PAUSE_MESSAGE,        cfg::get_pause_message());
	::uSetDlgItemText(wnd, IDC_STOP_MESSAGE,         cfg::get_stop_message());
	::uSetDlgItemText(wnd, IDC_SEEK_MESSAGE,         cfg::get_seek_message());
	::uSetDlgItemText(wnd, IDC_VOLUME_MESSAGE,       cfg::get_volume_message());

	::SendDlgItemMessage(wnd, IDC_TRACK_CHANGE_SWITCH, BM_SETCHECK, cfg::get_track_change_switch() ? BST_CHECKED : BST_UNCHECKED, 0);
	::SendDlgItemMessage(wnd, IDC_PAUSE_SWITCH,        BM_SETCHECK, cfg::get_pause_switch()        ? BST_CHECKED : BST_UNCHECKED, 0);
	::SendDlgItemMessage(wnd, IDC_STOP_SWITCH,         BM_SETCHECK, cfg::get_stop_switch()         ? BST_CHECKED : BST_UNCHECKED, 0);
	::SendDlgItemMessage(wnd, IDC_SEEK_SWITCH,         BM_SETCHECK, cfg::get_seek_switch()         ? BST_CHECKED : BST_UNCHECKED, 0);
	::SendDlgItemMessage(wnd, IDC_VOLUME_SWITCH,       BM_SETCHECK, cfg::get_volume_switch()       ? BST_CHECKED : BST_UNCHECKED, 0);

	has_changed = false;
}
