#ifndef __APUEX_YDT1363_CXX_INCLUDED_
#define __APUEX_YDT1363_CXX_INCLUDED_
#include <apuex/basic_parser.hpp>
#include <apuex/byte_buffer.hpp>
#include <apuex/ring_buffer.hpp>
#include <exception>
#include <iostream>
#include <iomanip>

#ifndef YDT1363_MAX_INFO_LENGTH
#define YDT1363_MAX_INFO_LENGTH 0x0fff
#endif
#define YDT1363_MAX_MESSAGE_LENGTH (YDT1363_MAX_INFO_LENGTH + 10)
#define YDT1363_MAX_ASCII_LENGTH ((YDT1363_MAX_INFO_LENGTH + 8) * 2)

namespace apuex {

namespace YDT1363 {

uint16_t toLength(const uint16_t dataLen) {
  uint16_t lengthId = 0xfff & dataLen;
  uint16_t b0 = 0xf & lengthId;
  uint16_t b1 = 0xf & (lengthId >> 4);
  uint16_t b2 = 0xf & (lengthId >> 8);
  uint16_t lchecksum = 0xf & (1 + (~((b0 + b1 + b2) % 16)));

  return 0xffff & ((lchecksum << 12) | lengthId);
}

uint16_t fromLength(const uint16_t length) {
  return 0x0fff & length;
}

uint16_t checksum(const uint8_t *bytes, const uint16_t len) {
  uint16_t sum = 0;
  for(uint16_t i = 0; i != len; ++i) {
    sum += *(bytes + i);
  }
  return (1 + (~sum));
}

inline uint8_t fromHalfByte(const uint8_t c) {
  if(9 >= c) return (c + 0x30);
  else if(0xA <= c && 0xF >= c) return (c + 0x37);
  else throw std::out_of_range("c > 0xF, which is not possible, since input is 4 bits.");
}

inline uint8_t toHalfByte(const uint8_t c) {
  if(0x30 <= c && 0x39 >= c) return (c - 0x30);
  else if(0x41 <= c && 0x46 >= c) return (c - 0x37);
  else if(0x61 <= c && 0x66 >= c) return (c - 0x57);
  else if(0x20 == c) return 0;
  else throw std::out_of_range("c is Not a valid hex char ([0-9A-Ba-b]).");
}

inline uint16_t fromHexChars(
  uint8_t *bytes, const uint16_t bytesLen,
  const uint8_t *chars, const uint16_t charBuffLen
  ) {
  uint16_t i = 0;
  if(  0 == charBuffLen
    || 0 == bytesLen) return 0;
  for(i = 0; i != bytesLen && ((i*2) != charBuffLen); ++i) {
    bytes[i] = 0xff & 
      ( (toHalfByte(chars[2*i]) << 4)
      | (toHalfByte(chars[2*i + 1]))
      );
  }
  return i;
}

inline uint16_t toHexChars(
  uint8_t *chars, const uint16_t charBuffLen, 
  const uint8_t *bytes, const uint16_t bytesLen
  ) {
  uint16_t i = 0;
  if(  0 == charBuffLen
    || 0 == bytesLen) return 0;
  for(i = 0; i != bytesLen && ((2*i) != charBuffLen); ++i) {
    chars [2*i + 1] = fromHalfByte(0xf & (bytes[i]     ));
    chars [2*i    ] = fromHalfByte(0xf & (bytes[i] >> 4));
  }
  return (2 * i);
}

// message definition
struct Header {
public:
  Header()
    : _ver(0x21)
    , _addr(0x01)
    , _cid1(0)
    , _cid2rtn(0)
  {}
  Header(uint8_t ver, uint8_t addr, uint8_t cid1, uint8_t cid2rtn)
    : _ver(ver)
    , _addr(addr)
    , _cid1(cid1)
    , _cid2rtn(cid2rtn)
  {}
  Header(const Header& r)
    : _ver(r._ver)
    , _addr(r._addr)
    , _cid1(r._cid1)
    , _cid2rtn(r._cid2rtn)
  {}
  virtual ~Header()
  {}

  Header& operator=(const Header& r) {
    _ver = r._ver;
    _addr = r._addr;
    _cid1 = r._cid1;
    _cid2rtn = r._cid2rtn;
    return *this;
  }

  bool operator==(const Header& r) const {
    return ( _ver == r._ver
      && _addr == r._addr
      && _cid1 == r._cid1
      && _cid2rtn == r._cid2rtn);
  }

  bool read(byte_buffer& buf) {
    bool result = true;
    if(result) result = buf.readBigEndian(_ver);
    if(result) result = buf.readBigEndian(_addr);
    if(result) result = buf.readBigEndian(_cid1);
    if(result) result = buf.readBigEndian(_cid2rtn);
    return result;
  }

  bool write(byte_buffer& buf) const {
    bool result = true;
    if(result) result = buf.writeBigEndian(_ver);
    if(result) result = buf.writeBigEndian(_addr);
    if(result) result = buf.writeBigEndian(_cid1);
    if(result) result = buf.writeBigEndian(_cid2rtn);
    return result;
  }

  uint8_t _ver;
  uint8_t _addr;
  uint8_t _cid1;
  uint8_t _cid2rtn;
};

struct InfoBytes {
public:
  InfoBytes()
    : _length(0)
  {}
  InfoBytes(const uint8_t *bytes, const uint16_t length)
    : _length(length)
  {
    for(uint16_t i = 0; i != length; ++i) {
      _bytes[i] = bytes[i];
    }
  }
  InfoBytes(const InfoBytes& r)
    : _length(r._length)
  {
    for(uint16_t i = 0; i != r._length; ++i) {
      _bytes[i] = r._bytes[i];
    }
  }
  virtual ~InfoBytes()
  {}

  InfoBytes& operator=(const InfoBytes& r) {
    _length = r._length;
    for(uint16_t i = 0; i != r._length; ++i) {
      _bytes[i] = r._bytes[i];
    }
    return *this;
  }

  bool operator==(const InfoBytes& r) const {
    bool result = (_length == r._length);
    for(uint16_t i = 0; result && i != r._length; ++i) {
      result = result && (_bytes[i] == r._bytes[i]);
    }
    return result;
  }

  bool read(byte_buffer& buf) {
    bool result = true;
    uint16_t length = 0;
    if(result) result = buf.readBigEndian(length);
#ifdef LENGTH_IN_BINARY
    _length = fromLength(length);
#else
    _length = fromLength(length) / 2;
#endif
    for(uint16_t i = 0; result && i != _length; ++i) {
      result = buf.readBigEndian(_bytes[i]);
    }
    return result;
  }

  bool write(byte_buffer& buf) const {
    bool result = true;
#ifdef LENGTH_IN_BINARY
    uint16_t length = toLength(_length);
#else
    uint16_t length = toLength(_length * 2);
#endif
    if(result) result = buf.writeBigEndian(length);
    for(uint16_t i = 0; result && i != _length; ++i) {
      result = buf.writeBigEndian(_bytes[i]);
    }
    return result;
  }

  uint16_t _length;
  uint8_t _bytes[YDT1363_MAX_INFO_LENGTH];
};

struct Message {
public:
  Message()
    : _header()
    , _info()
  { }
  Message(uint8_t ver, uint8_t addr, uint8_t cid1, uint8_t cid2rtn, const uint8_t *bytes, uint16_t length)
    : _header(ver, addr, cid1, cid2rtn)
    , _info(bytes, length)
  { }
  Message(const Message& r)
    : _header(r._header)
    , _info(r._info)
  { }
  virtual ~Message()
  {}

  Message& operator=(const Message& r) {
    _header = r._header;
    _info = r._info;
    return *this;
  }

  bool operator==(const Message& r) const {
    return (_header == r._header
      && _info == r._info);
  }

  bool read(byte_buffer& buf) {
    bool result = true;
    if(result) result = _header.read(buf); else return false;
    if(result) result = _info.read(buf); else return false;
    return result;
  }

  bool write(byte_buffer& buf) const {
    bool result = true;
    if(result) result = _header.write(buf); else return false;
    if(result) result = _info.write(buf); else return false;
    return result;
  }

  Header _header;
  InfoBytes _info;
};

struct Frame {
public:
  Frame()
    :_pos(0)
    , _buffer(_bufferContent, 0, 0, sizeof(_bufferContent)) 
  {}
  Frame(const uint8_t *bytes, const uint16_t length)
    :_pos(0)
    , _buffer(_bufferContent, 0, 0, sizeof(_bufferContent)) 
  {
    offer(bytes, length);
  }
  virtual ~Frame()
  {}

  inline void reset() {
    _pos = 0;
    _buffer.clear();
  }

  CodecState encode(uint8_t &b) {
    switch(_pos) {
    case 0:
      b = 0x7e;
      _pos += 1;
      return Produced;
    case 1:
      if(!_buffer.readBigEndian(b)) {
        // no more byte to produce, produce EOI.
        b = 0x0d;
        _pos += 1;
        return Completed;
      } else {
        return Produced;
      }
    default:
      return NoContent;
    }
  }

  CodecState decode(const uint8_t &b) {
    switch (_pos) {
    case 0:
      if(0x7e == b) {
        // is SOI, transit to next stage.
        _pos += 1;
      } else {
        // just skip invalid header, DO NOT REJECT!!!
      }
      return Consumed;
    case 1:
      if((0x30 <= b && b <= 0x66) || 0x20 == b) {
        // acceptable, append to buffer.
        if(_buffer.writeBigEndian(b)) {
          return Consumed;
        } else {
          // buffer full, Reject
          return Rejected;
        }
      } else if(0x0d == b) { // EOI not included in checksum calculation.
        _pos += 1;
        if(4 < _buffer.element_count()) {
          uint16_t chksum = checksum(_bufferContent, static_cast<uint16_t>(_buffer.element_count() - 4));
          uint8_t chksumBytes[sizeof(uint16_t)];
          uint16_t count = fromHexChars(chksumBytes, sizeof(chksumBytes), _bufferContent + _buffer.element_count() - 4, 4);
          if(sizeof(uint16_t) == count) {
            byte_buffer buf(chksumBytes, 0, sizeof(chksumBytes), sizeof(chksumBytes));
            uint16_t chksumr;
            if(buf.readBigEndian(chksumr) && chksumr == chksum) {
              return Completed;
            }
          }
        }
        return Rejected;
      } else {
        // not acceptable, Reject
        return Rejected;
      }
    default:
      // not acceptable, Reject
      return Rejected;
    }
  }

  void offer(const uint8_t *bytes, const uint16_t length) {
    reset();
    uint16_t hexLength = toHexChars(_bufferContent, sizeof(_bufferContent), bytes, length);
    uint16_t chksum = checksum(_bufferContent, hexLength);
    uint8_t chksumBytes[sizeof(uint16_t)];
    byte_buffer buf(chksumBytes, 0, 0, sizeof(chksumBytes));
    buf.writeBigEndian(chksum);
    toHexChars(_bufferContent + hexLength, sizeof(_bufferContent) - hexLength, chksumBytes, sizeof(chksumBytes));
    // EOI=0x0d is not included.
    _buffer = byte_buffer(_bufferContent, 0, hexLength + 4, sizeof(_bufferContent));
  }

  uint16_t take(uint8_t *buffer, const uint16_t bufferSize) const {
    // skip SOI=0x7e, and checksum 4 bytes ASCII. EOI=0x0d is not included.
    uint16_t bytesLength = fromHexChars(buffer, bufferSize, _bufferContent, static_cast<uint16_t>(_buffer.element_count() - 4));
    return bytesLength;
  }

  size_t _pos;
  uint8_t _bufferContent[YDT1363_MAX_ASCII_LENGTH];
  byte_buffer _buffer;
};

} /* namespace YDT1363 */

} /* namespace apuex */

#endif /* __APUEX_YDT1363_CXX_INCLUDED_ */

