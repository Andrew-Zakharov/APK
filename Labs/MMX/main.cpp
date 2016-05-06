#include <math.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <Windows.h> 
#include <conio.h>
#include <time.h>
#define n 16

int print(int*);
int fill(int*);

void main(void)
{
	LARGE_INTEGER numberOfCycles,startTime,endTime;

	double elapsedTime;

	int firstArray[n], secondArray[n], cResult[n] = {}, asmResult[n] = {}, mmxResult[n] = {};
	int times = 1000000;

	srand(time(NULL));

	printf("First:\n");
	fill(firstArray);
	print(firstArray);
	printf("\n");

	printf("Second:\n");
	fill(secondArray);
	print(secondArray);
	printf("\n");

	QueryPerformanceFrequency(&numberOfCycles);
	QueryPerformanceCounter(&startTime);
	
	for (int k = 0; k < times; k++)
	{
		for (int i = 0; i < n; i++)
		{
			cResult[i] = firstArray[i] & secondArray[i];
		}
	}

	QueryPerformanceCounter(&endTime);
	elapsedTime = (double)(endTime.QuadPart - startTime.QuadPart)/numberOfCycles.QuadPart;

	printf("\n\nC result:\n");
	print(cResult);
	printf("\nTime: %f\n", elapsedTime);

	QueryPerformanceFrequency(&numberOfCycles);
	QueryPerformanceCounter(&startTime);

	for (int k = 0; k < times; k++)
	{
		_asm
		{
				mov ecx, 16
				xor esi, esi
				xor edx, edx
				xor ebx, ebx
			c1:
				mov edx, firstArray[esi]
				mov ebx, secondArray[esi]
				and edx,ebx
				mov asmResult[esi], edx
				add esi, 4
				dec ecx
			jnz c1
		}
	}

	QueryPerformanceCounter(&endTime);
	elapsedTime = (double)(endTime.QuadPart - startTime.QuadPart) / numberOfCycles.QuadPart;

	printf("\nASM result:\n");
	print(asmResult);
	printf("\nTime: %f\n", elapsedTime);

	QueryPerformanceFrequency(&numberOfCycles);
	QueryPerformanceCounter(&startTime);

	for (int k = 0; k < times; k++)
	{
		_asm
		{
				xor esi, esi
				mov ecx, 8
			c2:
				movq mm1, firstArray[esi]
				movq mm0, secondArray[esi]
				pand mm0, mm1
				movq mmxResult[esi], mm0
				add esi, 8
				pxor mm0,mm0
				dec ecx
			jnz c2
			emms
		}
	}

	QueryPerformanceCounter(&endTime);
	elapsedTime = (double)(endTime.QuadPart - startTime.QuadPart) / numberOfCycles.QuadPart;

	printf("\nMMX result:\n");
	print(mmxResult);
	printf("\nTime: %f\n", elapsedTime);

	system("pause");
}

int print(int* Array){
	for(int i = 0; i < n; i++){
		printf("%2d",Array[i]);
	}
	return 0;
}

int fill(int* Array){
	for(int i = 0; i < n; i++){
		Array[i] = rand() % 2;
	}
	return 0;
} 