#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 16
#define PAGE_NUM 10

int myFree(void*);
void myDef();

struct PageInfo
{
	int isUse = 0;
	int npage = 0;
	void* pointer = NULL;
} pit[PAGE_NUM];
int initFlag = 0;
int k = 0;

void printPIT()
{
	for (int i = 0; i < PAGE_NUM; ++i)
	{
		if (pit[i].pointer)
		{
			printf("%d) %d %d %p - %d\n", i, pit[i].isUse, pit[i].npage, pit[i].pointer, *(int*)pit[i].pointer);
		}
		else
		{
			puts("error");
		}
	}
}

//in: pointer in pit
//out: pits index
//error: -1
int getIndex(void* pointer)
{
	for (int index = 0; index < PAGE_NUM; ++index)
	{
		if (pit[index].pointer == pointer)
		{
			return index;
		}
	}
	return -1;
}

//initializes pit
//error: 1
int pitInit()
{
	for (int i = 0; i < PAGE_NUM; ++i)
	{
		if (!(pit[i].pointer = malloc(PAGE_SIZE)))
		{
			return 1;
		}
		pit[i].npage = PAGE_NUM - i;
		*(int*)pit[i].pointer = i;
	}
	return 0;
}

//in: memory size
//out: memory pointer
//error: 0
void* myMalloc(int size)
{
	if (!initFlag)
	{
		if (pitInit())
		{
			return NULL;
		}
		else
		{
			initFlag = 1;
		}
	}

	if (size <= 0)
	{
		return NULL;
	}

	void* pointer = NULL;
	int index = 0;
	int npage = size / PAGE_SIZE + 1;
	int defFlag = 0;
	
	for (;;)
	{
		if (pit[index].isUse == 1 || pit[index].npage < npage)
		{
			index += pit[index].npage;
			if (index >= PAGE_NUM)
			{
				if (!defFlag)
				{
					myDef();
					defFlag = 1;
					index = 0;
				}
				else
				{
					return NULL;
				}
			}
		}
		else
		{
			int t = npage;
			for (int i = index; i < index + t; ++i)
			{
				pit[i].isUse = 1;
				pit[i].npage = npage--;
			}
			return pit[index].pointer;
		}
	}
}

//in: old pointer, memory size
//out: new pointer
//error: 1
void* myRealloc(void* pointer, int size)
{

	if (!initFlag)
	{
		if (pitInit())
		{
			return NULL;
		}
		else
		{
			initFlag = 1;
		}
	}

	if (size <= 0)
	{
		return NULL;
	}

	int index = getIndex(pointer);
	int npage = size / PAGE_SIZE + 1;

	if (index < PAGE_NUM - 1)// && index != -1)
	{
		if (!pit[index + 1].isUse && pit[index + 1].npage >= npage - pit[index].npage)
		{
			int j = 1;
			for (int i = index + npage - 1; i >= index; --i)
			{
				pit[i].isUse = 1;
				pit[i].npage = j++;
			}
			return pit[index].pointer;
		}
	}

	void* newPointer;
	int newIndex;
	void* oldPointer = pit[index].pointer;
	int count = 0;
	k = 0;
	if (!(newPointer = myMalloc(size)))
	{
		int i = PAGE_NUM - 1;
		while (!pit[i].isUse)
		{
			--i;
			++count;
		}
		if (pit[index].npage + count >= npage)
		{
			int psize = pit[index].npage;
			newIndex = PAGE_NUM - npage;

			while(psize > 0)
			{
				//void* tempPointer;
				//Page page;
				PageInfo pinfo;
				//tempPointer = pit[index + psize - 1].pointer;
				pinfo.isUse = pit[index + psize - 1].isUse;
				pinfo.npage = pit[index + psize - 1].npage;
				pinfo.pointer = pit[index + psize - 1].pointer;
				for (int j = index + psize; j < newIndex + psize; ++j)
				{
					pit[j - 1].isUse = pit[j].isUse;
					pit[j - 1].npage = pit[j].npage;
					pit[j - 1].pointer = pit[j].pointer;
 				}
				pit[newIndex + psize - 1].isUse = pinfo.isUse;
				pit[newIndex + psize - 1].npage = pinfo.npage;
				pit[newIndex + psize - 1].pointer = pinfo.pointer;
				--psize;
			}
			count = 1;
			psize = npage;
			while (psize >= 0)
			{
				pit[newIndex + psize - 1].isUse = 1;
				pit[newIndex + psize - 1].npage = count++;
				--psize;
			}
			return pit[index].pointer;
		}
		else
		{
			return NULL;
		}
	}
	if (k = 1)
	{
		index = getIndex(oldPointer);
	}

	count = 1;
	newIndex = getIndex(newPointer);
	if (index != -1)
	{
		for (int i = pit[index].npage - 1; i >= 0; --i)
		{
			//pit[i].isUse = 1;
			//pit[i].npage = count++;
			//if (i <= newIndex + pit[index].npage - 1)
			//{
				//*((Page*)pit[newIndex + i].pointer) = *((Page*)pit[index + i].pointer);
			//}
			void* tempPointer;
			tempPointer = pit[newIndex + i].pointer;
			pit[newIndex + i].pointer = pit[index + i].pointer;
			pit[index + i].pointer = tempPointer;
		}
		myFree(pit[index].pointer);
	}
	return newPointer;
}

//in: pointer that needs to be cleared
//error: 1
int myFree(void* pointer)
{
	int index = 0;
	for (index = 0; index < PAGE_NUM; ++index)
	{
		if (pit[index].pointer == pointer)
		{
			if (!pit[index].isUse)
			{
				return 1;
			}
			else
			{
				break;
			}
		}
	}
	if (index >= PAGE_NUM)
	{
		return 1;
	}
	
	int i = index + pit[index].npage - 1;
	if (i + 1 < PAGE_NUM)
	{
		if (!pit[i + 1].isUse)
		{
			i += 1 + pit[i + 1].npage;
if (i >= PAGE_NUM)
{
	--i;
}
		}
	}
	int j = 1;
	while (i >= index)
	{
		pit[i].isUse = 0;
		pit[i--].npage = j++;
	}
	while (i >= 0)
	{
		if (!pit[i].isUse)
		{
			pit[i--].npage = j++;
		}
		else
		{
			break;
		}
	}

	return 0;
}

//defragmenter
void myDef()
{
	int j = 0;
	void* tempPointer;
	for (int i = 0; i < PAGE_NUM; ++i)
	{
		if (pit[i].isUse)
		{
			pit[j].npage = pit[i].npage;
			pit[j].isUse = 1;
			tempPointer = pit[i].pointer;
			pit[i].pointer = pit[j].pointer;
			pit[j].pointer = tempPointer;
			++j;
		}
	}
	for (int i = PAGE_NUM - 1; i >= j; --i)
	{
		pit[i].isUse = 0;
		pit[i].npage = PAGE_NUM - i;
	}
}


int main()
{
	char c;
	void* parray[30];
	int num = 0, temp;
	void* pointer = NULL;
	pitInit();
	for (;;)
	{
		system("CLS");
		printPIT();
		puts("================");
		for (int i = 0; i < num; ++i)
		{
			printf("pointer - %d, index - %d, size - %d\n", i, getIndex(parray[i]), pit[getIndex(parray[i])].npage);
		}
		puts("================");
		puts("1) malloc");
		puts("2) realloc");
		puts("3) free");
		rewind(stdin);
		c = getchar();
		switch (c)
		{
		case '1':
			puts("input size");
			if (!scanf("%d", &temp))
			{
				rewind(stdin);
				break;
			}
			if (pointer = myMalloc(temp))
			{
				++num;
				parray[num - 1] = pointer;
			}
			break;
		case '2':
			puts("input pointer index");
			if (!scanf("%d", &temp) && temp >= 0)
			{
				rewind(stdin);
				break;
			}
			
			puts("input size");
			if (temp >= num)
			{
				int size;
				if (!scanf("%d", &size))
				{
					rewind(stdin);
					break;
				}
				if (pointer = myRealloc(NULL, size))
				{
					++num;
					parray[num - 1] = pointer;
				}
			}
			else
			{
				int size;
				if (!scanf("%d", &size))
				{
					rewind(stdin);
					break;
				}
				if (!(pointer = myRealloc(parray[temp], size)))
				{
					parray[temp] = pointer;
				}
			}
			break;
		case '3':
			puts("input pointer index");
			if (!scanf("%d", &temp) && temp >= num && temp >= 0)
			{
				rewind(stdin);
				break;
			}
			if (!myFree(parray[temp]))
			{	
				for (int i = temp; i < num - 1; ++i)
				{
					parray[i] = parray[i + 1];
				}
			}
			--num;
		}
	}
	/**
	p1 = myMalloc(3);
	p2 = myMalloc(34);
	p3 = myMalloc(33);
	printPIT();

	puts("==========");
	myFree(p2);
	p2 = myMalloc(2);
	printPIT();
	myFree(p3);
	puts("==========");
	//printPIT();
	/**/

	/**
	puts("==========");
	p1 = myRealloc(p1, 30);
	*(int*)p1 = 1;
	p2 = myRealloc(p2, 27);
	*(int*)p2 = 2;
	printPIT();
	puts("==========");
	p1 = myRealloc(p1, 60);
	printPIT();
	/**/

	/**
	puts("==========");
	p1 = myRealloc(p1, 34);
	*(int*)p1 = 1;
	p2 = myRealloc(p2, 27);
	*(int*)p2 = 2;
	p3 = myRealloc(p3, 60);
	*(int*)p3 = 3;
	printPIT();
	puts("==========");
	p1 = myRealloc(p1, 60);
	printPIT();
	/**/

	getchar();
	return 0;
}

