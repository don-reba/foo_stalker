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


#include "resource.h"

#include "../SDK/foobar2000.h"
#include "../helpers/helpers.h"

#include <process.h>
#include <conio.h>
#include <Psapi.h>
#include <strsafe.h>
#include <shlwapi.h>

#include <vector>

DECLARE_COMPONENT_VERSION
	( "S.T.A.L.K.E.R. Connection"
	, "0.4"
	, "Connection to the game S.T.A.L.K.E.R.: Shadow of Chernobyl"
	);

// track change event
const bool cfg_track_change_switch_default = true;
static cfg_bool cfg_track_change_switch
	( pfc::GUID_from_text("d0d89231-ca05-4ec9-bf53-f51284dd3dd5")
	, cfg_track_change_switch_default
	);
const char * const cfg_track_change_message_default = "Foobar2000: %tracknumber%. %title%";
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


class LtxWriter
{
private:

	critical_section2 cs;
	HANDLE ltx_file;

public:

	LtxWriter()
		: ltx_file(INVALID_HANDLE_VALUE)
	{
	}

	~LtxWriter()
	{
		close();
	}

	void open(HWND hwnd)
	{
		insync2(cs);
		close();
		ltx_file = OpenLtxFile(hwnd);
	}

	void close()
	{
		insync2(cs);
		if (INVALID_HANDLE_VALUE != ltx_file)
		{
			CloseHandle(ltx_file);
			ltx_file = INVALID_HANDLE_VALUE;
		}
	}

	void write(pfc::string8 text)
	{
		insync2(cs);
		if (INVALID_HANDLE_VALUE == ltx_file)
		{
			console::error("foo_stalker is trying to write a closed file");
			return;
		}
		else
		{
			DWORD bytes(strlen(text.get_ptr()));
			DWORD bytes_written(0);
			if (0 == ::WriteFile
				( ltx_file       // hFile
				, text.get_ptr() // lpBuffer
				, bytes          // nNumberOfBytesToWRite
				, &bytes_written // lpNumberOfBytesWritten
				, NULL           // lpOverlapped
				))
			{
				console::error("foo_stalker failed to write to file");
				return;
			}
		}
	}

private:

	static HANDLE OpenLtxFile(HWND hwnd)
	{
		DWORD process_id(0);
		::GetWindowThreadProcessId(hwnd, &process_id);
		if (0 == process_id)
		{
			console::error("GetWindowThreadProcessId failed");
			return INVALID_HANDLE_VALUE;
		}

		HANDLE process_handle = ::OpenProcess
			( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ // dwDesiredAccess
			, FALSE                                     // bInheritHandle
			, process_id                                // dwProcessId
			);
		if (NULL == process_handle)
		{
			console::error("OpenProcess failed");
			return INVALID_HANDLE_VALUE;
		}

		wchar_t path[MAX_PATH] = {};
		if (0 == ::GetModuleFileNameExW(process_handle, NULL, path, MAX_PATH))
		{
			console::error("GetModuleFileNameEx failed");
			return INVALID_HANDLE_VALUE;
		}

		::PathRemoveFileSpecW(path);
		::PathCombineW(path, path, L"..\\gamedata");

		if (!::PathFileExistsW(path))
		{
			console::error("gamedata does not exist");
			return INVALID_HANDLE_VALUE;
		}

		::PathCombine(path, path, L"config");
		::CreateDirectoryW(path, NULL);

		::PathCombineW(path, path, L"fb2k.ltx");

		HANDLE handle = CreateFileW
			( path                             // lpFileName
			, GENERIC_WRITE                    // dwDesiredAccess
			, FILE_SHARE_READ|FILE_SHARE_WRITE // dwShareMode
			, NULL                             // lpSecurityAttributes
			, CREATE_ALWAYS                    // dwCreationDisposition
			, FILE_ATTRIBUTE_NORMAL            // dwFlagsAndAttributes
			, NULL                             // hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == handle)
		{
			DWORD error = ::GetLastError();
			pfc::string8 error_message;
			::uFormatSystemErrorMessage(error_message, error);

			pfc::string8 message;
			message << "foo_stalker could not create file: '";
			message << error_message;
			message << "'";
			console::error(message.get_ptr());
		}
		return handle;
	}
};

LtxWriter ltx_writer;

class key_callback : public main_thread_callback
{
private:

	WPARAM key;

public:

	key_callback(WPARAM key)
		: key(key)
	{
	}

	void callback_run()
	{
		service_enum_t<keyboard_shortcut_manager> e;
		service_ptr_t<keyboard_shortcut_manager> p;
		while (e.next(p))
			p->on_keydown_auto(key);
	}
};

class play_callback_stalker : public play_callback
{
private:
	
	int id;

public:

	play_callback_stalker()
		: id(0)
	{
	}

	unsigned get_flags()
	{
		return flag_on_playback_new_track;
	}

	//! Playback process is being initialized. on_playback_new_track() should be called soon after this when first file is successfully opened for decoding.
	void FB2KAPI on_playback_starting(play_control::t_track_command p_command,bool p_paused)
	{
	}

	//! Playback advanced to new track.
	void FB2KAPI on_playback_new_track(metadb_handle_ptr p_track)
	{
		if (cfg_track_change_switch)
		{
			service_ptr_t<titleformat_object> script;
			static_api_ptr_t<titleformat_compiler>()->compile_safe(script, cfg_track_change_message);

			pfc::string8 message;
			p_track->format_title
				( NULL    // p_hook
				, message // p_out
				, script  // p_script
				, NULL    // p_filter
				);

			WriteEvent(message, "track_change");
		}
	}

	//! Playback stopped.
	void FB2KAPI on_playback_stop(play_control::t_stop_reason p_reason)
	{
		if (cfg_stop_switch)
			WriteEvent(cfg_stop_message, "stop");
	}

	//! User has seeked to specific time.
	void FB2KAPI on_playback_seek(double p_time)
	{
		if (cfg_seek_switch)
			WriteEvent(cfg_seek_message, "seek");
	}

	//! Called on pause/unpause.
	void FB2KAPI on_playback_pause(bool p_state)
	{
		if (cfg_pause_switch)
			WriteEvent(cfg_pause_message, "pause");
	}

	//! Called when currently played file gets edited.
	void FB2KAPI on_playback_edited(metadb_handle_ptr p_track)
	{
	}

	//! Dynamic info (VBR bitrate etc) change.
	void FB2KAPI on_playback_dynamic_info(const file_info & p_info)
	{
	}

	//! Per-track dynamic info (stream track titles etc) change. Happens less often than on_playback_dynamic_info().
	void FB2KAPI on_playback_dynamic_info_track(const file_info & p_info)
	{
	}

	//! Called every second, for time display
	void FB2KAPI on_playback_time(double p_time)
	{
	}

	//! User changed volume settings. Possibly called when not playing.
	//! @param p_new_val new volume level in dB; 0 for full volume.
	void FB2KAPI on_volume_change(float p_new_val)
	{
	}

private:

	void WriteEvent(pfc::string8 message, pfc::string8 type)
	{
		pfc::string8 text;
		text << "[event@" << id++ << "]\r\n";
		text << "text = " << message << "\r\n";
		text << "type = " << type    << "\r\n";
		ltx_writer.write(text);
	}
};

class initquit_stalker : public initquit
{
private:

	static bool quit;

	static void FindGameWindow(void *)
	{
		console::info("foo_stalker: passive mode");
		while (!quit)
		{
			HWND hwnd(::FindWindow(NULL, _T("S.T.A.L.K.E.R.: Shadow Of Chernobyl")));
			if (NULL != hwnd && hwnd == GetForegroundWindow())
			{
				MonitorKeyState(hwnd);
				console::info("foo_stalker: passive mode");
			}
			else
				::Sleep(1024);
		}
	}

	static void MonitorKeyState(HWND hwnd)
	{
		console::info("foo_stalker: active mode");

		ltx_writer.open(hwnd);

		static_api_ptr_t<main_thread_callback_manager> callback_manager;

		bool is_active = false;

		bool keys[0x100] = { false };
		while (!quit && hwnd == GetForegroundWindow())
		{
			for (int i = 1; i != 0x100; ++i) 
			{
				const bool key_pressed = (0 != (0x8000 & GetAsyncKeyState(i)));
				if (key_pressed && !keys[i])
				{
					switch (i)
					{
					case VK_SCROLL:
						is_active = !is_active;
						if (is_active)
							console::info("STALKER key forwarding enabled");
						else
							console::info("STALKER key forwarding disabled");
						break;
					default:
						if (is_active)
							callback_manager->add_callback(new service_impl_t<key_callback>(i));
					}
				}
				keys[i] = key_pressed;
			}
			::Sleep(128);
		}

		ltx_writer.close();
	}

public:

	virtual void on_init()
	{
		quit = false;

		if (-1L == _beginthread(FindGameWindow, 0, NULL))
			console::error("_beginthread(FindGameWindow) failed");

		static_api_ptr_t<play_callback_manager>()->register_callback
			( new service_impl_t<play_callback_stalker>()
			, play_callback_stalker::flag_on_playback_new_track
			, false
			);
	}

	virtual void on_quit()
	{
		quit = true;
	}
};

bool initquit_stalker::quit = false;

static initquit_factory_t<initquit_stalker> initquit_factory;

class config_stalker : public preferences_page
{
public:

    virtual HWND create(HWND parent)
    {
        return uCreateDialog(IDD_S_CONFIGVIEW, parent, ConfigProc);
    };

    virtual const char * get_name()
    {
        return "S.T.A.L.K.E.R. Connection";
    };

    virtual GUID get_guid()
    {
        return pfc::GUID_from_text("1d07f088-7b6f-4c11-abb0-72e71b72415b");
    }

    virtual GUID get_parent_guid()
    {
        return preferences_page::guid_tools;
    };

    virtual bool reset_query()
    {
        return true;
    };

    virtual void reset()
    {
		cfg_track_change_switch  = cfg_track_change_switch_default;
		cfg_track_change_message = cfg_track_change_message_default;

		cfg_pause_switch         = cfg_pause_switch_default;
		cfg_pause_message        = cfg_pause_message_default;

		cfg_stop_switch          = cfg_stop_switch_default;
		cfg_stop_message         = cfg_stop_message_default;

		cfg_seek_switch          = cfg_seek_switch_default;
		cfg_seek_message         = cfg_seek_message_default;
    };

    static BOOL CALLBACK ConfigProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        switch(msg)
        {
            case WM_INITDIALOG:
            {
				::uSetDlgItemText(wnd, IDC_TRACK_CHANGE_MESSAGE, cfg_track_change_message);
				::uSetDlgItemText(wnd, IDC_PAUSE_MESSAGE,        cfg_pause_message);
				::uSetDlgItemText(wnd, IDC_STOP_MESSAGE,         cfg_stop_message);
				::uSetDlgItemText(wnd, IDC_SEEK_MESSAGE,         cfg_seek_message);

				::SendDlgItemMessage(wnd, IDC_TRACK_CHANGE_SWITCH, BM_SETCHECK,  cfg_track_change_switch ? BST_CHECKED : BST_UNCHECKED, 0);
				::SendDlgItemMessage(wnd, IDC_PAUSE_SWITCH,        BM_SETCHECK,  cfg_pause_switch        ? BST_CHECKED : BST_UNCHECKED, 0);
				::SendDlgItemMessage(wnd, IDC_STOP_SWITCH,         BM_SETCHECK,  cfg_stop_switch         ? BST_CHECKED : BST_UNCHECKED, 0);
				::SendDlgItemMessage(wnd, IDC_SEEK_SWITCH,         BM_SETCHECK,  cfg_seek_switch         ? BST_CHECKED : BST_UNCHECKED, 0);
            } break;
            case WM_COMMAND:
            {
                switch(LOWORD(wp))
                {
                    case IDC_TRACK_CHANGE_MESSAGE:
					{
                        if (HIWORD(wp)==EN_UPDATE)
                        {
                            uGetDlgItemText(wnd, IDC_TRACK_CHANGE_MESSAGE, cfg_track_change_message);
                        }
					} break;
                    case IDC_PAUSE_MESSAGE:
					{
                        if (HIWORD(wp)==EN_UPDATE)
                        {
                            uGetDlgItemText(wnd, IDC_PAUSE_MESSAGE, cfg_pause_message);
                        }
					} break;
                    case IDC_STOP_MESSAGE:
					{
                        if (HIWORD(wp)==EN_UPDATE)
                        {
							uGetDlgItemText(wnd, IDC_STOP_MESSAGE, cfg_stop_message);
                        }
					} break;
                    case IDC_SEEK_MESSAGE:
					{
                        if (HIWORD(wp)==EN_UPDATE)
                        {
                            uGetDlgItemText(wnd, IDC_SEEK_MESSAGE, cfg_seek_message);
                        }
					} break;
                }
            } break;
        }
        return 0;
    }
};

preferences_page_factory_t<config_stalker> config_stalker_factory;
