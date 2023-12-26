#ifndef __APUEX_RING_BUFFER_CXX_INCLUDED_
#define __APUEX_RING_BUFFER_CXX_INCLUDED_

#include <cstddef>

namespace apuex {

template <typename T>
class ring_buffer {
 public:
  typedef T value_type;
  ring_buffer(size_t size)
     : _buffer_size(size)
     , _buffer(new value_type[size])
     , _element_count(0)
     , _rd_pos(0)
     , _wr_pos(0)
     { }
  ring_buffer(const ring_buffer& rv)
     : _buffer_size(rv._buffer_size)
     , _buffer(new value_type[rv._buffer_size])
     , _element_count(rv._element_count)
     , _rd_pos(rv._rd_pos)
     , _wr_pos(rv._wr_pos) 
     {
       for(size_t i = 0; i != rv._buffer_size; ++i) {
         *(_buffer + i) = *(rv._buffer + i);
       }
     }
  virtual ~ring_buffer() { delete[] _buffer; }

  size_t peek(value_type* rbuf, size_t to_read) {
    if(0 == _element_count) return 0;
    else {
      size_t i = 0;
      size_t peek_pos = _rd_pos;
      for(i = 0; i != to_read && i != _element_count; ++i) {
        *(rbuf + i) = *(_buffer + peek_pos); ++peek_pos;
        if(_buffer_size == peek_pos) peek_pos = 0;
      }
      return i;
    }
  }

  size_t skip(size_t to_read) {
    if(0 == _element_count) return 0;
    else {
      size_t i = 0;
      for(i = 0; i != to_read && i != _element_count; ++i) {
        ++_rd_pos;
        if(_buffer_size == _rd_pos) _rd_pos = 0;
      }
      _element_count -= i;
      return i;
    }
  }

  size_t read(value_type* rbuf, size_t to_read) {
    if(0 == _element_count) return 0;
    else {
      size_t i = 0;
      for(i = 0; i != to_read && i != _element_count; ++i) {
        *(rbuf + i) = *(_buffer + _rd_pos); ++_rd_pos;
        if(_buffer_size == _rd_pos) _rd_pos = 0;
      }
      _element_count -= i;
      return i;
    }
  }


  size_t write(const value_type* const wbuf, size_t to_write) {
    if(_buffer_size == _element_count) return 0;
    else {
      size_t i = 0;
      for(i = 0; i != to_write && i != (_buffer_size - _element_count); ++i) {
        *(_buffer + _wr_pos) = *(wbuf + i); ++_wr_pos;
        if(_buffer_size == _wr_pos) _wr_pos = 0;
      }
      _element_count += i;
      return i;
    }
  }

  bool empty() const { return (0 == _element_count); }
  bool full() const { return (_buffer_size == _element_count); }
  void clear() {
    _element_count = 0;
    _rd_pos = 0;
    _wr_pos = 0;
  }

  size_t buffer_size() const { return _buffer_size; }
  size_t element_count() const { return _element_count; }
  size_t read_pos() const { return _rd_pos; }
  size_t write_pos() const { return _wr_pos; }

 private:
  ring_buffer& operator=(const ring_buffer& rv) { return *this; }
  bool operator==(const ring_buffer& rv) const { return false; }

 private:
  const size_t _buffer_size;
  value_type* _buffer;
  size_t _element_count;
  size_t _rd_pos;
  size_t _wr_pos;
};

}

#endif /* __APUEX_RING_BUFFER_CXX_INCLUDED_ */

