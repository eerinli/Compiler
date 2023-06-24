#ifndef A10P2
#define A10P2
#include <cstdint>

//returns the address of a pair of words from the arena, initialized to a and b respectively, or 0 if no memory is available.
int64_t *cons(int64_t a, int64_t *b);

//returns the first element of the pair whose address is p.
int64_t car(int64_t *p);

//returns the second element of the pair whose address is p.
int64_t *cdr(int64_t *p);

//sets the first element of p to the value v and returns p.
int64_t *setcar(int64_t *p, int64_t v);

//sets the second element of p to the value v and returns p.
int64_t *setcdr(int64_t *p, int64_t *v);

//deletes the pair whose address is p
void snoc(int64_t *addr);

#endif
