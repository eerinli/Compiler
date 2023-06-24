#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "merl.h"
using namespace std;

/*
Starter code for the linker you are to write for CS241 Assignment 10 Problem 5.
The code uses the MERL library defined in "merl.h". This library depends on the object file
"merl.o" and this program must be compiled with those object files.

The starter code includes functionality for processing command-line arguments, reading the input 
MERL files, calling the linker, and producing output on standard output.

You need to implement the "linking constructor" for MERL files, which takes two MERL file objects
as input and produces a new MERL file object representing the result of linking the files:

MERL::MERL(MERL& m1, MERL& m2)

A definition of this function is at the end of the file. The default implementation creates a
dummy MERL file to demonstrate the use of the MERL library. You should replace it with code that
links the two input files.

You are free to modify the existing code as needed, but it should not be necessary.
You are free to add your own helper functions to this file.

The functions in the MERL library will throw std::runtime_error if an error occurs. The starter
code is set up to catch these exceptions and output an error message to standard error. You may
wish to use this functionality for your own error handling.
*/

// Links all the MERL objects in the given list, by recursively
// calling the "linking constructor" that links two MERL objects.
// You should not need to modify this function.
MERL link(std::list<MERL>& merls) {
  if(merls.size() == 1) {
    return merls.front();
  } else if(merls.size() == 2) {
    return MERL(merls.front(),merls.back()); 
  } 
  MERL first = merls.front();
  merls.pop_front();
  MERL linkedRest = link(merls);
  return MERL(first,linkedRest);
}

// Main function, which reads the MERL files and passes them into
// the link function, then outputs the linked MERL file. 
// You should not need to modify this function.
int main(int argc, char* argv[]) {
  if(argc == 1) { 
    std::cerr << "Usage: " << argv[0] << " <file1.merl> <file2.merl> ..."  << std::endl;
    return 1;
  }
  std::list<MERL> merls;
  try {
    for(int i=1; i<argc; i++) {
      std::ifstream file;
      file.open(argv[i]);
      merls.emplace_back(file);
      file.close();
    }
    // Link all the MERL files read from the command line.
    MERL linked = link(merls);
    // Print a readable representation of the linked MERL file to standard error for debugging.
    linked.print(std::cerr);
    // Write the binary representation of the linked MERL file to standard output.
    std::cout << linked;
  } catch(std::runtime_error &re) {
    std::cerr << "ERROR: " << re.what() << std::endl;
    return 1;
  }
  return 0;
}

// Linking constructor for MERL objects.
// Implement this, which constructs a new MERL object by linking the two given MERL objects. 
// The function is allowed to modify the inputs m1 and m2. It does not need to leave them in 
// the same state as when the function was called.
// You should not output anything to standard output here; the main function handles output.
MERL::MERL(MERL& m1, MERL& m2) { 
  
  // Step 1: Check for duplicate export errors
  std::unordered_set<string> ESDnames;
  for (auto &entry: m2.table) {
    if (entry.type == Entry::ESD) {
      if (entry.hasName()) ESDnames.insert(entry.name);
      }
  }
  for (auto &entry: m1.table) {
    if (entry.type == Entry::ESD) {
      if (ESDnames.count(entry.name))  throw runtime_error("ERROR: duplicate ESD");
    }
  }
  
  // Step 2: Combine the code segements for the linked file
  code = m1.code;
  code.insert(code.end(), m2.code.begin(), m2.code.end());

  // Step 3: Relocate m2’s table entries
  int reloc_offset = m1.endCode - 12;
  for (auto &entry: m2.table) {
    entry.location += reloc_offset;
  }

  // Step 4: Relocate m2.code
  const int WORD_SIZE = 4;
  for (auto &entry: m2.table) {
    if (entry.type == Entry::REL) {
      int index = (entry.location - 12) / WORD_SIZE; // (address to relocate - 12) / word size
      code[index] += reloc_offset;
    }
  }

  // Step 5: Resolve imports for m1
  unordered_map<string, int> m2ESDlocs; // <name, location>
  for (auto &entry: m2.table) {
    if (entry.type == Entry::ESD) m2ESDlocs[entry.name] = entry.location;
  }

  for (auto &entry: m1.table) {
    if (entry.type == Entry::ESR) {
      if (m2ESDlocs.count(entry.name)) {
        int index = (entry.location - 12) / WORD_SIZE;
        code[index] = m2ESDlocs[entry.name];
        entry.type = Entry::REL;
      }
    }
  } 
  // Step 6: Resolve imports for m2
  // Repeat Step 5 for imports from m2 and exports for m1 
  unordered_map<string, int> m1ESDlocs; // <name, location>
  for (auto &entry: m1.table) {
    if (entry.type == Entry::ESD) m1ESDlocs[entry.name] = entry.location;
  }

  for (auto &entry: m2.table) {
    if (entry.type == Entry::ESR) {
      if (m1ESDlocs.count(entry.name)) {
        int index = (entry.location - 12) / WORD_SIZE;
        code[index] = m1ESDlocs[entry.name];
        entry.type = Entry::REL;
      }
    }
  } 

  // Step 7: Combine the tables for the linked file
  // linked_table = concatenate modified m1.table and modified m2.table
  for (auto &entry: m1.table) this->table.push_back(entry);
  for (auto &entry: m2.table) this->table.push_back(entry); 

  // Step 8: Compute the header information 
  // endCode = 12 + linked_code size in bytes 
  const int BYTE_SIZE = 4;
  endCode = 12 + code.size() * BYTE_SIZE;
  endModule = endCode;
  for (auto &entry: table) endModule += entry.size();
  
  // Step 9: Output the MERL file
}