#include "stdafx.h"
enum READ_CONFIGURATION_ERRORS : int
{
	VARIANT_OUT_OF_BOUNDS = -1,
	ERROR_GET_EXE_PATH = -2,
	ERROR_GET_DIRECTORY = -3,
	ERROR_GET_STEAM = -4,
	ERROR_GET_APPID = -5,
};

// Read configuration by variant number, return error code (<0) or zero
int ReadConfiguration(LPCWSTR iniFile, int variant,
						wchar_t* exePath,
						wchar_t* currentWorkingDirectory,
						wchar_t* steam,
						int* appid,
						int buferSize);

