#include "stdafx.h"

#include "ConfigurationReader.h"

// Read general configuration, return error code (<0) or zero
int ReadGeneralConfiguration(LPWSTR iniFile, GeneralConfiguration* configuration, int buferSize)
{
	if (CheckFileExists(iniFile, false) == false)
	{
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_INI_FILE;
	}
	
	memset(configuration->steam, 0x00, buferSize);

	int nums = GetPrivateProfileString(L"General",
		L"Steam",
		NULL,
		configuration->steam,
		buferSize,
		iniFile);

	if (nums <= 0)
	{
		return READ_CONFIGURATION_ERRORS::ERROR_GET_STEAM;
	}

	if (CheckFileExists(configuration->steam, false) == false)
	{
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_STEAM_FILE;
	}

	int apd = GetPrivateProfileInt(L"General", L"AppID", 0, iniFile);

	if (apd <= 0)
	{
		return READ_CONFIGURATION_ERRORS::ERROR_GET_APPID;
	}

	configuration->appid = apd;

	nums = GetPrivateProfileString(L"General",
		L"Overlay",
		NULL,
		configuration->overlay,
		buferSize,
		iniFile);

	if (nums <= 0)
	{
		return READ_CONFIGURATION_ERRORS::ERROR_GET_OVERLAY_FILE;
	}

	return 0;
}

// Read launch configuration by variant number, return error code (<0) or zero
int ReadLaunchConfiguration(LPWSTR iniFile, int variant, LaunchConfiguration* configuration, int buferSize)
{
	if (variant < 0 || variant > 1024)
	{
		return READ_CONFIGURATION_ERRORS::VARIANT_OUT_OF_BOUNDS;
	}

	if (CheckFileExists(iniFile, false) == false)
	{
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_INI_FILE;
	}

	LPWSTR lpAppName = new WCHAR[4];
	wsprintf(lpAppName, L"%d", variant);

#pragma region EXEPATH
	memset(configuration->exePath, 0x00, buferSize);

	DWORD nums;
	nums = GetPrivateProfileString(lpAppName,
		L"FullPath",
		NULL,
		configuration->exePath,
		buferSize,
		iniFile);

	if (nums <= 0)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::ERROR_GET_EXE_PATH;
	}

	if (CheckFileExists(configuration->exePath, false) == false)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_EXE_FILE;
	}
#pragma endregion
#pragma region CWD
	memset(configuration->currentWorkingDirectory, 0x00, buferSize);

	nums = GetPrivateProfileString(lpAppName,
		L"WorkingDirectory",
		NULL,
		configuration->currentWorkingDirectory,
		buferSize,
		iniFile);

	if (nums <= 0)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::ERROR_GET_DIRECTORY;
	}

	if (CheckFileExists(configuration->currentWorkingDirectory, true) == false)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_DIRECTORY;
	}
#pragma endregion
#pragma region OVERLAY
	int ovr = GetPrivateProfileInt(lpAppName,
		L"RemoveOverlay",
		-1,
		iniFile);

	if (ovr < 0 || ovr > 1)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::ERROR_GET_REMOVERLAY;
	}

	configuration->removeOverlay = ovr == 1;
#pragma endregion
#pragma region COUNT
	int cou = GetPrivateProfileInt(lpAppName,
		L"Count",
		-1,
		iniFile);

	if (cou < 0 || cou > 1024)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::SAVE_COUNT_OUT_OF_BOUNDS;
	}

	configuration->backupCount = cou;

	if (cou == 0) // without backups
	{
		delete[] lpAppName;
		return 0;
	}
#pragma endregion
#pragma region SAVE
	memset(configuration->savePath, 0x00, buferSize);

	nums = GetPrivateProfileString(lpAppName,
		L"Save",
		NULL,
		configuration->savePath,
		buferSize,
		iniFile);

	if (nums <= 0)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::ERROR_GET_SAVE_DIRECTORY;
	}

	if (CheckFileExists(configuration->savePath, true) == false)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_SAVE_DIRECTORY;
	}
#pragma endregion
#pragma region BACKUP
	memset(configuration->backupPath, 0x00, buferSize);

	nums = GetPrivateProfileString(lpAppName,
		L"Backup",
		NULL,
		configuration->backupPath,
		buferSize,
		iniFile);

	if (nums <= 0)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::ERROR_GET_BACKUP_DIRECTORY;
	}

	if (CheckFileExists(configuration->backupPath, true) == false)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_BACKUP_DIRECTORY;
	}
#pragma endregion

	delete[] lpAppName;
	return 0;
}


// Check the file or directory is exists
bool CheckFileExists(LPCWSTR path, bool isDir)
{
	DWORD attr = GetFileAttributes(path);
	if (attr == INVALID_FILE_ATTRIBUTES) return false;
	
	if (isDir && ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0)) return false;

	if (isDir == false && ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)) return false;

	return true;
}