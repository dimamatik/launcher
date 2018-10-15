#include "stdafx.h"

#include "CommandLineParser.h"

// Process command line arguments, return error code (<0) or zero
int ParseCommandLine(LPWSTR* argList, int argCount, int* variant, bool* fromSteam)
{
	// faild to parse command line or bad count of elements
	if (argList == NULL || argCount < 3)
	{
		return PARSE_COMMAND_LINE_ERRORS::NOT_ENOUGTH_ARGUMENTS;
	}

	// search the index of launcher variant
	int index = SearchTheIndex(L"-launch", argList, argCount);

	if (index < 0 || index + 1>= argCount)
	{
		return PARSE_COMMAND_LINE_ERRORS::NOT_FOUND_LAUNCH;
	}

	// need to parse this variant
	int pars = -1;
	if (swscanf_s(argList[index + 1], L"%d", &pars) != 1 || pars < 0)
	{
		return PARSE_COMMAND_LINE_ERRORS::CAN_NOT_PARSE;
	}

	*variant = pars;

	index = SearchTheIndex(L"-fromsteam", argList, argCount);

	*fromSteam = index >= 0;

	return 0;
}

int SearchTheIndex(const wchar_t* option, LPWSTR* argList, int argCount)
{
	int index = -1;

	for (int i = 0; i < argCount && index < 0; i++)
	{
		int size = wcslen(argList[i]) + 1;
		LPWSTR argument = new wchar_t[size];
		wcscpy_s(argument, size, argList[i]);
		_wcslwr_s(argument, size);

		if (wcscmp(argument, option) == 0)
		{
			index = i;
		}

		delete[] argument;
	}

	return index;
}