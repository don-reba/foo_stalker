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


#include "game_event_buffer.h"

using game_event;


//---------------------------------
// game_event_buffer implementation
//---------------------------------
	
event_buffer::event_buffer
	( int size
	)
	: events (size)
	, id     (0)
{
}

void
event_buffer::add_event
	( pfc::string8 text
	, pfc::string8 type
	)
{
	events[id % events.size()] = GameEvent(id, text, type);
	++id;
	WriteEvents();
}

void
event_buffer::write_events
	()
{
	pfc::string8 text;

	text << "[info]\r\n";
	text << "event_count = " << events.size() << "\r\n";
	
	for (int i = 0; i != events.size(); ++i)
	{
		text << "[event@" << i << "]\r\n";
		text << "text = " << events[i].GetText() << "\r\n";
		text << "type = " << events[i].GetType() << "\r\n";
		text << "id = "   << events[i].GetID()   << "\r\n";
	}

	writer.reset();
	writer.write(text);
}

//---------------------------------------------
// game_event_buffer::game_event implementation
//---------------------------------------------

event_buffer::game_event::game_event
	()
	: id   (-1)
	: text ()
	: type ()
{
}

event_buffer::game_event::game_event
	( int id, pfc::string8 text
	, pfc::string8 type
	)
	: id   (id)
	, text (text)
	, type (type)
{
}

int event_buffer::game_event::get_id
	() const
{
	return id;
}

pfc::string8
event_buffer::game_event::get_text
	() const
{
	return text;
}

pfc::string8
event_buffer::game_event::get_type
	() const
{
	return type;
}
