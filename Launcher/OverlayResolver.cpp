#include "stdafx.h"

#include <shellapi.h>

#include "ConfigurationReader.h"

// Rename overlay file, if exists
bool RenameOverlay(LPWSTR overlay, int buferSize)
{
	if (CheckFileExists(overlay, false) == false) return false;

	// Can not use "standart" c++ functions, only by shell...

	LPWSTR bufer = new wchar_t[buferSize];
	LPWSTR overcop = new wchar_t[buferSize];

	wsprintf(overcop, L"%s.copy", overlay);
	wsprintf(bufer, L"/K \"copy /Y \"%s\" \"%s\"\"", overlay, overcop);


	HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", bufer, NULL, SW_NORMAL);

	if ((int)result <= 32 || CheckFileExists(overcop, false) == false)
	{
		delete[] bufer;
		delete[] overcop;
		return false;
	}
	delete[] overcop;

	wsprintf(bufer, L"/K \"del /F /Q \"%s\" \"", overlay);

	result = ShellExecute(NULL, L"open", L"cmd.exe", bufer, NULL, SW_NORMAL);
	delete[] bufer;

	if ((int)result <= 32)
	{
		return false;
	}

	if (CheckFileExists(overlay, false)) return false;
	else return true;
}

// Restore overlay file, if a copy exists
bool RestoreOverlay(LPWSTR overlay, int buferSize)
{
	if (CheckFileExists(overlay, false)) return false;

	wchar_t* overcop = new wchar_t[buferSize + 256];
	wsprintf(overcop, L"%s.copy", overlay);

	if (CheckFileExists(overcop, false) == false)
	{
		delete[] overcop;
		return false;
	}

	if (CopyFile(overcop, overlay, false) == false || CheckFileExists(overlay, false) == false)
	{
		delete[] overcop;
		return false;
	}

	if (DeleteFile(overcop) == false)
	{
		delete[] overcop;
		return false;
	}
	else
	{
		delete[] overcop;
		return true;
	}
}