#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>

using namespace std;

const int kMaxSize = 100;
const DWORD kDwMaximumSizeLow = 0;
const DWORD kDwMaximumSizeHigh = kMaxSize;


void main(int argc, char* argv[]) {
	HANDLE Work = CreateSemaphore(
		/*������� lpSecurityDescriptor ��������� ��������� ���������� ������������ ��� ������ ��������.
 ���� ���� �������� ����� NULL, ������� �������� ���������� ������������ �� ���������.*/
		NULL,
		/*��������� ���� ��� ������� ��������. ��� �������� ������ ���� ������ ��� ����� ���� � ������ ��� ����� lMaximumCount.
 ��������� �������� ���������������, ����� ��� ���������� ������ ���� � �� ����� ��������, ����� ��� ����� ����.
 ���� ����������� �� �������, ����� ������� �������� ����������� �����, ��������� �������.
 ���� ������������� �� ��������� ����������, ������� ������� ReleaseSemaphore.*/
		0,
		/*������������ ������� ��� ������� ��������. ��� �������� ������ ���� ������ ����.*/
		1,
		/*���� lpName ������������� ����� ������������� ������� ��������, ��� ������� ����������� ����� ������� SEMAPHORE_ALL_ACCESS.
 � ���� ������ ��������� lInitialCount � lMaximumCount ������������, ��������� ��� ��� ���� ����������� ��������� ��������.
 ���� �������� lpSemaphoreAttributes �� ����� NULL, �� ����������, ����� �� ����������� ����������, �� ��� ������� ����������� ������������ ������������.*/
		TEXT("Work"));

	HANDLE Close = CreateSemaphore(NULL, 0, 1, TEXT("Close"));

	HANDLE WINAPI FileProjection = CreateFileMapping(
		/*���������� �����, �� �������� ����� ������� ������ ������������� ������.
 ���� hFile �������� INVALID_HANDLE_VALUE, ���������� ������� ������ ����� ������� ������ ��� ������� ������������� ������
 � ���������� dwMaximumSizeHigh � dwMaximumSizeLow. � ���� ������ CreateFileMapping ������� ������ ������������� ������
 ���������� �������, ������� �������������� ������ �������� �������, � �� ������ � �������� �������.*/
		INVALID_HANDLE_VALUE,
		/*��������� �� ��������� SECURITY_ATTRIBUTES, ������� ����������, ����� �� ������������ ���������� ������������� ��������� ����������.
 ���� lpSecurityDescriptor ��������� SECURITY_ATTRIBUTES ��������� ���������� ������������ ��� ������ ������� ������������� ������.
 ���� lpAttributes NULL, ���������� �� ����� ���� �����������, � ������ ������������� ������ �������� ���������� ������������ �� ���������.*/
		NULL,
		/*��������� ������ �������� ������� ������������� ������. ��� �������������� ���� ������� ������ ���� ���������� � ���� �������.
 PAGE_READWRITE ��������� ���������� ������������� ��� ������ ������ ��� ������, ����������� �� ������ ��� ������� ��� ������ / ������.*/
		PAGE_READWRITE,
		/*DWORD ������������� ������� ������� ������������� ������.*/
		kDwMaximumSizeLow,
		/*����������� �������� DWORD ��� ������������� ������� ������� ������������� ������.
		���� ���� �������� � dwMaximumSizeHigh ����� 0 (����), ������������ ������ ������� ������������� ������ ����� �������� ������� �����,
		������� hFile ��������������.*/
		kDwMaximumSizeHigh,
		/*��� ������� ������������� ������.
		���� ���� �������� ������������� ����� ������������� ������� �������������,
		������� ����������� ������ � ������� � �������, ������� ���������� ������� flProtect.*/
		TEXT("FileProjection"));

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;

	TCHAR command[] = TEXT("Client");

	if (!CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "Create Process failed" << GetLastError() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	LPVOID memoryMap;
	memoryMap = MapViewOfFile(FileProjection, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	char* stringRepresentation = (char*)memoryMap;

	int currPos = 0;
	bool readyForInput = true;
	string buffString;
	buffString.resize(kMaxSize, '\0');

	while (true)
	{
		if (readyForInput)
		{
			currPos = 0;
			cout << "Server: Please, enter the string" << endl;
			getline(cin, buffString);
			readyForInput = false;
		}

		string tempBuff;
		int newLength = 0;
		tempBuff.append(buffString, 0, kMaxSize - 1);
		currPos = tempBuff.length();

		strcpy(stringRepresentation, const_cast<char*>(tempBuff.c_str()));
		stringRepresentation[tempBuff.length()] = '\0';

		tempBuff.clear();
		newLength = buffString.length() - currPos;
		if (newLength > 0)
		{
			tempBuff.append(buffString, currPos, newLength);
		}
		buffString = tempBuff;

		ReleaseSemaphore(Work, 1, NULL);
		WaitForSingleObject(Work, INFINITE);

		if (buffString.empty())
		{
			readyForInput = true;
			cout << "\nExit (0)" << endl;
			if (cin.get() == '0')
			{
				ReleaseSemaphore(Close, 1, NULL);	//����������� ���������� ���������� ������� �������� �� ������������ ��������.
				ReleaseSemaphore(Work, 1, NULL);
				WaitForSingleObject(pi.hProcess, INFINITE);
				UnmapViewOfFile(memoryMap);			//��������� �������������� ��� ����� �� ��������� ������������ ����������� ��������.
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				CloseHandle(Close);
				CloseHandle(Work);
				CloseHandle(FileProjection);
				return;
			}

			buffString.clear();

			cin.clear();
			cout << endl;
		}
	}
}

