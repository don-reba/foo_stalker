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


#include "cfg.h"

using namespace foo_stalker;


//-------------------------------
// static configuration variables
//-------------------------------

// track change event
const bool cfg_track_change_switch_default = true;
static cfg_bool cfg_track_change_switch
	( pfc::GUID_from_text("d0d89231-ca05-4ec9-bf53-f51284dd3dd5")
	, cfg_track_change_switch_default
	);
const char * const cfg_track_change_message_default = "Foobar2000: playing %artist% - %title%";
static cfg_string cfg_track_change_message
	( pfc::GUID_from_text("883e6696-fce1-42e1-9d49-5fc9b29bb848")
	, cfg_track_change_message_default
	);

// pause event
const bool cfg_pause_switch_default = true;
static cfg_bool cfg_pause_switch
	( pfc::GUID_from_text("cd8cdb7c-4d52-451a-b6e7-cc55a4526cc4")
	, cfg_pause_switch_default
	);
const char * const cfg_pause_message_default = "Foobar2000: pause";
static cfg_string cfg_pause_message
	( pfc::GUID_from_text("caed806b-841f-4921-950c-6ba1d472335f")
	, cfg_pause_message_default
	);

// stop event
const bool cfg_stop_switch_default = true;
static cfg_bool cfg_stop_switch
	( pfc::GUID_from_text("4f141aa7-cb21-47ac-9c82-95b4660918ba")
	, cfg_stop_switch_default
	);
const char * const cfg_stop_message_default = "Foobar2000: stop";
static cfg_string cfg_stop_message
	( pfc::GUID_from_text("69fe7f41-8a68-4749-b981-0a0cd3b23dca")
	, cfg_stop_message_default
	);

// seek event
const bool cfg_seek_switch_default = true;
static cfg_bool cfg_seek_switch
	( pfc::GUID_from_text("6eaff465-5fd3-429a-aab6-3b66687cf9da")
	, cfg_seek_switch_default
	);
const char * const cfg_seek_message_default = "Foobar2000: seek";
static cfg_string cfg_seek_message
	( pfc::GUID_from_text("977ab552-b351-450e-8a7f-7f3d433d65d5")
	, cfg_seek_message_default
	);

//-------------------
// cfg implementation
//-------------------

bool         cfg::get_track_change_switch_default()  { return cfg_track_change_switch_default;  }
cfg_bool &   cfg::get_track_change_switch()          { return cfg_track_change_switch;          }
const char * cfg::get_track_change_message_default() { return cfg_track_change_message_default; }
cfg_string & cfg::get_track_change_message()         { return cfg_track_change_message;         }
bool         cfg::get_pause_switch_default()         { return cfg_pause_switch_default;         }
cfg_bool &   cfg::get_pause_switch()                 { return cfg_pause_switch;                 }
const char * cfg::get_pause_message_default()        { return cfg_pause_message_default;        }
cfg_string & cfg::get_pause_message()                { return cfg_pause_message;                }
bool         cfg::get_stop_switch_default()          { return cfg_stop_switch_default;          }
cfg_bool &   cfg::get_stop_switch()                  { return cfg_stop_switch;                  }
const char * cfg::get_stop_message_default()         { return cfg_stop_message_default;         }
cfg_string & cfg::get_stop_message()                 { return cfg_stop_message;                 }
bool         cfg::get_seek_switch_default()          { return cfg_seek_switch_default;          }
cfg_bool &   cfg::get_seek_switch()                  { return cfg_seek_switch;                  }
const char * cfg::get_seek_message_default()         { return cfg_seek_message_default;         }
cfg_string & cfg::get_seek_message()                 { return cfg_seek_message;                 }
