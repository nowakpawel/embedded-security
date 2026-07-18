#include <stdio.h>

int main(void) {
	int x = 42;
	int *p = &x;

	printf("x = %d\n", x);
	printf("*p = %d\n", *p);

	printf("address x (&x) = %p\n", (void*)&x);
	printf("Value p (&x) = %p\n", (void*)p);
	
	*p = 100;
	printf("x after *p = 100: x = %d\n", x);

	return 0;
}
