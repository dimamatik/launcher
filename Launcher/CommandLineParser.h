#include "stdafx.h"
enum PARSE_COMMAND_LINE_ERRORS : int
{
	NOT_ENOUGTH_ARGUMENTS = -1,
	NOT_FOUND_LAUNCH = -2,
	CAN_NOT_PARSE = -3,
};
// Process command line arguments, return error code (<0) or zero
int ParseCommandLine(LPWSTR* argList, int argCount, int* variant, bool* fromSteam);
int SearchTheIndex(const wchar_t* option, LPWSTR* argList, int argCount);