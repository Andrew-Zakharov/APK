/*F(x) = 2sin(x) + cos(3x)*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

int main()
{

	double intervalStart,intervalEnd;

	double step,x,f;

	double elapsedTime;

	LARGE_INTEGER frequency,startTime,finishTime;

	double TWO = 2.0;

	double THREE = 3.0;

	while(1)
	{

		system("cls");

		printf("Enter the beginning of interval: ");
		scanf("%lf",&intervalStart);

		printf("Enter the end of interval: ");
		scanf("%lf",&intervalEnd);

		printf("Enter the step: ");
		scanf("%lf",&step);

		x = intervalStart;

		f = 0.0;

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);

		_asm{

			finit

				fld intervalEnd
				fld x

loop_start:

			fcom
				fstsw ax
				and ah, 01000101b
				jz loop_end

				fld x
				fsin
				fld TWO
				fmul

				fld x
				fld THREE
				fmul
				fcos

				fadd

				fadd f
				fstp f

				fadd step
				fst x
				jmp loop_start

loop_end:
			fwait
		}

		QueryPerformanceCounter(&finishTime);

		elapsedTime = (double)(finishTime.QuadPart - startTime.QuadPart)/frequency.QuadPart;

		printf("\nasm: %f\n",f);

		printf("Elapsed time: %f\n\n",elapsedTime);

		x = intervalStart;

		f = 0.0;

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);

		while(x <= intervalEnd)
		{

			f += TWO*sin(x) + cos(THREE*x);

			x += step;

		}

		QueryPerformanceCounter(&finishTime);

		elapsedTime = (double)(finishTime.QuadPart - startTime.QuadPart)/frequency.QuadPart;

		printf("C: %f\n",f);

		printf("Elapsed time: %f\n\n",elapsedTime);

		system("pause");

		printf("\nContinue?[y/n]: ");

		fflush(stdin);

		if(getchar() == 'n')

			break;

	}

	return 0;

}
