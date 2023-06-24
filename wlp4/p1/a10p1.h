#ifndef A10P1
#define A10P1
#include <cstdint>

/*
 * returns the address of the beginning of a block of RAM that may be used for dynamic memory allocation. 
 * Initially, the size of the arena (in bytes) is stored in the first word. That is, the expression *arena() 
 * returns the size of the arena, which will not be less than 2^23 or greater than 2^31. 
 * You may also assume it is a multiple of 8.
 */
int64_t *arena();

#endif
