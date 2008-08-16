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


#include "event_buffer.h"

#include "../SDK/foobar2000.h"
#include "../helpers/helpers.h"

namespace foo_stalker
{
	//! FB2K playback event handler
	//! record the events into event_buffer
	class play_callback : public ::play_callback
	{
	private:
		
		event_buffer & event_buffer;

		const cfg_bool     & cfg_track_change_switch;
		const cfg_string   & cfg_track_change_message;
		const cfg_bool     & cfg_pause_switch;
		const cfg_string   & cfg_pause_message;
		const cfg_bool     & cfg_stop_switch;
		const cfg_string   & cfg_stop_message;
		const cfg_bool     & cfg_seek_switch;
		const cfg_string   & cfg_seek_message;

	public:

		play_callback
			( event_buffer & event_buffer
			, const cfg_bool     & cfg_track_change_switch
			, const cfg_string   & cfg_track_change_message
			, const cfg_bool     & cfg_pause_switch
			, const cfg_string   & cfg_pause_message
			, const cfg_bool     & cfg_stop_switch
			, const cfg_string   & cfg_stop_message
			, const cfg_bool     & cfg_seek_switch
			, const cfg_string   & cfg_seek_message
			);

		//! Playback process is being initialized. on_playback_new_track() should be called soon after this when first file is successfully opened for decoding.
		void FB2KAPI on_playback_starting(play_control::t_track_command p_command,bool p_paused);

		//! Playback advanced to new track.
		void FB2KAPI on_playback_new_track(metadb_handle_ptr p_track);

		//! Playback stopped.
		void FB2KAPI on_playback_stop(play_control::t_stop_reason p_reason);

		//! User has seeked to specific time.
		void FB2KAPI on_playback_seek(double p_time);

		//! Called on pause/unpause.
		void FB2KAPI on_playback_pause(bool p_state);

		//! Called when currently played file gets edited.
		void FB2KAPI on_playback_edited(metadb_handle_ptr p_track);

		//! Dynamic info (VBR bitrate etc) change.
		void FB2KAPI on_playback_dynamic_info(const file_info & p_info);

		//! Per-track dynamic info (stream track titles etc) change. Happens less often than on_playback_dynamic_info().
		void FB2KAPI on_playback_dynamic_info_track(const file_info & p_info);

		//! Called every second, for time display
		void FB2KAPI on_playback_time(double p_time);

		//! User changed volume settings. Possibly called when not playing.
		//! @param p_new_val new volume level in dB; 0 for full volume.
		void FB2KAPI on_volume_change(float p_new_val);
	};
}