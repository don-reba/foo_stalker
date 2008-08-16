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


#include "event_buffer.h"

#include "ltx_writer.h"

using namespace foo_stalker;


//---------------------------------
// game_event_buffer implementation
//---------------------------------
	
event_buffer::event_buffer()
	: events (8)
	, id     (0)
{
}

void
event_buffer::add_event
	( pfc::string8 text
	, pfc::string8 type
	)
{
	events[id % events.size()] = game_event(id, text, type);
	++id;
	write_events();
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
		text << "text = " << events[i].get_text() << "\r\n";
		text << "type = " << events[i].get_type() << "\r\n";
		text << "id = "   << events[i].get_id()   << "\r\n";
	}

	ltx_writer & writer = ltx_writer::get_instance();

	writer.reset();
	writer.write(text);
}

//---------------------------------------------
// game_event_buffer::game_event implementation
//---------------------------------------------

event_buffer::game_event::game_event
	()
	: id   (-1)
	, text ()
	, type ()
{
}

event_buffer::game_event::game_event
	( int id
	, pfc::string8 text
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

foo_stalker::event_buffer &
foo_stalker::event_buffer::get_instance()
{
	static event_buffer event_buffer;
	return event_buffer;
}