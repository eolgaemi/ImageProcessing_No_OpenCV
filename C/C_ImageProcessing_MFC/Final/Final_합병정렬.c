#include <stdlib.h>

void merge(unsigned char arr[], int low, int mid, int high) {

	unsigned char* array_temp = (unsigned char*)malloc(sizeof(unsigned char) * (high - low + 1));

	int l_idx = low;
	int r_idx = mid + 1;
	int record_idx = 0;

	while (l_idx <= mid && r_idx <= high) {
		if (arr[l_idx] < arr[r_idx]) {
			array_temp[record_idx] = arr[l_idx];
			record_idx++;
			l_idx++;
		}
		else {
			array_temp[record_idx] = arr[r_idx];
			record_idx++;
			r_idx++;
		}
	}

	while (l_idx <= mid) {
		array_temp[record_idx] = arr[l_idx];
		record_idx++;
		l_idx++;
	}

	while (r_idx <= high) {
		array_temp[record_idx] = arr[r_idx];
		record_idx++;
		r_idx++;
	}
	--record_idx;

	while (record_idx >= 0) {
		arr[low + record_idx] = array_temp[record_idx];
		record_idx--;
	}

	free(array_temp);
}

void mergesort(unsigned char arr[], int low, int high) {


	if (low < high) {
		int mid = 0;

		if ((low + high) % 2 == 0) {
			mid = (low + high) / 2;
		}
		else {
			mid = (low + high - 1) / 2;
		}
		mergesort(arr, low, mid);
		mergesort(arr, mid + 1, high);

		merge(arr, low, mid, high);
	}
	else {
		return;
	}

}