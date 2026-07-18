#include <stdio.h>
#include <stdint.h>

int main(void) {

	uint8_t reg = 0b00001000;
	printf("start:		%d\n", reg);

	reg = reg | (1 << 5);
	printf("after setting bit 5:		%d\n", reg);
	
	reg = reg  & ~(1 << 3);
	printf("after cleaning bit 3:		%d\n", reg);

	reg = reg ^ (1 << 5);
	printf("after toggle bit 5:		%d\n", reg);
	
	return 0;
}
