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
  byte_buffer(const byte_buffer& rv)
    : _buffer_size(rv._buffer_size)
    , _buffer(rv._buffer)
    , _element_count(rv._element_count)
    , _pos(rv._pos)
  { }
  virtual ~byte_buffer() { }

  byte_buffer& operator=(const byte_buffer& rv) {
    _buffer_size = rv._buffer_size;
    _buffer = rv._buffer;
    _element_count = rv._element_count;
    _pos = rv._pos;
    return *this;
  }

  // little endian
  bool readUInt8LE(uint8_t &v) {
    return readLittleEndian(&v, sizeof(uint8_t));
  }

  bool writeUInt8LE(const uint8_t &v) {
    return writeLittleEndian(&v, sizeof(uint8_t));
  }

  bool readLittleEndian(uint8_t &v) {
    return readLittleEndian(&v, sizeof(uint8_t));
  }

  bool writeLittleEndian(const uint8_t &v) {
    return writeLittleEndian(&v, sizeof(uint8_t));
  }

  bool readUInt16LE(uint16_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint16_t));
  }

  bool writeUInt16LE(const uint16_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  bool readLittleEndian(uint16_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint16_t));
  }

  bool writeLittleEndian(const uint16_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  bool readUInt32LE(uint32_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint32_t));
  }

  bool writeUInt32LE(const uint32_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  bool readLittleEndian(uint32_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint32_t));
  }

  bool writeLittleEndian(const uint32_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  bool readUInt64LE(uint64_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint64_t));
  }

  bool writeUInt64LE(const uint64_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  bool readLittleEndian(uint64_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint64_t));
  }

  bool writeLittleEndian(const uint64_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  bool readInt8LE(int8_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int8_t));
  }

  bool writeInt8LE(const int8_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int8_t));
  }

  bool readLittleEndian(int8_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int8_t));
  }

  bool writeLittleEndian(const int8_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int8_t));
  }

  bool readInt16LE(int16_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int16_t));
  }

  bool writeInt16LE(const int16_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int16_t));
  }

  bool readLittleEndian(int16_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int16_t));
  }

  bool writeLittleEndian(const int16_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int16_t));
  }

  bool readInt32LE(int32_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int32_t));
  }

  bool writeInt32LE(const int32_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int32_t));
  }

  bool readLittleEndian(int32_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int32_t));
  }

  bool writeLittleEndian(const int32_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int32_t));
  }

  bool readInt64LE(int64_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int64_t));
  }

  bool writeInt64LE(const int64_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int64_t));
  }

  bool readLittleEndian(int64_t &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int64_t));
  }

  bool writeLittleEndian(const int64_t &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int64_t));
  }

  bool readFloat32LE(float &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(float));
  }

  bool writeFloat32LE(const float &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  bool readLittleEndian(float &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(float));
  }

  bool writeLittleEndian(const float &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  bool readFloat64LE(double &v) {
    return readLittleEndian(reinterpret_cast<uint8_t*>(&v), sizeof(double));
  }

  bool writeFloat64LE(const double &v) {
    return writeLittleEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(double));
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
  bool readUInt8BE(uint8_t &v) {
    return readBigEndian(&v, sizeof(uint8_t));
  }

  bool writeUInt8BE(const uint8_t &v) {
    return writeBigEndian(&v, sizeof(uint8_t));
  }

  bool readBigEndian(uint8_t &v) {
    return readBigEndian(&v, sizeof(uint8_t));
  }

  bool writeBigEndian(const uint8_t &v) {
    return writeBigEndian(&v, sizeof(uint8_t));
  }

  bool readUInt16BE(uint16_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint16_t));
  }

  bool writeUInt16BE(const uint16_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  bool readBigEndian(uint16_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint16_t));
  }

  bool writeBigEndian(const uint16_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint16_t));
  }

  bool readUInt32BE(uint32_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint32_t));
  }

  bool writeUInt32BE(const uint32_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  bool readBigEndian(uint32_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint32_t));
  }

  bool writeBigEndian(const uint32_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint32_t));
  }

  bool readUInt64BE(uint64_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint64_t));
  }

  bool writeUInt64BE(const uint64_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  bool readBigEndian(uint64_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(uint64_t));
  }

  bool writeBigEndian(const uint64_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(uint64_t));
  }

  bool readInt8BE(int8_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int8_t));
  }

  bool writeInt8BE(const int8_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int8_t));
  }

  bool readBigEndian(int8_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int8_t));
  }

  bool writeBigEndian(const int8_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int8_t));
  }

  bool readInt16BE(int16_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int16_t));
  }

  bool writeInt16BE(const int16_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int16_t));
  }

  bool readBigEndian(int16_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int16_t));
  }

  bool writeBigEndian(const int16_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int16_t));
  }

  bool readInt32BE(int32_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int32_t));
  }

  bool writeInt32BE(const int32_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int32_t));
  }

  bool readBigEndian(int32_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int32_t));
  }

  bool writeBigEndian(const int32_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int32_t));
  }

  bool readInt64BE(int64_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int64_t));
  }

  bool writeInt64BE(const int64_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int64_t));
  }

  bool readBigEndian(int64_t &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(int64_t));
  }

  bool writeBigEndian(const int64_t &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(int64_t));
  }

  bool readFloat32BE(float &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(float));
  }

  bool writeFloat32BE(const float &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  bool readBigEndian(float &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(float));
  }

  bool writeBigEndian(const float &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(float));
  }

  bool readFloat64BE(double &v) {
    return readBigEndian(reinterpret_cast<uint8_t*>(&v), sizeof(double));
  }

  bool writeFloat64BE(const double &v) {
    return writeBigEndian(reinterpret_cast<const uint8_t*>(&v), sizeof(double));
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

  bool writeBigEndian(const uint8_t *p, size_t size) {
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
    _pos = 0;
    _element_count = 0;
  }

  size_t buffer_size() const { return _buffer_size; }
  size_t element_count() const { return _element_count; }
  size_t pos() const { return _pos; }

 private:
  bool operator==(const byte_buffer& rv) const { return false; }

 private:
  size_t _buffer_size;
  uint8_t* _buffer;
  size_t _element_count;
  size_t _pos;
}; // class byte_buffer

} // namespace apuex

#endif /* __APUEX_BYTE_BUFFER_CXX_INCLUDED_ */

