#include <windows.h>
#include <iostream>
#include <string>

int main() {
    
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;  // 
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        std::cerr << "Не вдалося створити канал!" << std::endl;
        return 1;
    }

    
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = hReadPipe;    
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); 
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);   
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    
    if (!CreateProcess(NULL, 
        (LPSTR)"child.exe",   
        NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Не вдалося створити дочірній процес!" << std::endl;
        return 1;
    }

    // Закриття непотрібних ручок у батьківському процесі
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Запис даних у канал
    const char* data = "Привіт з батьківського процесу!";
    DWORD bytesWritten;
    if (!WriteFile(hWritePipe, data, strlen(data), &bytesWritten, NULL)) {
        std::cerr << "Не вдалося записати в канал!" << std::endl;
        return 1;
    }

    
    CloseHandle(hWritePipe);

    
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Закриття всіх ручок
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
