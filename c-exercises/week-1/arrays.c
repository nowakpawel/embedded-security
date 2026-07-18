#include <stdio.h>

int main() {
	int arr[4] = {10, 20, 30, 40};
	int *p = arr;

	printf("arr[2]			= %d\n", arr[2]);
	printf("*(arr+2)		= %d\n", *(arr + 2));
	printf("p[2]			= %d\n", p[2]);
	printf("sizeof(arr)		= %zu\n", sizeof(arr));
	printf("sizeof(p)		= %zu\n", sizeof(p));

	return 0;
}
