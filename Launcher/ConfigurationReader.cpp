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

	int apd = GetPrivateProfileInt(L"General", L"AppID", 0, iniFile);

	if (apd <= 0)
	{
		return READ_CONFIGURATION_ERRORS::ERROR_GET_APPID;
	}

	*appid = apd;

	return 0;
}