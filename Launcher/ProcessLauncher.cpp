#include "stdafx.h"

#include <shellapi.h>

// Launch game from Steam client and add -fromSteam argument
bool LaunchFromSteam(LPCWSTR steam, int appid, int variant, bool fromCmd)
{
	LPWSTR bufer = new wchar_t[wcslen(steam) + 256];
	if (fromCmd)
		wsprintf(bufer, L"/C \"\"%s\" -applaunch %d -fromSteam -launch %d -fromCmd\"", steam, appid, variant);
	else
		wsprintf(bufer, L"/C \"\"%s\" -applaunch %d -fromSteam -launch %d\"", steam, appid, variant);

	HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", bufer, NULL, SW_HIDE);

	delete[] bufer;

	return (int)result > 32;
}
// Launch game from child process
bool LaunchProcess(LPCWSTR exe, WCHAR* cmdLine, LPCWSTR cwd)
{
	STARTUPINFO si = { sizeof(STARTUPINFO), 0 };
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = { 0 };

	if (CreateProcess(exe, cmdLine, NULL, NULL, FALSE, 0, NULL, cwd, &si, &pi))
	{
		// wait for wrapped exe to exit
		WaitForSingleObject(pi.hProcess, INFINITE);
		// close the process and thread object handles
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	}
	else return false;
}
// Launch not from Steam and add -fromCmd argument
bool LaunchIndependent(int variant, int buferSize)
{
	LPWSTR launcher = new WCHAR[buferSize];

	if (GetModuleFileName(NULL, launcher, buferSize) == 0)
	{
		delete[] launcher;
		return false;
	}

	LPWSTR cmdLine = new WCHAR[buferSize];
	wsprintf(cmdLine, L"/C \"%s\" -launch %d -fromCmd", launcher, variant);

	HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", cmdLine, NULL, SW_HIDE);

	delete[] launcher;
	delete[] cmdLine;

	return (int)result > 32;
}