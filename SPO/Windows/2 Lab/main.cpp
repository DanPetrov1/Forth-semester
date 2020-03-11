#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <Windows.h>
#include <list>
#include <tchar.h>

using namespace std;

const int DELAY = 100;
const int BUFF_SIZE = 10;
const int MAIN_DELAY = 500;
const int MAIN_PROCESS_ARGS = 1;

HANDLE createNewProcess(int procNumber);

int main() {

	cout << "Create new process: +" << endl;
	cout << "Kill last process: -" << endl;
	cout << "Exit the programm: q" << endl;

	HANDLE writeEvent = CreateEvent(NULL, FALSE, true, LPCWSTR("writeEvent"));

	list <HANDLE> processList;

	while (true) {

		WaitForSingleObject(writeEvent, INFINITE);
		cout << endl;

		char temp;
		do {
			cin.clear();
			cin.get(temp);
		} while ((temp != '+') && (temp != '-') && (temp != 'q'));

		switch (temp) {
		case '+':
			processList.push_front(createNewProcess(processList.size() + 1));
			break;
		case '-':
			if (!processList.empty()) {
				TerminateProcess(processList.front(), EXIT_SUCCESS);
				CloseHandle(processList.front());
				processList.pop_front();
			}
			else {
				cout << "List is empty, create process please" << endl;
			}
			break;
		case 'q':
			while (!processList.empty()) {
				TerminateProcess(processList.front(), EXIT_SUCCESS);
				CloseHandle(processList.front());
				processList.pop_front();
			}

			CloseHandle(writeEvent);
			return 0;
		}

		SetEvent(writeEvent);
		Sleep(MAIN_DELAY);
	}
}

HANDLE createNewProcess(int procNumber) {
	string commandArgs = string("ChildProcess ");
	char* buffer = new char[10];
	_itoa(procNumber, buffer, 10);
	commandArgs += buffer;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	TCHAR str[] = TEXT("C:\\Users\\Dan\\source\\repos\\LabSPOCM2\\Debug\\writeEvent.exe");
	TCHAR *command = new TCHAR[commandArgs.length() + 1];
	mbstowcs_s(NULL, command, _tcslen(command), commandArgs.c_str(), commandArgs.length());
	if (!CreateProcess(str,//вызвать второй проект с дочерним процессом
		command,// передать аргументы командной строки
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)
		) {
		cout << "Error while creating new process!" << endl;
		Sleep(MAIN_DELAY);
		exit(EXIT_FAILURE);
	}

	return pi.hProcess;
}
