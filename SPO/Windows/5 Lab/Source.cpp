#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#define SIZE 256
#define READ_BUFFER_SIZE 3
#define READ_EVENT "readEvent"
#define WRITE_EVENT	"writeEvent"
#define READ_EVENT_END "readEventEnd"
#define WRITE_EVENT_END "writeEventEnd"
#define END_EVENT "endEvent"


//DWORD WINAPI writer(void*);
//DWORD WINAPI reader(void*);
void test();

int main()
{
	typedef void(*MYFUN)();
	HMODULE hLib = LoadLibrary("FileConcat.dll");
	MYFUN fun = (MYFUN)GetProcAddress(hLib, "fcat");
	fun();
	//test();
	FreeLibrary(hLib);
	system("pause");
	return 0;
}


void test()
{
	WIN32_FIND_DATA fileData;
	HANDLE hFile;
	hFile = FindFirstFile("D:\\testDir\\*.*", &fileData);
	FindNextFile(hFile, &fileData);
	
	char buffer[READ_BUFFER_SIZE];
	char fileName[SIZE];
	const char* prefix = "D:\\testDir\\";

	while (FindNextFile(hFile, &fileData))
	{
		puts(fileData.cFileName);
	}
}