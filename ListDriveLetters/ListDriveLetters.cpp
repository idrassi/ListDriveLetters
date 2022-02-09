/*
 Copyright (c) 2022 Mounir IDRASSI (mounir@idrix.fr)

 Governed by the Apache License 2.0 the full text of which is
 contained in the file LICENSE included in the source.
*/

#include <SDKDDKVer.h>
#include <Windows.h>
#include <winnetwk.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>

#pragma comment (lib, "mpr.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	wprintf (L"List used Windows drive letters. (c) 2022 Mounir IDRASSI (mounir@idrix.fr)\n\n");

	DWORD dwUsedDrives = GetLogicalDrives();
	wprintf (L"The call to GetLogicalDrives returned 0x%.8X\n\n", dwUsedDrives);
	wprintf (L"List of used drive letters:\n");
	WCHAR remotePath[512];
	WCHAR drive[3] = {L'A', L':', 0};
	WCHAR rootPath[4] = {L'A', L':', L'\\', 0};
	DWORD dwLen, status;
	for (WCHAR i = 0; i <= 'Z' - 'A'; i++)
	{
		drive[0] = L'A' + i;
		rootPath[0] = L'A' + i;
		if ((dwUsedDrives & (1 << i)))
		{
			wprintf (L" - %s ", drive);
			UINT type = GetDriveType(rootPath);
			switch (type)
			{
			case DRIVE_UNKNOWN: wprintf (L"(Unknown drive type)\n"); break;
			case DRIVE_NO_ROOT_DIR: wprintf (L"(Invalid root path)\n"); break;
			case DRIVE_REMOVABLE: wprintf (L"(Reovable media)\n"); break;
			case DRIVE_FIXED: wprintf (L"(Fixed media)\n"); break;
			case DRIVE_REMOTE: wprintf (L"(Remote network drive)\n"); break;
			case DRIVE_CDROM: wprintf (L"(CD-ROM drive)\n"); break;
			case DRIVE_RAMDISK: wprintf (L"(RAM disk)\n"); break;
			default: wprintf (L"(Unsupported type %d)\n", type); break;
			}
		}
		else
		{
			dwLen = ARRAYSIZE (remotePath);
			status =  WNetGetConnection (drive, remotePath, &dwLen);
			if ((NO_ERROR == status) || (status == ERROR_CONNECTION_UNAVAIL))
			{
				/* this is a mapped network share, mark it as used */
				wprintf (L" - %s (Mapped network share)\n", drive);
			}
		}
	}

	wprintf (L"\n\n");
	wprintf (L"Press any key to exit...");
	while (!_kbhit())
		Sleep (250);
	return 0;
}


