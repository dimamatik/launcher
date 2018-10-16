#include "stdafx.h"

#include "ConfigurationReader.h"

// Read configuration by variant number, return error code (<0) or zero
int ReadConfiguration(LPCWSTR iniFile, int variant, wchar_t* exePath, wchar_t* currentWorkingDirectory, 
						wchar_t* steam, int* appid,	int buferSize)
{
	if (variant < 0 || variant > 1024)
	{
		return READ_CONFIGURATION_ERRORS::VARIANT_OUT_OF_BOUNDS;
	}

	if (CheckFileExists(iniFile, false))
	{
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_INI_FILE;
	}

	wchar_t* lpAppName = new wchar_t[4];
	wsprintf(lpAppName, L"%d", variant);

	memset(exePath, 0x00, buferSize);

	DWORD nums;
	nums = GetPrivateProfileString( lpAppName,
									L"FullPath", 
									NULL, 
									exePath,
									buferSize,
									iniFile);

	if (nums <= 0)
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::ERROR_GET_EXE_PATH;
	}

	if (CheckFileExists(exePath, false))
	{
		delete[] lpAppName;
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_EXE_FILE;
	}

	memset(currentWorkingDirectory, 0x00, buferSize);

	nums = GetPrivateProfileString(lpAppName,
		L"WorkingDirectory",
		NULL,
		currentWorkingDirectory,
		buferSize,
		iniFile);

	delete[] lpAppName;

	if (nums <= 0)
	{
		return READ_CONFIGURATION_ERRORS::ERROR_GET_DIRECTORY;
	}

	if (CheckFileExists(currentWorkingDirectory, true))
	{
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_DIRECTORY;
	}

	memset(steam, 0x00, buferSize);

	nums = GetPrivateProfileString(L"General",
		L"Steam",
		NULL,
		steam,
		buferSize,
		iniFile);

	if (nums <= 0)
	{
		return READ_CONFIGURATION_ERRORS::ERROR_GET_STEAM;
	}

	if (CheckFileExists(steam, false))
	{
		return READ_CONFIGURATION_ERRORS::NOT_EXIST_STEAM_FILE;
	}

	int apd = GetPrivateProfileInt(L"General", L"AppID", 0, iniFile);

	if (apd <= 0)
	{
		return READ_CONFIGURATION_ERRORS::ERROR_GET_APPID;
	}

	*appid = apd;

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