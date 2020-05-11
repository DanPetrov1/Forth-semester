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
	// ������� ����� � ���������� ����������

	void* pLocal;
	UINT   uMaxFreeMem;

	pLocal = memoryAllocation(size);

	if (pLocal == NULL)
	{
		//LocalCompact ����p�p��� ��������� ���� p����p�� �� ����� MinFree. ���� �����, ������� ����� ���������� p�������p������� ����� �/��� ��p�������  
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

	// ��������� ����������� ���������� ������
	hmemLocal = LocalAlloc(GHND, size + sizeof(HLOCAL));

	if (hmemLocal != NULL)
	{
		// ���� ����� �������, ��������� ��� � ������
		pLocal = LocalLock(hmemLocal);
		if (pLocal != NULL)
		{
			// ������ ����������� � ������ ���������� ������
			pLocal = hmemLocal;
			// ��������� ������ ������ �� ��������� ����� ����������� ����
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
	// ��������� ����������� ������
	hmemLocal = (char*)ptr - sizeof(HLOCAL);
	// ������ hmemLocal
	LocalUnlock(hmemLocal);
	// ���������� NULL ��� �������� ������������ ������
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