Read and write the process memory
The memory is allocated in things called "pages" which re essentially blocks of memory on your RAM
1 page is usually around 4kb long
These pages of memory are then organised into sections

Algorithm
1. Loop through the process list
2. Find the program's process
3. Once we find the process we can "open" it by setting our process access rights to PROCESS_ALL_ACCESS meaning we can read and write all the process memory.


BOOL WINAPI ReadProcessMemory(
	_In_ HANDLE hProcess,
	_In_ LPCVOID lpBaseAddress,
	_Out_ LPVOID lpBuffer,
	_In_ SIZE_T nSize,
	_Out_ SIZE_T *lpNumberOfBytesRead
);

BOOL WINAPI WriteProcessMemory(
	_In_ HANDLE hProcess,
	_In_ LPCVOID lpBaseAddress,
	_In_ LPVOID lpBuffer,
	_In_ SIZE_T nSize,
	_Out_ SIZE_T *lpNumberOfBytesWritten
);

1. Make a program to attach to the process
2. Give our program all access to the process
3. Read and write memory to the opened process
