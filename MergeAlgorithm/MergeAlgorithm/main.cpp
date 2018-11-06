#include <iostream>


void mergeArrays(int* arr1, int* arr2, int n1, int n2, int* arr3){
	int i = 0, j = 0, k = 0;

	while (i < n1 && j < n2)

	{
		if (arr1[i] < arr2[j])
			arr3[k++] = arr1[i++];
		else
			arr3[k++] = arr2[j++];
	}

	while (i < n1)
		arr3[k++] = arr1[i++];

	while (j < n2)
		arr3[k++] = arr2[j++];

}

using namespace std;

int main(void) {


	int* size = new int[3]{
	5,3,2
	};

	int** arr1 = new int*[3];
	arr1[0] = new int[size[0]]{
		1,5,11,12,25
	};
	arr1[1] = new int[size[1]]{
		1,3,6
	};
	arr1[2] = new int[size[2]]{
		0,59
	};

	//**************************For n arrays********************************

	int* output = nullptr;
	int size_o = 0;

	for (int i = 0; i < 3; i++) {
		int s = 0;
		for (int y = 0; y <= i; y++) {
			s += size[y];
		}

		int* tmp = new int[s];

		mergeArrays(output, arr1[i], size_o, size[i], tmp);
		output = tmp;
		size_o = s;
	}

	//************************************************************************

	for (int i = 0; i < size_o; i++) {
		cout << output[i] << ",";
	}

	cout << endl;


	delete arr1;

	system("Pause");
	return 0;
}



