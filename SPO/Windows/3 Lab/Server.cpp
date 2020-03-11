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
		/*Элемент lpSecurityDescriptor структуры указывает дескриптор безопасности для нового семафора.
 Если этот параметр равен NULL, семафор получает дескриптор безопасности по умолчанию.*/
		NULL,
		/*Начальный счет для объекта семафора. Это значение должно быть больше или равно нулю и меньше или равно lMaximumCount.
 Состояние семафора сигнализируется, когда его количество больше нуля и не имеет значения, когда оно равно нулю.
 Счет уменьшается на единицу, когда функция ожидания освобождает поток, ожидающий семафор.
 Счет увеличивается на указанное количество, вызывая функцию ReleaseSemaphore.*/
		0,
		/*Максимальный счетчик для объекта семафора. Это значение должно быть больше нуля.*/
		1,
		/*Если lpName соответствует имени существующего объекта семафора, эта функция запрашивает право доступа SEMAPHORE_ALL_ACCESS.
 В этом случае параметры lInitialCount и lMaximumCount игнорируются, поскольку они уже были установлены процессом создания.
 Если параметр lpSemaphoreAttributes не равен NULL, он определяет, можно ли наследовать дескриптор, но его элемент дескриптора безопасности игнорируется.*/
		TEXT("Work"));

	HANDLE Close = CreateSemaphore(NULL, 0, 1, TEXT("Close"));

	HANDLE WINAPI FileProjection = CreateFileMapping(
		/*Дескриптор файла, из которого можно создать объект сопоставления файлов.
 Если hFile является INVALID_HANDLE_VALUE, вызывающий процесс должен также указать размер для объекта сопоставления файлов
 в параметрах dwMaximumSizeHigh и dwMaximumSizeLow. В этом случае CreateFileMapping создает объект сопоставления файлов
 указанного размера, который поддерживается файлом подкачки системы, а не файлом в файловой системе.*/
		INVALID_HANDLE_VALUE,
		/*Указатель на структуру SECURITY_ATTRIBUTES, которая определяет, может ли возвращенный дескриптор наследоваться дочерними процессами.
 Член lpSecurityDescriptor структуры SECURITY_ATTRIBUTES указывает дескриптор безопасности для нового объекта сопоставления файлов.
 Если lpAttributes NULL, дескриптор не может быть унаследован, а объект сопоставления файлов получает дескриптор безопасности по умолчанию.*/
		NULL,
		/*Указывает защиту страницы объекта сопоставления файлов. Все сопоставленные виды объекта должны быть совместимы с этой защитой.
 PAGE_READWRITE позволяет отображать представления для чтения только для чтения, копирования на запись или доступа для чтения / записи.*/
		PAGE_READWRITE,
		/*DWORD максимального размера объекта сопоставления файлов.*/
		kDwMaximumSizeLow,
		/*Минимальное значение DWORD для максимального размера объекта сопоставления файлов.
		Если этот параметр и dwMaximumSizeHigh равны 0 (ноль), максимальный размер объекта сопоставления файлов равен текущему размеру файла,
		который hFile идентифицирует.*/
		kDwMaximumSizeHigh,
		/*Имя объекта сопоставления файлов.
		Если этот параметр соответствует имени существующего объекта сопоставления,
		функция запрашивает доступ к объекту с защитой, которую определяет функция flProtect.*/
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
				ReleaseSemaphore(Close, 1, NULL);	//Увеличивает количество указанного объекта семафора на определенное значение.
				ReleaseSemaphore(Work, 1, NULL);
				WaitForSingleObject(pi.hProcess, INFINITE);
				UnmapViewOfFile(memoryMap);			//Отключает сопоставленный вид файла из адресного пространства вызывающего процесса.
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

