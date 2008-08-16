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


#include "preferences.h"

#include "cfg.h"
#include "resource.h"

using namespace foo_stalker;


HWND
preferences::create
	( HWND parent
	)
{
    return uCreateDialog(IDD_S_CONFIGVIEW, parent, ConfigProc);
};

const char *
preferences::get_name
	()
{
    return "S.T.A.L.K.E.R. Connection";
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

bool
preferences::reset_query
	()
{
    return true;
}

void
preferences::reset
	()
{
	cfg::get_track_change_switch()  = cfg::get_track_change_switch_default();
	cfg::get_track_change_message() = cfg::get_track_change_message_default();

	cfg::get_pause_switch()         = cfg::get_pause_switch_default();
	cfg::get_pause_message()        = cfg::get_pause_message_default();

	cfg::get_stop_switch()          = cfg::get_stop_switch_default();
	cfg::get_stop_message()         = cfg::get_stop_message_default();

	cfg::get_seek_switch()          = cfg::get_seek_switch_default();
	cfg::get_seek_message()         = cfg::get_seek_message_default();
};

BOOL CALLBACK
preferences::ConfigProc
	( HWND   wnd
	, UINT   msg
	, WPARAM wp
	, LPARAM lp
	)
{
    switch(msg)
    {
        case WM_INITDIALOG:
        {
			::uSetDlgItemText(wnd, IDC_TRACK_CHANGE_MESSAGE, cfg::get_track_change_message());
			::uSetDlgItemText(wnd, IDC_PAUSE_MESSAGE,        cfg::get_pause_message());
			::uSetDlgItemText(wnd, IDC_STOP_MESSAGE,         cfg::get_stop_message());
			::uSetDlgItemText(wnd, IDC_SEEK_MESSAGE,         cfg::get_seek_message());

			::SendDlgItemMessage(wnd, IDC_TRACK_CHANGE_SWITCH, BM_SETCHECK,  cfg::get_track_change_switch() ? BST_CHECKED : BST_UNCHECKED, 0);
			::SendDlgItemMessage(wnd, IDC_PAUSE_SWITCH,        BM_SETCHECK,  cfg::get_pause_switch()        ? BST_CHECKED : BST_UNCHECKED, 0);
			::SendDlgItemMessage(wnd, IDC_STOP_SWITCH,         BM_SETCHECK,  cfg::get_stop_switch()         ? BST_CHECKED : BST_UNCHECKED, 0);
			::SendDlgItemMessage(wnd, IDC_SEEK_SWITCH,         BM_SETCHECK,  cfg::get_seek_switch()         ? BST_CHECKED : BST_UNCHECKED, 0);
        } break;
        case WM_COMMAND:
        {
            switch(LOWORD(wp))
            {
                case IDC_TRACK_CHANGE_MESSAGE:
				{
                    if (HIWORD(wp)==EN_UPDATE)
                    {
                        uGetDlgItemText(wnd, IDC_TRACK_CHANGE_MESSAGE, cfg::get_track_change_message());
                    }
				} break;
                case IDC_PAUSE_MESSAGE:
				{
                    if (HIWORD(wp)==EN_UPDATE)
                    {
                        uGetDlgItemText(wnd, IDC_PAUSE_MESSAGE, cfg::get_pause_message());
                    }
				} break;
                case IDC_STOP_MESSAGE:
				{
                    if (HIWORD(wp)==EN_UPDATE)
                    {
						uGetDlgItemText(wnd, IDC_STOP_MESSAGE, cfg::get_stop_message());
                    }
				} break;
                case IDC_SEEK_MESSAGE:
				{
                    if (HIWORD(wp)==EN_UPDATE)
                    {
                        uGetDlgItemText(wnd, IDC_SEEK_MESSAGE, cfg::get_seek_message());
                    }
				} break;
            }
        } break;
    }
    return 0;
}
