#ifndef __APUEX_BYTE_BUFFER_CXX_INCLUDED_
#define __APUEX_BYTE_BUFFER_CXX_INCLUDED_

#include <apuex/stddef_config.h>
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
  bool readLittleEndian(uint8_t &v) {
    return readLittleEndian(&v, sizeof(uint8_t));
  }

  bool writeLittleEndian(const uint8_t &v) {
    return writeLittleEndian(&v, sizeof(uint8_t));
  }

  bool readLittleEndian(uint16_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint16_t));
  }

  bool writeLittleEndian(const uint16_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  bool readLittleEndian(uint32_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint32_t));
  }

  bool writeLittleEndian(const uint32_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  bool readLittleEndian(uint64_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint64_t));
  }

  bool writeLittleEndian(const uint64_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  bool readLittleEndian(float &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(float));
  }

  bool writeLittleEndian(const float &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  bool readLittleEndian(double &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(double));
  }

  bool writeLittleEndian(const double &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(double));
  }

  bool readLittleEndian(uint8_t *p, size_t size) {
    if(size > _element_count) return false;
    for(size_t i = 0; i != size; ++i) {
      *(p + i) = *(_buffer + _pos + i);
    }
    _pos += size;
    _element_count -= size;
    return true;
  }

  bool writeLittleEndian(const uint8_t *p, size_t size) {
    if(size > (_buffer_size - _element_count)) return false;
    for(size_t i = 0; i != size; ++i) {
      *(_buffer + _pos + i) = *(p + i);
    }
    _pos += size;
    _element_count += size;
    return true;
  }

  // big endian
  bool readBigEndian(uint8_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint8_t));
  }

  bool writeBigEndian(uint8_t &v) {
    return writeBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint8_t));
  }

  bool readBigEndian(uint16_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint16_t));
  }

  bool writeBigEndian(const uint16_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  bool readBigEndian(uint32_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint32_t));
  }

  bool writeBigEndian(const uint32_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  bool readBigEndian(uint64_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint64_t));
  }

  bool writeBigEndian(const uint64_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  bool readBigEndian(float &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(float));
  }

  bool writeBigEndian(const float &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  bool readBigEndian(double &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(double));
  }

  bool writeBigEndian(const double &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(double));
  }

  bool readBigEndian(uint8_t *p, size_t size) {
    if(size > _element_count) return false;
    for(size_t i = 0; i != size; ++i) {
      *(p + size - 1 - i) = *(_buffer + _pos + i);
    }
    _pos += size;
    _element_count -= size;
    return true;
  }

  size_t writeBigEndian(const uint8_t *p, size_t size) {
    if(size > (_buffer_size - _element_count)) return false;
    for(size_t i = 0; i != size; ++i) {
      *(_buffer + _pos + i) = *(p + size - 1 - i);
    }
    _pos += size;
    _element_count += size;
    return true;
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
  byte_buffer(const byte_buffer& rv)
     : _buffer_size(rv._buffer_size)
     , _buffer(rv._buffer)
     , _element_count(rv._element_count)
     , _pos(rv._pos)
     { }
  byte_buffer& operator=(const byte_buffer& rv) { return *this; }
  bool operator==(const byte_buffer& rv) const { return false; }

 private:
  const size_t _buffer_size;
  uint8_t* _buffer;
  size_t _element_count;
  size_t _pos;
}; // class byte_buffer

} // namespace apuex

#endif /* __APUEX_BYTE_BUFFER_CXX_INCLUDED_ */

