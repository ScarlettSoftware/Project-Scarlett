#include "injection.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <TlHelp32.h>
#include <fstream>

#include "bytes.h"

bool IsCorrectTargetArchitecture(HANDLE hProc) {
	BOOL bTarget = FALSE;
	if (!IsWow64Process(hProc, &bTarget)) {
		return false;
	}

	BOOL bHost = FALSE;
	IsWow64Process(GetCurrentProcess(), &bHost);

	return (bTarget == bHost);
}

DWORD GetProcessIdByName(const std::wstring& name) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			wchar_t wideExeFileName[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, entry.szExeFile, -1, wideExeFileName, MAX_PATH);
			if (_wcsicmp(wideExeFileName, name.c_str()) == 0) {
				CloseHandle(snapshot);
				return entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    // Games work:

   std::wstring processName = L"cs2.exe";
   //std::wstring processName = L"Pixel Gun 3D.exe"; 
   

    DWORD PID = GetProcessIdByName(processName);

    if (PID == 0) {
        std::cout << "[Scarlett-ldr] Process not found!" << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get(); 
        return -1;
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (!hProc) {
        std::cout << "[Scarlett-ldr] Failed to open process!" << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return -2;
    }

    if (!IsCorrectTargetArchitecture(hProc)) {
        CloseHandle(hProc);
        std::cout << "[Scarlett-ldr] Target architecture mismatch!" << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return -3;
    }

    // Let the magic begin
        if (!ManualMapDll(hProc, dll_main, sizeof(dll_main))) {
      //if (!ManualMapDll(hProc, dll_second, sizeof(dll_second))) { 
        CloseHandle(hProc);
        std::cout << "[Scarlett-ldr] Failed to inject DLL!" << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return -8;
    }

    CloseHandle(hProc);
    std::cout << "[Scarlett-ldr] Scarlett injected successfully!" << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
