#include "stdafx.h"
enum READ_CONFIGURATION_ERRORS : int
{
	VARIANT_OUT_OF_BOUNDS = -1,
	ERROR_GET_EXE_PATH = -2,
	ERROR_GET_DIRECTORY = -3,
	ERROR_GET_STEAM = -4,
	ERROR_GET_APPID = -5,
	NOT_EXIST_INI_FILE = -6,
	NOT_EXIST_EXE_FILE = -7,
	NOT_EXIST_DIRECTORY = -8,
	NOT_EXIST_STEAM_FILE = -9,
	ERROR_GET_REMOVERLAY = -10,
	ERROR_GET_OVERLAY_FILE = -11,
	SAVE_COUNT_OUT_OF_BOUNDS = -12,
	NOT_EXIST_SAVE_DIRECTORY = -13,
	NOT_EXIST_BACKUP_DIRECTORY = -14,
	ERROR_GET_SAVE_DIRECTORY = -15,
	ERROR_GET_BACKUP_DIRECTORY = -16,
};
class GeneralConfiguration
{
	public: LPWSTR steam;
	public: LPWSTR overlay;
	public: int appid;

	public: GeneralConfiguration(int buferSize)
	{
		steam = new WCHAR[buferSize];
		overlay = new WCHAR[buferSize];
	}
	~GeneralConfiguration()
	{
		delete[] steam;
		delete[] overlay;
	}

};
class LaunchConfiguration
{
	public: LPWSTR exePath;
	public: LPWSTR currentWorkingDirectory;
	public: bool removeOverlay;

	public: LPWSTR savePath;
	public: LPWSTR backupPath;
	public: int backupCount;

	public: LaunchConfiguration(int buferSize)
	{
		exePath = new WCHAR[buferSize];
		currentWorkingDirectory = new WCHAR[buferSize];
		savePath = new WCHAR[buferSize];
		backupPath = new WCHAR[buferSize];
	}
	~LaunchConfiguration()
	{
		delete[] exePath;
		delete[] currentWorkingDirectory;
		delete[] savePath;
		delete[] backupPath;
	}
};

// Read general configuration, return error code (<0) or zero
int ReadGeneralConfiguration(LPWSTR iniFile, GeneralConfiguration* configuration, int buferSize);
// Read launch configuration by variant number, return error code (<0) or zero
int ReadLaunchConfiguration(LPWSTR iniFile, int variant, LaunchConfiguration* configuration, int buferSize);

// Check the file or directory is exists
bool CheckFileExists(LPCWSTR path, bool isDir);