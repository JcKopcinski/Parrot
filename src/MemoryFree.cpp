#ifdef ESP32
#include "include/DuckEsp.h"

int freeMemory() {
	return duckesp::freeHeapMemory();
}

#elif defined(__linux__) && defined(__arm__)
#define _GNU_SOURCE //allows for full GNU C library features (including mallinfo2)
#include <malloc.h>

int freeMemory() {
	struct mallinfo2 mi = mallinfo2();
	return mi.fordblks;
}
#elif defined(__arm__)
//ARM based MCU
extern "C" char* sbrk(int incr);

int freeMemory() {
	char top;
	return &top - reinterpret_cast<char*>(sbrk(0));
}
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
extern char*__brkval;

int freeMemory() {
	char top;
	return &top - __brkval;
}
#else
//Generic AVR fallback
extern char *__brkval;
extern char*__malloc_heap_start;

int freeMemory() {
	char top;
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
}
#endif //ESP32
