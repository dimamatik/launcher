#include "stdafx.h"

#include <shellapi.h>

#include "ConfigurationReader.h"

// Create a new backup folder with saves and remove old backup
bool Backup(LPWSTR save, LPWSTR backup, int count, int buferSize)
{
	if (count <= 0) return false;
	
	LPWSTR folder = new WCHAR[buferSize];
	LPWSTR cmd = new WCHAR[buferSize];

	wsprintf(folder, L"%s\\Backup_%04d", backup, count - 1);

	// remove last backup
	if (CheckFileExists(folder, true))
	{
		wsprintf(cmd, L"/C rmdir /S /Q \"%s\"", folder);

		HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", cmd, NULL, SW_HIDE);
		if ((int)result <= 32)
		{
			delete[] folder;
			delete[] cmd;
			return false;
		}
	}
	// rename old backups
	for (int i = count - 2; i >= 0; i--)
	{
		wsprintf(folder, L"%s\\Backup_%04d", backup, i);

		if (CheckFileExists(folder, true) == false) continue;
	
		wsprintf(cmd, L"/C ren \"%s\" Backup_%04d", folder, i + 1);

		HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", cmd, NULL, SW_HIDE);

		if ((int)result <= 32)
		{
			delete[] folder;
			delete[] cmd;
			return false;
		}
	}
	
	wsprintf(folder, L"%s\\Backup_%04d", backup, 0);
	// create new backup directory
	{
		wsprintf(cmd, L"/C mkdir \"%s\"", folder);
		HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", cmd, NULL, SW_HIDE);
		if ((int)result <= 32)
		{
			delete[] folder;
			delete[] cmd;
			return false;
		}
	}
	// copy save files
	{
		wsprintf(cmd, L"/C xcopy /E /H \"%s\" \"%s\"", save, folder);

		HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", cmd, NULL, SW_HIDE);

		delete[] folder;
		delete[] cmd;
		return (int)result > 32;
	}
}