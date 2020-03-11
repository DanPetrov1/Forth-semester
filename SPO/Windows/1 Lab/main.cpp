#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <iostream>

int main()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory (&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
    time_t rawtime;
    struct tm* timeinfo;
    TCHAR CommandLine[] = TEXT("sleep");
    if (!CreateProcess(NULL,//вызвать второй проект с дочерним процессом
        CommandLine,// передать аргументы командной строки
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi)
        ) {
        printf("CreateProcess failed");
    }
	printf("Descr: %d\n", pi.hProcess);
	printf("ID: %d\n", pi.dwProcessId);

	WaitForSingleObject(pi.hProcess, INFINITE);
    printf("End of dautcher process.\n");

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current time and date: %s", asctime(timeinfo));
    system("pause");
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}