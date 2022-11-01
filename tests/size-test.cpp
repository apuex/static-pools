#include <iostream>
#include <stddef.h>

using namespace std;

int main(int argc, char* argv[]) {
  cout << "sizeof(int) = " << sizeof(int) << endl;  
  cout << "sizeof(long) = " << sizeof(long) << endl;  
#if _MSC_VER > 1300
  cout << "sizeof(long long) = " << sizeof(long long) << endl;  
#endif
  cout << "sizeof(size_t) = " << sizeof(size_t) << endl;  
  cout << "sizeof(time_t) = " << sizeof(time_t) << endl;
#if _MSC_VER > 1300
  cout << "sizeof(uintptr_t) = " << sizeof(uintptr_t) << endl;  
  cout << "sizeof(ptrdiff_t) = " << sizeof(ptrdiff_t) << endl;  
#endif

  return 0;
}

