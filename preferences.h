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


#pragma once


#include "../SDK/foobar2000.h"
#include "../helpers/helpers.h"

const bool cfg_track_change_switch_default = true;
const char * const cfg_track_change_message_default = "Foobar2000: %tracknumber%. %title%";

namespace foo_stalker
{
	//! Preferences page for foo_stalker.
	class preferences : public preferences_page
	{
	private:

		const bool cfg_pause_switch_default;
		const bool cfg_seek_switch_default;
		const bool cfg_stop_switch_default;
		const bool cfg_track_change_switch_default;
		const char * const cfg_pause_message_default;
		const char * const cfg_seek_message_default;
		const char * const cfg_stop_message_default;
		const char * const cfg_track_change_message_default;

		const cfg_bool & cfg_pause_switch;
		const cfg_bool & cfg_seek_switch;
		const cfg_bool & cfg_stop_switch;
		const cfg_bool & cfg_track_change_switch;
		const cfg_string & cfg_pause_message;
		const cfg_string & cfg_seek_message;
		const cfg_string & cfg_stop_message;
		const cfg_string & cfg_track_change_message;

	public:

		preferences
			( const bool cfg_pause_switch_default
			, const bool cfg_seek_switch_default
			, const bool cfg_stop_switch_default
			, const bool cfg_track_change_switch_default
			, const char * const cfg_pause_message_default
			, const char * const cfg_seek_message_default
			, const char * const cfg_stop_message_default
			, const char * const cfg_track_change_message_default
			, const cfg_bool & cfg_pause_switch
			, const cfg_bool & cfg_seek_switch
			, const cfg_bool & cfg_stop_switch
			, const cfg_bool & cfg_track_change_switch
			, const cfg_string & cfg_pause_message
			, const cfg_string & cfg_seek_message
			, const cfg_string & cfg_stop_message
			, const cfg_string & cfg_track_change_message
			);

		//! Creates preferences page dialog window. It is safe to assume that two dialog instances will never coexist. Caller is responsible for embedding it into preferences dialog itself.
		HWND create(HWND parent);

		//! Retrieves name of the prefernces page to be displayed in preferences tree (static string).
		const char * get_name();

		//! Retrieves GUID of the page.
		GUID get_guid();

		//! Retrieves GUID of parent page/branch of this page. See preferences_page::guid_* constants for list of standard parent GUIDs. Can also be a GUID of another page or a branch (see: preferences_branch).
		GUID get_parent_guid();

		//! Queries whether this page supports "reset page" feature.
		bool reset_query();

		//! Activates "reset page" feature. It is safe to assume that the preferences page dialog does not exist at the point this is called (caller destroys it before calling reset and creates it again afterwards).
		void reset();

	private:

		static BOOL CALLBACK ConfigProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	};
}
