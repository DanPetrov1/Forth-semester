#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <random>
#include <math.h>
#include <Windows.h>

int main() {
	LARGE_INTEGER frequency, start, finish, start2, frequency2, finish2;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	long long int a = 5;
	long double sum1 = 0, sum2 = 0, qq;
	long double y, x = 0, step = 0.001, max = 500;
	printf("%d\n", sizeof(y));
	_asm {
		FINIT
		FLD max
		FLD x

		loop_start :
		FCOM
			FSTSW AX
			AND AH, 01000101b
			JZ loop_end

			FLD x
			FSIN
			FLD x
			FIADD a
			FPTAN
			FSTP qq
			FMULP ST(1), ST(0)

			FADD sum1
			FSTP sum1

			FADD step
			FST x
			JMP loop_start

			loop_end :

		FWAIT

	}
	QueryPerformanceCounter(&finish);
	printf("%lf\n", sum1);
	float delay = (finish.QuadPart - start.QuadPart) * 1000.0f / frequency.QuadPart;
	printf("The time: %f seconds\n", delay);
	QueryPerformanceCounter(&start2);
	QueryPerformanceFrequency(&frequency2);
	for (x = 0; x <= max; x += step) {
		y = sin(x) * tan((x + 5));
		sum2 += y;
	}
	QueryPerformanceCounter(&finish2);
	printf("%lf\n", sum2);

	float delay2 = (finish2.QuadPart - start2.QuadPart) * 1000.0f / frequency2.QuadPart;

	printf("The time: %f seconds\n", delay2);
	return 0;
}