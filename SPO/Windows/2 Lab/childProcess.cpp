#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <Windows.h>
#include <list>
#include <tchar.h>

void printProcessSignature(string procNumber);

const int DELAY = 100;
using namespace std;

int main(int argc, char *argv[]) {
	HANDLE eventHdl = OpenEvent(EVENT_ALL_ACCESS, false, TEXT("writeEvent"));
	string information = argv[1];

	while (true) {
		WaitForSingleObject(eventHdl, INFINITE);
		printProcessSignature(information);
		SetEvent(eventHdl);
	}
}


void printProcessSignature(string procNumber) {
	for (int i = 0; i < procNumber.length(); i++) {
		cout << procNumber[i];
	}

	string message = " Process";
	for (int i = 0; i < message.length(); i++) {
		cout << message[i];
		Sleep(DELAY);
	}
	cout << endl;

	Sleep(DELAY);
}