#include <iostream>

#include <algorithm>
#include <cctype>
#include <string>

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
		std::cout << procEntry32.szExeFile << std::endl;

		// If the process we're looking ..
		if (!strcmp(procName, procEntry32.szExeFile))
		{
			std::cout << "Found process: " << procEntry32.szExeFile << " with process ID " << procEntry32.th32ProcessID << std::endl;
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

void printInfo()
{
	std::cout << "\n\n------------------------------------------------------------\n";
	std::cout << "Options:\n";
	std::cout << "(R)ead  - (0)Entry / (1) Exit / (A) All Values\n";
	std::cout << "(W)rite - (0)Entry / (1) Exit - Value\n";
	std::cout << "(C)lear\n";
	std::cout << "                                      Burak BUYUKYUKSEL\n";
}



int main()
{
	char processName[]{ "DIST500-Visualization.exe" };
	DWORD entryAddress = 0x0304A778;
	DWORD exitAddress  = 0x0304A898;
	
	/*
	std::cout << "Please enter Entry Address: ";
	std::cin >> entryAddress;

	std::cout << "Please enter Exit Address: ";
	std::cin >> exitAddress;
	*/
	
	attachProc(processName);
	
	system("COLOR A");
	printInfo();
	while (true)
	{

		{
			std::string cmd;
			std::cout << "Command >>";
			std::cin >> cmd;
			std::transform(cmd.begin(), cmd.end(), cmd.begin(),
				[](unsigned char c) { return std::tolower(c); });
			

			if (cmd.rfind("c", 0) == 0) {
				system("CLS && COLOR A");
				printInfo();
			}
			else if (cmd.rfind("ra", 0) == 0)
			{
				int val;
				
				val = rpm<int>(entryAddress);
				std::cout << "Entry: " << val << std::endl;
				val = rpm<int>(exitAddress);
				std::cout << "Exit : " << val << std::endl;
			}
			else if (cmd.rfind("r0", 0) == 0)
			{
				// Read Value from Entry
				auto val = rpm<int>(entryAddress);
				std::cout << "Entry: " << val << std::endl;
			}
			else if (cmd.rfind("r1", 0) == 0)
			{
				// Read Value from Exit
				auto val = rpm<int>(exitAddress);
				std::cout << "Exit : " << val << std::endl;
			}
			else if (cmd.rfind("w0", 0) == 0)
			{
				// Write Value to Entry
				auto value = cmd.substr(2, cmd.length() - 2);
				auto val = std::stoi(value);
				std::cout << "w0 value is : " << val << std::endl;
				wpm<int>(val, entryAddress);
			}
			else if (cmd.rfind("w1", 0) == 0)
			{
				// Write Value to Exit
				auto value = cmd.substr(2, cmd.length() - 2);
				auto val = std::stoi(value);
				std::cout << "w1 value is : " << val << std::endl;
				wpm<int>(val, exitAddress);
			}
		}
	}

	return 0;
}