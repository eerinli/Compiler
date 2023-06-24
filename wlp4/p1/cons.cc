#include <iostream>
#include <cstdint>
#include <cstdio>
#include "a10p1.h"
#include "a10p2.h"
using namespace std;

// The program implement the functionality provided by a10p2.o using a10p1.o.
int64_t *freeList = nullptr;
bool isFirst = true;

//returns the address of a pair of words, initialized to a and b respectively, or 0 if no memory is available
int64_t *cons(int64_t a, int64_t *b) {
    // initialization
    if (isFirst) {
        isFirst = false;
        freeList = arena();
        freeList[0] /= 8; // length of block
        freeList[1] = 0; // address of next
    }
    if (freeList != nullptr) {
        int64_t *newFreelist = freeList;
        if (freeList[0] < 2) {
            return nullptr;
        } else if (freeList[0] < 4) {
            int64_t *next = (int64_t*)freeList[1];
            freeList = next;
        } else {
            freeList[2] = freeList[0] - 2; 
            freeList[3] = freeList[1];
            freeList = freeList + 2;
        }
        newFreelist = setcar(newFreelist, a);
        newFreelist = setcdr(newFreelist, b);
        return newFreelist;
    } else {
        return nullptr;
    }

}

//returns the first element of the pair whose address is p
int64_t car(int64_t *p) {return p[0];}

//returns the second element of the pair whose address is p
int64_t *cdr(int64_t *p) {return (int64_t*)p[1];}

//sets the first element of p to the value v and returns p
int64_t *setcar(int64_t *p, int64_t v) {
    p[0] = v;
    return p;
}

//sets the second element of p to the value v and returns p
int64_t *setcdr(int64_t *p, int64_t *v) {
    p[1] = (int64_t)v;
    return p;
}

//deletes the pair whose address is p, or does nothing if p is null
void snoc(int64_t *p) {
    if (p != nullptr) {
        p[0] = 2;
        p[1] = (int64_t)freeList;
        freeList = p;
    }
}
