#include <iostream>
#include <cstdint>
#include "a10p1.h"
using namespace std;

// === Insert any helper functions here

int64_t wain(int64_t* a, int64_t n) {
  int64_t *arePointer = arena();
  int64_t freqNum = 0;
  for (unsigned int i = 0; i < n; i++) {
      arePointer[a[i]]++;
      freqNum = max(freqNum, arePointer[a[i]]);
  }
  return freqNum; // replace as appropriate
}

// Do not modify the code below.
int main(int argc, char *argv[]) {
  int64_t l, c;
  int64_t* a;
  std::cout << "Enter length of array: ";
  std::cin >> l; 
  a = new int64_t[l];
  for(int64_t i = 0; i < l; ++i) {
    std::cout << "Enter value of array element " << i << " ";
    std::cin >> a[i];
  }
  c = wain(a,l);
  delete [] a;
  std::cerr << c << std::endl;
  return c;
}
