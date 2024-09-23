#include "timestamp.h"

#include <iostream>
#include <string>

int main() {
	Timestamp *time = new Timestamp();
	Timestamp tmp = time->Now();
	std::cout << tmp.ToFormattedString();
	delete time;
	return 0;
}

