#ifndef __APUEX_BYTE_BUFFER_CXX_INCLUDED_
#define __APUEX_BYTE_BUFFER_CXX_INCLUDED_

#include <cstddef>
#include <stdexcept>

namespace apuex {

class byte_buffer {
 public:
  byte_buffer(uint8_t *p, size_t offset, size_t elements, size_t size)
     : _buffer_size(size)
     , _buffer(p)
     , _element_count(elements)
     , _pos(offset)
     { }
  virtual ~byte_buffer() { }

  // little endian
  size_t readLittleEndian(uint8_t &v) {
    return readLittleEndian(&v, sizeof(uint8_t));
  }

  size_t writeLittleEndian(const uint8_t &v) {
    return writeLittleEndian(&v, sizeof(uint8_t));
  }

  size_t readLittleEndian(uint16_t &v) {
    return readLittleEndian(&v, sizeof(uint16_t));
  }

  size_t writeLittleEndian(const uint16_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  size_t readLittleEndian(uint32_t &v) {
    return readLittleEndian(&v, sizeof(uint32_t));
  }

  size_t writeLittleEndian(const uint32_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  size_t readLittleEndian(uint64_t &v) {
    return readLittleEndian(&v, sizeof(uint64_t));
  }

  size_t writeLittleEndian(const uint64_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  size_t readLittleEndian(float &v) {
    return readLittleEndian(&v, sizeof(float));
  }

  size_t writeLittleEndian(const float &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  size_t readLittleEndian(double &v) {
    return readLittleEndian(&v, sizeof(double));
  }

  size_t writeLittleEndian(const double &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(double));
  }

  size_t readLittleEndian(uint8_t *p, size_t size) {
    if(size > _element_count) throw std::out_of_range("insufficient bytes left.");
    size_t i = 0;
    for(; i != size; ++i) {
      *(p + i) = *(_buffer + _pos + i);
    }
    _pos += size;
    return size;
  }

  size_t writeLittleEndian(const uint8_t *p, size_t size) {
    if(size > (_buffer_size - _element_count)) throw std::out_of_range("insufficient space left.");
    size_t i = 0;
    for(; i != size; ++i) {
      *(_buffer + _pos + i) = *(p + i);
    }
    _pos += size;
    return size;
  }

  // big endian
  size_t readBigEndian(uint8_t &v) {
    return readBigEndian(&v, sizeof(uint8_t));
  }

  size_t writeBigEndian(uint8_t &v) {
    return writeBigEndian(&v, sizeof(uint8_t));
  }

  size_t readBigEndian(uint16_t &v) {
    return readBigEndian(&v, sizeof(uint16_t));
  }

  size_t writeBigEndian(const uint16_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  size_t readBigEndian(uint32_t &v) {
    return readBigEndian(&v, sizeof(uint32_t));
  }

  size_t writeBigEndian(const uint32_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  size_t readBigEndian(uint64_t &v) {
    return readBigEndian(&v, sizeof(uint64_t));
  }

  size_t writeBigEndian(const uint64_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  size_t readBigEndian(float &v) {
    return readBigEndian(&v, sizeof(float));
  }

  size_t writeBigEndian(const float &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  size_t readBigEndian(double &v) {
    return readBigEndian(&v, sizeof(double));
  }

  size_t writeBigEndian(const double &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(double));
  }

  size_t readBigEndian(uint8_t *p, size_t size) {
    if(size > _element_count) throw std::out_of_range("insufficient bytes left.");
    size_t i = size;
    for(; i != 0; --i) {
      *(p + size - 1) = *(_buffer + _pos + i);
    }
    _pos += size;
    return size;
  }

  size_t writeBigEndian(const uint8_t *p, size_t size) {
    if(size > (_buffer_size - _element_count)) throw std::out_of_range("insufficient space left.");
    size_t i = size;
    for(; i != 0; --i) {
      *(_buffer + _pos + i) = *(p + size - 1);
    }
    _pos += size;
    return size;
  }

  bool empty() const { return (0 == _element_count); }
  bool full() const { return (_buffer_size == _element_count); }
  void clear() {
    _element_count = 0;
  }

  size_t buffer_size() const { return _buffer_size; }
  size_t element_count() const { return _element_count; }
  size_t pos() const { return _pos; }

 private:
  byte_buffer(const byte_buffer& rv) { }
  byte_buffer& operator=(const byte_buffer& rv) { return *this; }
  bool operator==(const byte_buffer& rv) const { return false; }

 private:
  const size_t _buffer_size;
  uint8_t* _buffer;
  size_t _element_count;
  size_t _pos;
};

}

#endif /* __APUEX_BYTE_BUFFER_CXX_INCLUDED_ */

