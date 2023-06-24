#include <iostream>
#include <cstdint>
#include <cstdio>
#include "a10p2.h"
using namespace std;
// === Insert any helper functions here

// writes the input file once to standard output.
void getOutput(int64_t *outFrom) {
  if (outFrom != nullptr) {
    char output = car(outFrom);
    getOutput(cdr(outFrom));
    cout << output;
  }
}

// empty the memory with snoc
void snocMemory(int64_t *outFrom) {
  if (outFrom != nullptr) {
    snocMemory(cdr(outFrom));
    snoc(outFrom);
  }
}

// The program writes the input file twice to standard output.
int64_t wain(int64_t*, int64_t) {
  int64_t *outFrom = nullptr;
  int byteNum = 0;
  cin >> noskipws; // keep the white space
  char input;
  while (cin >> input) {
    byteNum++;
    outFrom = cons(input, outFrom);
  }

  getOutput(outFrom);
  getOutput(outFrom);
  snocMemory(outFrom);
  return byteNum;
}

// Do not modify the code below.
int main(int argc, char *argv[]) {
  int ret = wain(0,0);
  std::cerr << ret << std::endl;
  return ret;
}