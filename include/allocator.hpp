#ifndef __APUEX_ALLOCATOR_CXX_INCLUDED_
#define __APUEX_ALLOCATOR_CXX_INCLUDED_

#include "stddef-config.h"
#include <memory>
#include <iostream>

namespace apuex {

  struct block {
    void* _location;
    apuex_bool _used;
  };

  template <typename T>
  class allocator {
  public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    typedef value_type reference;
    typedef const value_type const_reference;

    typedef size_t size_type;
    typedef size_t difference_type;

    explicit allocator(size_t size): _capacity(size) {}
    allocator(const allocator& rv): _capacity(rv._capacity) {}

    template<typename U> 
    allocator(const allocator<U>&): _capacity(100) { }
    template<typename U> 
    bool operator==(const allocator<U>&) const { return true; }

    /*
    template< class T, class... Args >
    static void construct(Alloc& a, T* p, Args&&... args) {

    }

    template <class _Uty>
    void destroy(_Uty* const _Ptr) {
        _Ptr->~_Uty();
    }

    value_type* allocate(size_t size, void* location) {
      std::cout << "allocate " << typeid(value_type).name() << ": " << size << std::endl;
      return static_cast<value_type*>(malloc(size * sizeof(value_type)));
    }
    */

    value_type* allocate(size_t size) {
      std::cout << "allocate " << typeid(value_type).name() << ": " << size << std::endl;
      return static_cast<value_type*>(malloc(size * sizeof(value_type)));
    }

    void deallocate(value_type* const location, size_t size) {
      std::cout << "deallocate " << typeid(value_type).name() << ": " << size << std::endl;
      free(location);
    }

  private:
    const size_t _capacity;

  };

}

#endif /* __APUEX_ALLOCATOR_CXX_INCLUDED_ */

