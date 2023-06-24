#include <iostream>
#include <cstdint>
#include <cstdio>
#include "a10p1.h"
using namespace std;
// The program implement functions, in terms of a10p1.o, using the free list algorithm:
int64_t *freeList = nullptr;
bool isFirst = true;

// allocates a block of memory of at least size words (not bytes), 
// and returns the address of that memory or 0 (NULL) if memory could not be allocated.
int64_t *mymalloc(int64_t size) {
    if (isFirst) {
        freeList = arena();
        freeList[0] /= 8; // length of block
        freeList[1] = 0; // address of next
        isFirst = false;
    }
    if (freeList == nullptr) return nullptr;
    else if (freeList[0] <= size) {
        int64_t *prev = freeList;
        int64_t *cur = (int64_t*)freeList[1];
        while (cur != nullptr) {
            if (cur[0] < size + 1) {
                prev = cur;
                cur = (int64_t*)cur[1];
            } else { 
                int64_t *malloced = cur;
                if (cur[0] > size + 2) {
                    int64_t *nextAddr = (int64_t*)cur[1];
                    cur = cur + size + 1;
                    cur[0] = cur[0] - size - 1; // left block
                    cur[1] = (int64_t)nextAddr;
                    prev[1] = (int64_t)cur;
                    malloced[0] = size + 1;
                } else { // cur[0] == size +1 or +2
                    prev[1] = cur[1];   // prev -> cur -> next ==> prev -> next
                }
                return malloced + 1;
            } 
        }
        return nullptr; 
    } else  {  // divide to 2 blocks
        int64_t *malloced = freeList;
        if (freeList[0] > size + 2) {
            int64_t *nextAdd = (int64_t*)freeList[1];
            freeList = freeList + size + 1;
            freeList[0] = freeList[0] - size - 1; 
            freeList[1] = (int64_t)nextAdd;
            malloced[0] = size + 1;
        } else {
            freeList = (int64_t*)freeList[1];   // update next block
        }
        return malloced + 1;
    }
}

// deallocates the memory stored at address.
// assumes that address contains either an address allocated by mymalloc, in which case it deallocates that memory, 
// or the value 0 (NULL), in which case myfree does nothing.
void myfree(int64_t *address) {
    if (address != nullptr) {
        int64_t *freeAt = address - 1;
        int64_t len = freeAt[0];
        if (freeList != nullptr) {
            if (freeAt >= freeList) {  
                int64_t *prev = freeList;
                int64_t *cur = (int64_t*)freeList[1];
                while (cur != nullptr) {
                    if (prev < freeAt && freeAt < cur) {
                        break;
                    } else {
                        prev = cur;
                        cur = (int64_t*)cur[1];
                    }
                }
                if (cur == nullptr) {   // prev -> freeAt -> nullptr;
                    int64_t prevLen = prev[0];
                    if (freeAt != prev + prevLen) {      // prev -> freeAt -> nullptr;
                        prev[1] = (int64_t)freeAt;
                        freeAt[1] = 0;
                    } else { // merge prev and freeAt
                        prev[0] = len + prevLen;
                        prev[1] = 0;
                    } 
                } else {    // prev -> freeAt -> cur
                    bool checkFreeAt = (freeAt == prev + prev[0]);
                    bool checkCur = (cur == freeAt + freeAt[0]);
                    if (checkFreeAt && checkCur) { // merge all
                        prev[0] += freeAt[0] + cur[0];
                        prev[1] = cur[1];
                    } else if (checkFreeAt) { // prev + freeAt -> cur
                        prev[0] += freeAt[0];
                        prev[1] = (int64_t)cur;
                    } else if (checkCur) { // prev -> freeAt + cur
                        freeAt[0] += cur[0];
                        prev[1] = (int64_t)freeAt;
                        freeAt[1] = cur[1];
                    } else { // prev -> freeAt -> cur
                        prev[1] = (int64_t)freeAt;
                        freeAt[1] = (int64_t)cur;
                    }
                }
            
            } else { // freeAt < freeList
                bool checklen = (freeList == freeAt + len);
                bool checklenPlus = (freeList == freeAt + len + 1);
                if (checklen || checklenPlus) {
                    int64_t freeListLen = freeList[0];
                    int64_t *next = (int64_t*)freeList[1];
                    if (checklen) freeAt[0] = len + freeListLen;
                    else freeAt[0] = len + freeListLen + 1;
                    freeAt[1] = (int64_t)next;
                } else { // freeAt -> freeList -> next 
                    freeAt[1] = (int64_t)freeList;
                }
                freeList = freeAt;
            }
        } else { // null freeList
            freeList = freeAt;
            freeAt[1] = 0;
        }
    }
}
