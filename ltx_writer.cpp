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


#include "ltx_writer.h"

#include <conio.h>
#include <Psapi.h>
#include <strsafe.h>
#include <shlwapi.h>

using namespace foo_stalker;

//--------------------------
// ltx_writer implementation
//--------------------------

ltx_writer::ltx_writer()
	: ltx_file(INVALID_HANDLE_VALUE)
{
}

ltx_writer::~ltx_writer()
{
	close();
}

void ltx_writer::open(HWND hwnd, const TCHAR * config_folder)
{
	insync2(cs);
	close();
	ltx_file = OpenLtxFile(hwnd, config_folder);
}

void ltx_writer::close()
{
	insync2(cs);
	if (INVALID_HANDLE_VALUE != ltx_file)
	{
		::SetFilePointer
			( ltx_file   // hFile
			, 0          // lDistanceToMove
			, NULL       // lpDistanceToMoveHigh
			, FILE_BEGIN // dwMoveMethod
			);
		::SetEndOfFile(ltx_file);
		::CloseHandle(ltx_file);
		ltx_file = INVALID_HANDLE_VALUE;
	}
}

void ltx_writer::reset()
{
	insync2(cs);
	::SetFilePointer
		( ltx_file   // hFile
		, 0          // lDistanceToMove
		, NULL       // lpDistanceToMoveHigh
		, FILE_BEGIN // dwMoveMethod
		);
	::SetEndOfFile(ltx_file);
}

void ltx_writer::write(pfc::string8 text)
{
	insync2(cs);
	if (INVALID_HANDLE_VALUE == ltx_file)
	{
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

HANDLE ltx_writer::OpenLtxFile(HWND hwnd, const TCHAR * config_folder)
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

	::PathCombine(path, path, config_folder);
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

foo_stalker::ltx_writer &
foo_stalker::ltx_writer::get_instance()
{
	static ltx_writer ltx_writer;
	return ltx_writer;
}
