#include "stdafx.h"

#include <shellapi.h>

#include "ConfigurationReader.h"

// Create a new backup folder with saves and remove old backup
bool Backup(LPWSTR save, LPWSTR backup, int count, int buferSize)
{
	if (count <= 0 || count > 1024) return false;
	
	LPWSTR bufer = new WCHAR[buferSize * 32];

	wsprintf(bufer, L"/E:ON /V:ON \"cmd /C "
					 "SET COUNT=%d && "
					 "SET /A REMD=!COUNT!-1 && "

					 "(IF /I !REMD! LSS 1000 (SET REMD=0!REMD!)) && "
					 "(IF /I !REMD! LSS 100  (SET REMD=0!REMD!)) && "
					 "(IF /I !REMD! LSS 10   (SET REMD=0!REMD!)) && "

					 "(IF EXIST \"%s\\Backup_!REMD!\" (RMDIR /S /Q \"%s\\Backup_!REMD!\")) && "

					 "SET /A MAX=!COUNT!-2 && "
					
					 "(FOR /L %%k IN (!MAX!, -1, 0) DO ("
						"SET /A SUFFIX=%%k && "
						"(IF /I !SUFFIX! LSS 1000 (SET SUFFIX=0!SUFFIX!)) && "
						"(IF /I !SUFFIX! LSS 100  (SET SUFFIX=0!SUFFIX!)) && "
						"(IF /I !SUFFIX! LSS 10   (SET SUFFIX=0!SUFFIX!)) && "
						"SET NAME=%s\\Backup_!SUFFIX! && "
						"SET POSFIX=!REMD! && "
						"SET LAME=Backup_!POSFIX! && "
						"(IF EXIST \"!NAME!\" (REN \"!NAME!\" \"!LAME!\")) && "
						"SET REMD=!SUFFIX!"
					 ")) && "
					 "MKDIR \"%s\\Backup_0000\" && "
					 "XCOPY /E /H \"%s\" \"%s\\Backup_0000\" && "
					 "TIMEOUT 5 && "
					 "(IF !ERRORLEVEL! NEQ 0 (PAUSE && PAUSE)) "// \" not need 
		,count, backup, backup, backup, backup, save, backup
	);

	HINSTANCE result = ShellExecute(NULL, L"open", L"cmd.exe", bufer, NULL, SW_NORMAL);
	
	delete[] bufer;

	return (int)result > 32;
}