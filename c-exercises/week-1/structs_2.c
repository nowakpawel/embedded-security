#include <stdio.h>
#include <stdbool.h>

struct Student {
	char name[50];
	int age;
	float gpa;
	bool isFullTime;


};

int main() {
	struct Student student1 = {"Spongebob", 30, 2.5, true};

	printf("studen1 name: %s\n", student1.name);
	printf("studen1 age: %d\n", student1.age);
	printf("studen1 gpa: %.2f\n", student1.gpa);
	printf("studen1 is full time?: %s.\n", (student1.gpa) ? "Yes": "No");

	return 0;
}
