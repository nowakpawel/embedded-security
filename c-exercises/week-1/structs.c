#include <stdio.h>
#include <stdint.h>

struct SensorReading {
		int16_t x;
		int16_t y;
		int16_t z;
		uint32_t timestamp;
};

void print_reading(struct SensorReading *r) {
	printf("x = %d y = %d z = %d t = %u\n", r->x, r->y, r->z, r->timestamp);
}

int main() {
	struct SensorReading reading;
	reading.x = 5;
	reading.y = -5;
	reading.z = 100;
	reading.timestamp = 123456;

	print_reading(&reading);

	printf("sizeof(struct SensorReading) = %zu\n", sizeof(struct SensorReading));
	return 0;
}
