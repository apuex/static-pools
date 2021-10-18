#ifndef __APUEX_ALLOCATOR_CXX_INCLUDED_
#define __APUEX_ALLOCATOR_CXX_INCLUDED_

#include <apuex/stddef_config.h>
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

    typedef value_type& reference;
    typedef const value_type& const_reference;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    template <typename U>
    struct rebind {
      typedef allocator<U> other;
    };
    
    allocator(const allocator& rv) throw();
#if !defined(_MSC_VER) || (_MSC_VER >= 1400)
    template<typename U> allocator(const allocator<U>& rv) throw();
#endif
    explicit allocator(size_t capacity) throw();
    virtual ~allocator() throw();


    size_type max_size() const throw();
    pointer address(reference value) const throw();
    const_pointer address(const_reference value) const throw(); 

    pointer allocate(size_type size, void* hint = 0);
    void deallocate(pointer location, size_type size);
    void construct(pointer location, const T& value);
    void destroy(pointer p);

  private:
    allocator() throw();
    template<typename U> bool operator==(const allocator<U>&) const throw() { return false; }
    const size_t _capacity;

  };

  template<typename T> allocator<T>::allocator(const allocator& rv) throw() : _capacity(rv._capacity) {
    std::cout << "copy construct allocate<" << typeid(value_type).name() << "> with capacity " << rv._capacity << std::endl;
    std::cout << "=> this = " << this << ", &rv = " << &rv << std::endl;
  }

#if !defined(_MSC_VER) || (_MSC_VER >= 1400)
  template<typename T> 
  template<typename U> 
  allocator<T>::allocator(const allocator<U>& rv) throw() : _capacity(0) {
    std::cout << "convert allocate from <" << typeid(U).name() << "> to <" << typeid(T).name() << ">" << std::endl;
    std::cout << "=> this = " << this << ", &rv = " << &rv << std::endl;
  }
#endif

  template<typename T> allocator<T>::allocator(size_t capacity) throw() : _capacity(capacity) {
    std::cout << "construct allocate<" << typeid(value_type).name() << "> with capacity " << capacity << std::endl;
    std::cout << "=> this = " << this << std::endl;
  }

  template<typename T> allocator<T>::~allocator() throw() {
    // TODO: free ALL memory allocated.
    std::cout << "destruct allocate<" << typeid(value_type).name() << ">with capacity " << _capacity << std::endl;
    std::cout << "=> this = " << this << std::endl;
  }
  
  template<typename T> typename allocator<T>::size_type allocator<T>::max_size() const throw() {
    return _capacity;
  }
    
  template<typename T> typename allocator<T>::pointer allocator<T>::address(reference value) const throw() {
    return &value;
  }

  template<typename T> typename allocator<T>::const_pointer allocator<T>::address(const_reference value) const throw() {
    return &value;
  }

  /* NOT USED private constructor*/
  template<typename T> allocator<T>::allocator() throw() : _capacity(0) {}

  template <typename T>  typename allocator<T>::pointer allocator<T>::allocate(typename allocator<T>::size_type size, void* hint) {
    std::cout << "allocate <" << typeid(value_type).name() << ">: " << size << " elements" << std::endl;
    return static_cast<value_type*>(malloc(size * sizeof(value_type)));
  }
  
  template <typename T> void allocator<T>::deallocate(typename allocator<T>::pointer location, typename allocator<T>::size_type size) {
    std::cout << "deallocate <" << typeid(value_type).name() << ">: " << size << " elements" << std::endl;
    free(location);
  }

  template <typename T> void allocator<T>::construct(typename allocator<T>::pointer location, const T& value) {
    std::cout << "construct <" << typeid(value_type).name() << ">: initial value is " << value << std::endl;
    new ((void*)location) T(value);
  }

  template <typename T> void allocator<T>::destroy(typename allocator<T>::pointer p) {
    std::cout << "destroy <" << typeid(value_type).name() << ">" << std::endl;
    p->~T();
  }
}

#endif /* __APUEX_ALLOCATOR_CXX_INCLUDED_ */

