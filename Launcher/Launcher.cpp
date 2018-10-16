#include "stdafx.h"

#include <wchar.h>
#include <stdio.h>
#include <shellapi.h>

#include "CommandLineParser.h"
#include "ConfigurationReader.h"
#include "ProcessLauncher.h"

const int buferSize = 1024;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Current command line arguments
	WCHAR* cmdLine = GetCommandLine();

	int argCount = -1;
	LPWSTR* argList = CommandLineToArgvW(cmdLine, &argCount);
	
	// Parse launch variant or get error
	int variant = 0;
	int errorCode = 0;
	bool fromSteam = false;
	errorCode = ParseCommandLine(argList, argCount, &variant, &fromSteam);
	if (argList != NULL) LocalFree(argList);

	// Errors...
	if (errorCode < 0)
	{
		switch (errorCode)
		{
		case PARSE_COMMAND_LINE_ERRORS::NOT_ENOUGTH_ARGUMENTS:
			MessageBox(NULL, L"Not enough command line arguments.\n Use '-launch 0', for example, to select variant", NULL, MB_OK);
			break;
		case PARSE_COMMAND_LINE_ERRORS::NOT_FOUND_LAUNCH:
			MessageBox(NULL, L"Can not found '-launch' argument.\n Try '-launch 0'", NULL, MB_OK);
			break;
		case PARSE_COMMAND_LINE_ERRORS::CAN_NOT_PARSE:
			MessageBox(NULL, L"Can not parse '-launch' argument value.\n Try '-launch 0'", NULL, MB_OK);
			break;
		default:
			MessageBox(NULL, L"Unknown error while parsing command line arguments", NULL, MB_OK);
			break;
		}

		return -1;
	}

	// All right, we have a variant for launching
	// Now we are parsing .ini file with options
		
	wchar_t* iniFile = new wchar_t[buferSize];
	GetCurrentDirectory(MAX_PATH, iniFile);
	wcscat_s(iniFile, 1024, L"\\LauncherConfiguration.ini");

	wchar_t* exe = new wchar_t[buferSize];
	wchar_t* cwd = new wchar_t[buferSize];
	wchar_t* steam = new wchar_t[buferSize];
	int appid = -1;

	errorCode = ReadConfiguration(iniFile, variant, exe, cwd, steam, &appid, 1024);
	delete[] iniFile;

	if (errorCode < 0)
	{
		delete[] exe;
		delete[] cwd;
		delete[] steam;

		switch (errorCode)
		{
		case READ_CONFIGURATION_ERRORS::VARIANT_OUT_OF_BOUNDS:
			MessageBox(NULL, L"Variant for launching < 0 or > 1024.\n Try new variant in bounds", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::ERROR_GET_EXE_PATH:
			MessageBox(NULL, L"Can not get variable FullPath", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::ERROR_GET_DIRECTORY:
			MessageBox(NULL, L"Can not get variable WorkingDirectory", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::ERROR_GET_STEAM:
			MessageBox(NULL, L"Can not get Steam.exe path", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::ERROR_GET_APPID:
			MessageBox(NULL, L"Can not get Steam application id", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::NOT_EXIST_DIRECTORY:
			MessageBox(NULL, L"Current Working Directory does not exist", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::NOT_EXIST_EXE_FILE:
			MessageBox(NULL, L"EXE file for launching does not exist", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::NOT_EXIST_INI_FILE:
			MessageBox(NULL, L"LauncherConfiguration.ini does not exist", NULL, MB_OK);
			break;
		case READ_CONFIGURATION_ERRORS::NOT_EXIST_STEAM_FILE:
			MessageBox(NULL, L"Steam.exe does not exist", NULL, MB_OK);
			break;
		default:
			MessageBox(NULL, L"Unknown error while reading configuration file", NULL, MB_OK);
			break;
		}

		return -1;
	}
	
	bool result = false;

	// Launching not from Steam - relaunch with -fromSteam argument and exit
	if (fromSteam == false) result = LaunchFromSteam(steam, appid, variant);
	// Now, launch new proccess and wait...
	else result = LaunchProcess(exe, cmdLine, cwd);
		
	delete[] exe;
	delete[] cwd;
	delete[] steam;

	if (result) return 0;
	else
	{
		if (fromSteam == false) MessageBox(NULL, L"Can not launch from Steam. Unknown error", NULL, MB_OK);
		else MessageBox(NULL, L"Can not launch child process. Unknown error", NULL, MB_OK);
		return -1;
	}
}