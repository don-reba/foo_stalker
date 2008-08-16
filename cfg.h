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


namespace foo_stalker
{
	class cfg
	{
	public:

		static bool         get_track_change_switch_default();
		static cfg_bool &   get_track_change_switch();
		static const char * get_track_change_message_default();
		static cfg_string & get_track_change_message();
		static bool         get_pause_switch_default();
		static cfg_bool &   get_pause_switch();
		static const char * get_pause_message_default();
		static cfg_string & get_pause_message();
		static bool         get_stop_switch_default();
		static cfg_bool &   get_stop_switch();
		static const char * get_stop_message_default();
		static cfg_string & get_stop_message();
		static bool         get_seek_switch_default();
		static cfg_bool &   get_seek_switch();
		static const char * get_seek_message_default();
		static cfg_string & get_seek_message();
	};
}
