#include "apuex/allocator.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace apuex;

typedef apuex::allocator<uint32_t> myallocator;

int main(int argc, char* argv[]) {
  uint32_t d = 0;
  myallocator myalloc(32);
  vector<uint32_t, myallocator> v(32, d, myalloc);

  return 0;
}

