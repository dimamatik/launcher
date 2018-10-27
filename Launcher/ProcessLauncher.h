#include "stdafx.h"

// Launch game from Steam client and add -fromSteam argument
bool LaunchFromSteam(LPCWSTR steam, int appid, int variant, bool fromCmd);
// Launch game from child process
bool LaunchProcess(LPCWSTR exe, WCHAR* cmdLine, LPCWSTR cwd);
// Launch not from Steam and add -fromCmd argument
bool LaunchIndependent(int variant, int buferSize);