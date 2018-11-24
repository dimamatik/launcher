#include "stdafx.h"

#include <wchar.h>
#include <stdio.h>
#include <shellapi.h>

#include "CommandLineParser.h"
#include "ConfigurationReader.h"
#include "ProcessLauncher.h"
#include "OverlayResolver.h"
#include "ErrorMessenger.h"
#include "SaveBackuper.h"

const int buferSize = 1024;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Current command line arguments
	WCHAR* cmdLine = GetCommandLine();

	int argCount = -1;
	LPWSTR* argList = CommandLineToArgvW(cmdLine, &argCount);
	
	// Parse launch variant or get error
	CommandLineArguments* arguments = new CommandLineArguments();

	// Errors...
	if (CheckCommandLineParseError(ParseCommandLine(argList, argCount, arguments)))
	{
		LocalFree(argList);
		delete arguments;
		return -1;
	}
	LocalFree(argList);

	// All right, we have a variant for launching
	// Now we are parsing .ini file with options
		
	LPWSTR iniFile = new wchar_t[buferSize];
	GetCurrentDirectory(MAX_PATH, iniFile);
	wcscat_s(iniFile, buferSize, L"\\LauncherConfiguration.ini");

	GeneralConfiguration* generalConfiguration = new GeneralConfiguration(buferSize);
	LaunchConfiguration* launchConfiguration = new LaunchConfiguration(buferSize);

	if (CheckReadConfigurationError(ReadGeneralConfiguration(iniFile, generalConfiguration, buferSize)) ||
		CheckReadConfigurationError(ReadLaunchConfiguration(iniFile, arguments->variant, launchConfiguration, buferSize)))
	{
		delete[] iniFile;

		delete arguments;
		delete generalConfiguration;
		delete launchConfiguration;

		return -1;
	}
	delete[] iniFile;

	bool result = false;
	bool resultOverlay = true;
	bool backup = true;

	if (launchConfiguration->removeOverlay && CheckFileExists(generalConfiguration->overlay, false))
	{
		// relaunch not from the Steam
		if (arguments->fromCmd == false)
		{
			result = LaunchIndependent(arguments->variant, buferSize);
		}
		// remove overlay file and launch from Steam
		else
		{
			// make a backup
			if (launchConfiguration->backupCount > 0)
			{
				backup = Backup(launchConfiguration->savePath, launchConfiguration->backupPath, launchConfiguration->backupCount, buferSize);
			}
			if (backup == false)
			{
				MessageBox(NULL, L"Can not create save backup", NULL, MB_OK);
			}
			// pause, wait closing and free overlay
			Sleep(1500);
			resultOverlay = RenameOverlay(generalConfiguration->overlay, buferSize);
			result = resultOverlay && LaunchFromSteam(generalConfiguration->steam, generalConfiguration->appid, arguments->variant, arguments->fromCmd);
			// wait until game launching
			Sleep(1500);
			resultOverlay = result && RestoreOverlay(generalConfiguration->overlay, buferSize);
		}
	}
	// Launching not from Steam - relaunch with -fromSteam argument and exit
	else if (arguments->fromSteam == false)
	{
		result = LaunchFromSteam(generalConfiguration->steam, generalConfiguration->appid, arguments->variant, arguments->fromCmd);
	}
	// Now, launch new proccess and wait...
	else
	{
		// make a backup
		if (launchConfiguration->backupCount > 0)
		{
			backup = Backup(launchConfiguration->savePath, launchConfiguration->backupPath, launchConfiguration->backupCount, buferSize);
		}
		if (backup == false)
		{
			MessageBox(NULL, L"Can not create save backup", NULL, MB_OK);
		}
		result = LaunchProcess(launchConfiguration->exePath, cmdLine, launchConfiguration->currentWorkingDirectory);
	}
		
	delete generalConfiguration;
	delete launchConfiguration;

	if (result && resultOverlay)
	{
		delete arguments;
		return backup ? 0 : -1;
	}
	else
	{
		if (resultOverlay)
		{
			if (arguments->fromSteam == false) MessageBox(NULL, L"Can not launch from Steam. Unknown error", NULL, MB_OK);
			else MessageBox(NULL, L"Can not launch child process. Unknown error", NULL, MB_OK);
		}
		else if (result)
		{
			MessageBox(NULL, L"Can not restore Steam Overlay DLL", NULL, MB_OK);
		}
		else
		{
			MessageBox(NULL, L"Can not remove Steam Overlay DLL", NULL, MB_OK);
		}
		delete arguments;
		return -1;
	}
}