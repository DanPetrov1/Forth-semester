#include <windows.h>
#include <iostream>

void* memoryAllocation(int size);
void* myMalloc(int size);
void  myFree(void* ptr);
void* myRealloc(void* ptr, int newSize);

int main()
{
	char* tmp;
	tmp = (char*)myMalloc(20);
	if (!tmp) {
		myFree(tmp);
		return 0;
	}
	std::cin >> tmp;
	std::cout << tmp;
	//myFree(tmp);

	tmp = (char*)myRealloc(tmp, 30);
	if (!tmp) return 0;
	std::cin >> tmp;
	std::cout << tmp;
	myFree(tmp);

	return 0;
}

void* myMalloc(int size)
{
	// находит место и возвращает дескриптор

	void* pLocal;
	UINT   uMaxFreeMem;

	pLocal = memoryAllocation(size);

	if (pLocal == NULL)
	{
		//LocalCompact генеpиpует свободный блок pазмеpом не менее MinFree. Если нужно, функция будет уничтожать pазблокиpованные блоки и/или пеpемещать  
		uMaxFreeMem = LocalCompact(size); 
		std::cout << "Trying to swap memory" << std::endl;
		pLocal = memoryAllocation(size);
		if (pLocal == NULL)
		{
			std::cout << "Not enough memory" << std::endl;
			return nullptr;
		}
		else
		{
			return nullptr;
		}
	}
	return pLocal;
}

void* memoryAllocation(int size)
{
	HLOCAL hmemLocal;
	void* pLocal;

	// получение дескриптора выделяемой памяти
	hmemLocal = LocalAlloc(GHND, size + sizeof(HLOCAL));

	if (hmemLocal != NULL)
	{
		// Если буфер получен, фиксируем его в памяти
		pLocal = LocalLock(hmemLocal);
		if (pLocal != NULL)
		{
			// Запись дескриптора в начало выделенной памяти
			pLocal = hmemLocal;
			// установка начала памяти на следующий после дескриптора байт
			pLocal = (char*)pLocal + sizeof(HLOCAL);
			return pLocal;
		}
		else
		{
			std::cout << std::endl << "Error in locking block";
		}
	}
	else
	{
		std::cout << std::endl << "Not enough memory";
	}
	return nullptr;
}

void  myFree(void* ptr)
{
	HLOCAL hmemLocal;
	// получение дескриптора памяти
	hmemLocal = (char*)ptr - sizeof(HLOCAL);
	// разлок hmemLocal
	LocalUnlock(hmemLocal);
	// возвращает NULL при успешном освобождении памяти
	if (LocalFree(hmemLocal) != NULL)
	{
		std::cout << std::endl << "Error in making memory free";
	}
	else
	{
		std::cout << std::endl << "Memory is free";
	}
	ptr = nullptr;
}

void* myRealloc(void* ptr, int newSize)
{

	HLOCAL hmemLoDiscard = (char*)ptr - sizeof(HLOCAL);
	hmemLoDiscard = LocalReAlloc(hmemLoDiscard, newSize, NULL);

	if (hmemLoDiscard == NULL)
	{
		std::cout << std::endl << "Error in locking block";
		return nullptr;
	}
	void* startMem = (char*)ptr - sizeof(HLOCAL);
	startMem = LocalLock(hmemLoDiscard);
	if (startMem == NULL)
	{
		std::cout << std::endl << "Error in locking block";
		std::cout << std::endl << GetLastError();

	}
	startMem = hmemLoDiscard;
	return (char*)startMem + sizeof(HLOCAL);
}