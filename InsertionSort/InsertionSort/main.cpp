#include <iostream>


void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

/* Function to sort an array using insertion sort*/
void insertionSortASC(int* arr, int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;

		/* Move elements of arr[0..i-1], that are
		   greater than key, to one position ahead
		   of their current position */
		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}

void insertionSortDESC(int* arr, int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;

		/* Move elements of arr[0..i-1], that are
		   greater than key, to one position ahead
		   of their current position */
		while (j >= 0 && arr[j] < key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
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

	insertionSortDESC(x, N);

	for (int i = 0; i < N; i++) {
		printf("%d, ", x[i]);
	}

	system("pause");
	return 0;

}