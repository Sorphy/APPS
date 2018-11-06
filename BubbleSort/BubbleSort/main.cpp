#include <iostream>

using namespace std;

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// A function to implement bubble sort 
void bubbleSortASC(int* arr, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)

		// Last i elements are already in place    
		for (j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}

void bubbleSortDESC(int* arr, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)

		// Last i elements are already in place    
		for (j = 0; j < n - i - 1; j++)
			if (arr[j] < arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}

int main() {
	int N = 10;

	int* x = new int[N];
	x[0] = 5;
	x[1] = 1;
	x[2] = -20;
	x[3] = 100;
	x[4] = 1;
	x[5] = 0;
	x[6] = -200;
	x[7] = 5;
	x[8] = -300;
	x[9] = 153;

	bubbleSortDESC(x, N);

	for (int i = 0; i < N; i++) {
		printf("%d, ", x[i]);
	}

	system("pause");
	return 0;
}