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


#pragma once


#include "../SDK/foobar2000.h"
#include "../helpers/helpers.h"

const bool cfg_track_change_switch_default = true;
const char * const cfg_track_change_message_default = "Foobar2000: %tracknumber%. %title%";

namespace foo_stalker
{
	//! Preferences page for foo_stalker.
	class preferences : public preferences_page_v3
	{
	public:

		//! Retrieves name of the prefernces page to be displayed in preferences tree (static string).
		const char * get_name();

		//! Retrieves GUID of the page.
		GUID get_guid();

		//! Retrieves GUID of parent page/branch of this page. See preferences_page::guid_* constants for list of standard parent GUIDs. Can also be a GUID of another page or a branch (see: preferences_branch).
		GUID get_parent_guid();

		preferences_page_instance::ptr instantiate(HWND parent, preferences_page_callback::ptr callback);
	};

	class preferences_instance : public preferences_page_instance
	{
	public:

		preferences_instance(HWND parent, preferences_page_callback::ptr callback);

		//! @returns a combination of preferences_state constants.
		t_uint32 get_state();

		//! @returns the window handle.
		HWND get_wnd();

		//! Applies preferences changes.
		void apply();

		//! Resets this page's content to the default values. Does not apply any changes - lets user preview the changes before hitting "apply".
		void reset();

	private:

		static BOOL CALLBACK config_proc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

		void init_wnd();

	private:

		HWND wnd;

		bool has_changed;
		
		const preferences_page_callback::ptr callback;
	};
}
