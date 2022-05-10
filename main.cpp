#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

// Global Variables
HANDLE hProc = NULL;
DWORD pID;

bool attachProc(char* procName)
{
	PROCESSENTRY32 procEntry32;

	// Definig the size so we can populate it
	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	// Taking a snaphot of all processes running
	auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to take snapshot of processes!" << std::endl;
		return false;
	}

	while (Process32Next(hProcSnap, &procEntry32)) {
		std::cout << "Looking.. Process name: " << procEntry32.szExeFile << std::endl;

		// If the process we're looking ..
		if (!strcmp(procName, procEntry32.szExeFile))
		{
			std::cout << "Found process " << procEntry32.szExeFile << " with process ID " << procEntry32.th32ProcessID << std::endl;
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			pID = procEntry32.th32ProcessID;

			if (hProc == NULL)
			{
				std::cout << "Failed getting handle to process." << std::endl;
			}

			CloseHandle(hProcSnap);
			return true;
		}
	}
	std::cout << "Couldn't find " << procName << " in the process snapshot" << std::endl;
	CloseHandle(hProcSnap);
	return false;
}

template <class T>
void wpm(T value, DWORD addressToWrite)
{
	WriteProcessMemory(hProc, (PVOID)addressToWrite, &value, sizeof(T), 0);
}

template <class T>
T rpm(DWORD addressToRead)
{
	T rpmBuffer;
	ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(T), 0);
	return rpmBuffer;
}



int main()
{
	DWORD memoryAddress = 0x0304A778;
	
	attachProc((char*)"DIST500-Visualization.exe");
	while (true)
	{
		wpm<int>(27, memoryAddress);
		break;
	}

	system("PAUSE");
	return 0;
}