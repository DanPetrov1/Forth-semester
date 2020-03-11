//#define _CRT_SECURE_NO_WARNINGS
//
//#include <conio.h>
//#include <windows.h>
//#include <cstdio>
//#include <cstdlib>
//#include <iostream>
//#include <string>
//#include <limits>
//
//const int kDelay = 100;
//
//using namespace std;
//
//int main() {
//	HANDLE Work = OpenSemaphore(
//		/*Доступ к объекту семафора. Функция не работает, если дескриптор безопасности указанного
//		объекта не разрешает запрашиваемый доступ для вызывающего процесса*/
//		SEMAPHORE_ALL_ACCESS,
//		/*Если это значение TRUE, процессы, созданные этим процессом, наследуют дескриптор. В противном случае процессы не наследуют этот дескриптор.*/
//		FALSE,
//		TEXT("Work"));
//	HANDLE Close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("Close"));
//	HANDLE WINAPI FileProjection = OpenFileMapping(
//		/*Доступ к объекту сопоставления файлов. Этот доступ проверяется на любой дескриптор безопасности в объекте сопоставления целевого файла.*/
//		FILE_MAP_ALL_ACCESS,
//		/*Не наследует дескриптор*/
//		FALSE,
//		/*Имя объекта сопоставления файлов, который нужно открыть. Если с этим именем есть открытый дескриптор объекта
//		сопоставления файлов, а дескриптор безопасности в объекте сопоставления не
//		конфликтует с параметром dwDesiredAccess, операция открытия успешно завершается.*/
//		TEXT("FileProjection"));
//
//	LPVOID pointer;
//	pointer = MapViewOfFile(FileProjection,
//		/*Отображается представление чтения / записи файла. Объект сопоставления файлов должен быть
//		создан с защитой PAGE_READWRITE или PAGE_EXECUTE_READWRITE.*/
//		FILE_MAP_ALL_ACCESS,
//		/*DWORD верхнего порядка смещения файла, где начинается представление.*/
//		0,
//		/*DWORD младшего порядка для смещения файла, где должно начинаться представление.
//		Комбинация высоких и низких смещений должна указывать смещение в сопоставлении файлов.
//		Они также должны соответствовать гранулярности распределения памяти в системе.
//		То есть, смещение должно быть кратным гранулярности распределения.*/
//		0,
//		/*Число байтов сопоставления файлов для сопоставления с представлением.
//		Все байты должны быть в максимальном размере, указанном CreateFileMapping.
//		Если этот параметр равен 0 (ноль), отображение продолжается от указанного смещения до конца сопоставления файлов.*/
//		0);
//
//	char* stringRepresentation = (char*)pointer;
//
//	while (true)
//	{
//		WaitForSingleObject(Work, INFINITE);
//		if (WaitForSingleObject(Close, kDelay) == WAIT_OBJECT_0) {
//			CloseHandle(Work);
//			CloseHandle(Close);
//			UnmapViewOfFile(pointer);
//			CloseHandle(FileProjection);
//			return 0;
//		}
//
//		cout << "Client got: " << stringRepresentation << endl;
//
//		ReleaseSemaphore(Work, 1, NULL);
//	}
//}