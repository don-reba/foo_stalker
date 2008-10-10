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


#include "play_callback.h"

#include "cfg.h"
#include "event_buffer.h"


//-----------------------------
// play_callback implementation
//-----------------------------

//! Playback process is being initialized. on_playback_new_track() should be called soon after this when first file is successfully opened for decoding.
void FB2KAPI
foo_stalker::play_callback::on_playback_starting
	( play_control::t_track_command p_command
	, bool p_paused
	)
{
}

//! Playback advanced to new track.
void FB2KAPI
foo_stalker::play_callback::on_playback_new_track
	( metadb_handle_ptr p_track
	)
{
	if (foo_stalker::cfg::get_track_change_switch())
	{
		service_ptr_t<titleformat_object> script;
		static_api_ptr_t<titleformat_compiler>()->compile_safe(script, foo_stalker::cfg::get_track_change_message());

		pfc::string8 message;
		p_track->format_title
			( NULL    // p_hook
			, message // p_out
			, script  // p_script
			, NULL    // p_filter
			);

		pfc::stringcvt::string_ansi_from_utf8 message_converter
			( message.get_ptr()    // p_source
			, message.get_length() // p_source_size
			);
		pfc::string8 ascii_message
			( message_converter.get_ptr() // p_string
			, message_converter.length()  // p_length
			);

		add_event(ascii_message, "track_change");
	}
}

//! Playback stopped.
void FB2KAPI
foo_stalker::play_callback::on_playback_stop
	( play_control::t_stop_reason p_reason
	)
{
	if (p_reason == play_control::t_stop_reason::stop_reason_user)
	if (foo_stalker::cfg::get_stop_switch())
		add_event(foo_stalker::cfg::get_stop_message(), "stop");
}

//! User has seeked to specific time.
void FB2KAPI
foo_stalker::play_callback::on_playback_seek
	( double p_time
	)
{
	if (foo_stalker::cfg::get_seek_switch())
	{
		pfc::string message(cfg::get_seek_message());
		add_event
			( message.replace("%time%", pfc::format_time_ex(p_time, 1).get_ptr()).ptr()
			, "seek"
			);
	}
}

//! Called on pause/unpause.
void FB2KAPI
foo_stalker::play_callback::on_playback_pause
	( bool p_state
	)
{
	if (foo_stalker::cfg::get_pause_switch())
		add_event(foo_stalker::cfg::get_pause_message(), "pause");
}

//! Called when currently played file gets edited.
void FB2KAPI
foo_stalker::play_callback::on_playback_edited
	( metadb_handle_ptr p_track
	)
{
}

//! Dynamic info (VBR bitrate etc) change.
void FB2KAPI
foo_stalker::play_callback::on_playback_dynamic_info
	( const file_info & p_info
	)
{
}

//! Per-track dynamic info (stream track titles etc) change. Happens less often than on_playback_dynamic_info().
void FB2KAPI
foo_stalker::play_callback::on_playback_dynamic_info_track
	( const file_info & p_info
	)
{
}

//! Called every second, for time display
void FB2KAPI
foo_stalker::play_callback::on_playback_time
	( double p_time
	)
{
}

//! User changed volume settings. Possibly called when not playing.
//! @param p_new_val new volume level in dB; 0 for full volume.
void FB2KAPI
foo_stalker::play_callback::on_volume_change
	( float p_new_val
	)
{
	if (foo_stalker::cfg::get_volume_switch())
	{
		pfc::string message(cfg::get_volume_message());
		add_event
			( message.replace("%volume%", pfc::format_float(p_new_val, 0, 2).get_ptr()).ptr()
			, (-100.0f == p_new_val) ? "volume_mute" : "volume_change"
			);
	}
}

//! Return the flags corresponding to the base class functions implemented.
unsigned
foo_stalker::play_callback::get_flags()
{
	return
		flag_on_playback_new_track
		| flag_on_playback_stop
		| flag_on_playback_seek
		| flag_on_playback_pause
		| flag_on_volume_change
		;
}

void
foo_stalker::play_callback::add_event(pfc::string8 message, pfc::string8 type)
{
	event_buffer::get_instance().add_event(message, type);
}
