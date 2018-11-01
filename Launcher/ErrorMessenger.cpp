#include "stdafx.h"

#include "CommandLineParser.h"
#include "ConfigurationReader.h"
#include "ProcessLauncher.h"

bool CheckCommandLineParseError(int errorCode)
{
	if (errorCode >= 0) return false;

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

	return true;
}

bool CheckReadConfigurationError(int errorCode)
{
	if (errorCode >= 0) return false;
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
	case READ_CONFIGURATION_ERRORS::ERROR_GET_REMOVERLAY:
		MessageBox(NULL, L"Can not get variable RemoveOverlay.\n Try RemoveOverlay=0", NULL, MB_OK);
		break;
	case READ_CONFIGURATION_ERRORS::ERROR_GET_OVERLAY_FILE:
		MessageBox(NULL, L"Can not get variable Overlay", NULL, MB_OK);
		break;
	case READ_CONFIGURATION_ERRORS::SAVE_COUNT_OUT_OF_BOUNDS:
		MessageBox(NULL, L"Count of backups <0 or >1024.\n Try Count=0 without backups", NULL, MB_OK);
		break;
	case READ_CONFIGURATION_ERRORS::NOT_EXIST_SAVE_DIRECTORY:
		MessageBox(NULL, L"Save directory does not exist", NULL, MB_OK);
		break;
	case READ_CONFIGURATION_ERRORS::NOT_EXIST_BACKUP_DIRECTORY:
		MessageBox(NULL, L"Directory for backups does not exist", NULL, MB_OK);
		break;
	case READ_CONFIGURATION_ERRORS::ERROR_GET_SAVE_DIRECTORY:
		MessageBox(NULL, L"Can not get variable Save", NULL, MB_OK);
		break;
	case READ_CONFIGURATION_ERRORS::ERROR_GET_BACKUP_DIRECTORY:
		MessageBox(NULL, L"Can not get variable Backup", NULL, MB_OK);
		break;

	default:
		MessageBox(NULL, L"Unknown error while reading configuration file", NULL, MB_OK);
		break;
	}

	return true;
}