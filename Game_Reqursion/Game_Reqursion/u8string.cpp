#include "u8string.h"

size_t u8length(std::string str) {
	std::string::iterator it = str.begin(), end = str.end();
	size_t result = 0;
	while (it != end) {
		if (((*it & (1 << 7)) && (*it & (1 << 6))) || !(*it & (1 << 7)))
			result++;
		it += 1;
	}
	return result;
}
