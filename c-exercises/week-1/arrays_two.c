#include <stdio.h>

void increase_all(int *tab, int n) {
	for (int i = 0; i < n; i++) {
		tab[i] = tab[i] + 1;
	}

}

int main () {
	int digits[5] = {10, 20, 30, 40, 50};
	printf("Before:\n");
	for (int i = 0; i < 5; i++) printf("%d: %d\n", i, digits[i]);	


	increase_all(digits, 5);

	printf("\nAfter:\n");
	for (int i = 0; i < 5; i++) printf("%d: %d\n", i, digits[i]);	
	printf("\n");

	printf("digits address: %p\n", (void*)digits);
	printf("gigits value: %d\n", *digits);
	printf("digits[0] value: %d\n", digits[0]);

	return 0;
}
