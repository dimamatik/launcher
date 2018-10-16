#include "stdafx.h"

// Launch game from Steam client and add -fromSteam argument
bool LaunchFromSteam(LPCWSTR steam, int appid, int variant);
// Launch game from child process
bool LaunchProcess(LPCWSTR exe, WCHAR* cmdLine, LPCWSTR cwd);