#include <iostream>
#include <Windows.h>
#include <stack>
#include <sstream>
using namespace std;

#define THREADS_NUMBER 10
#define PAUSE 2000 /* ms */

DWORD dwCounter = 0;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
	CONST HANDLE hMutex = (CONST HANDLE)lpParam;
	ostringstream stream;
	dwCounter++;
	stream << dwCounter;
	string message = "It's a thread number " + stream.str();
	while(true) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << message << endl;
		ReleaseMutex(hMutex);
		Sleep(PAUSE);
	}
	ExitThread(0);
}

void Error(CONST HANDLE hStdOut, CONST LPCWSTR szMessage) {
	DWORD dwTemp;
	TCHAR szError[256];
	WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);
	wsprintf(szError, TEXT("LastError = %d\r\n"), GetLastError());
	WriteConsole(hStdOut, szError, lstrlen(szError), &dwTemp, NULL);
	ExitProcess(0);
}

int main() {
	TCHAR szMessage[256];
	DWORD dwTemp, i;
	stack<HANDLE> hThreads;
	CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONST HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
	if (NULL == hMutex) {
		Error(hStdOut, TEXT("Failed to create mutex.\r\n"));
	}
	while (true)
	{
		cout << "Choose the variant:" << endl;
		cout << "'+' - Add thread" << endl;
		cout << "'-' - Delete thread" << endl;
		cout << "'q' - Exit the programm" << endl;
		rewind(stdin);
		cin.clear();
		char choose = cin.get();
		switch (choose)
		{
		case '+':
		{
			HANDLE new_thread = CreateThread(NULL, 0, &ThreadProc, hMutex, 0, NULL);
			if (!new_thread) {
				Error(hStdOut, TEXT("Failed to create thread.\r\n"));
			}
			hThreads.push(new_thread);
			break;
		}
		case '-':
		{
			if (!hThreads.empty()) {
				dwCounter--;
				::TerminateThread(hThreads.top(), NO_ERROR);
				hThreads.pop();
			}
			else {
				cout << "Error, there are no threads!" << endl;
			}
			break;
		}
		case 'q':
		{
			while (!hThreads.empty()) {
				::TerminateThread(hThreads.top(), NO_ERROR);
				hThreads.pop();
			}

			CloseHandle(hMutex);
			ExitProcess(0);
		}
		default:
			cout << "Invalid input! Please try again!" << endl;
			break;
		}
	}
	CloseHandle(hMutex);
	ExitProcess(0);
}