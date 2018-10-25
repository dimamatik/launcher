#include "stdafx.h"
enum PARSE_COMMAND_LINE_ERRORS : int
{
	NOT_ENOUGTH_ARGUMENTS = -1,
	NOT_FOUND_LAUNCH = -2,
	CAN_NOT_PARSE = -3,
};
class CommandLineArguments
{
	public: int variant;
	public: bool fromSteam;
	public: bool fromCmd;
	public: CommandLineArguments() {}
};
// Process command line arguments, return error code (<0) or zero
int ParseCommandLine(LPWSTR* argList, int argCount, CommandLineArguments* arguments);
int SearchTheIndex(const wchar_t* option, LPWSTR* argList, int argCount);