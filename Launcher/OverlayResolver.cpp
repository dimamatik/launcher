#include "stdafx.h"

#include <shellapi.h>

#include "ConfigurationReader.h"

// Rename overlay file, if exists
bool RenameOverlay(LPWSTR overlay, int buferSize)
{
	if (CheckFileExists(overlay, false) == false) return false;

	LPWSTR overcop = new WCHAR[buferSize];
	wsprintf(overcop, L"%s.copy", overlay);

	BOOL result = MoveFileEx(overlay, overcop,
		MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);

	result = result && CheckFileExists(overcop, false) && CheckFileExists(overlay, false) == false;

	delete[] overcop;

	return result;
}

// Restore overlay file, if a copy exists
bool RestoreOverlay(LPWSTR overlay, int buferSize)
{
	// do not need to restore
	if (CheckFileExists(overlay, false)) return true;

	LPWSTR overcop = new WCHAR[buferSize];
	wsprintf(overcop, L"%s.copy", overlay);

	BOOL result = MoveFileEx(overcop, overlay,
		MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);

	result = result && CheckFileExists(overlay , false) && CheckFileExists(overcop, false) == false;

	delete[] overcop;

	return result;
}