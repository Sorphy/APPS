#include <iostream>

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void selectionSortASC(int* arr, int n)
{
	int i, j, min_idx;

	// One by one move boundary of unsorted subarray 
	for (i = 0; i < n - 1; i++)
	{
		// Find the minimum element in unsorted array 
		min_idx = i;
		for (j = i + 1; j < n; j++)
			if (arr[j] < arr[min_idx])
				min_idx = j;

		// Swap the found minimum element with the first element 
		swap(&arr[min_idx], &arr[i]);
	}

}

void selectionSortDESC(int* arr, int n)
{
	int i, j, min_idx;

	// One by one move boundary of unsorted subarray 
	for (i = 0; i < n - 1; i++)
	{
		// Find the minimum element in unsorted array 
		min_idx = i;
		for (j = i + 1; j < n; j++)
			if (arr[j] > arr[min_idx])
				min_idx = j;

		// Swap the found minimum element with the first element 
		swap(&arr[min_idx], &arr[i]);
	}

}

using namespace std;

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


	selectionSortDESC(x, N);

	for (int i = 0; i < N; i++) {
		printf("%d, ", x[i]);
	}

	system("pause");
	return 0;
}