#ifndef __APUEX_YDT1363_CXX_INCLUDED_
#define __APUEX_YDT1363_CXX_INCLUDED_
#include <apuex/basic_parser.hpp>
#include <apuex/byte_buffer.hpp>
#include <apuex/ring_buffer.hpp>
#include <exception>
#include <iostream>
#include <iomanip>
#include <memory>
#include <list>

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

// Serializable
struct Serializable
{
  typedef std::shared_ptr<Serializable> Ptr;
  enum SERIALIZABLE_TYPE
  { SER_UINT8 = 0
  , SER_UINT16
  , SER_UINT32
  , SER_FLOAT32
  , SER_LIST
  , SER_REPLICATABLE
  };
  Serializable(SERIALIZABLE_TYPE type, bool invalid)
      : _type(type)
      , _invalid(invalid)
  {
  }
  virtual ~Serializable()
  {
  }

  virtual int read(const uint8_t * buff, const size_t length, size_t& pos) = 0;
  virtual int write(uint8_t * buff, const size_t length, size_t& pos) = 0;
  virtual Ptr duplicate() = 0;
  virtual bool operator==(const Serializable& n) = 0;

  const bool _type;
  bool _invalid;
};

// UInt8
struct UInt8 : public Serializable
{
  explicit UInt8(uint8_t v)
    : Serializable(SER_UINT8, false)
    , _value(v)
  {}

  explicit UInt8()
    : Serializable(SER_UINT8, true)
    , _value(0)
  {}

  virtual ~UInt8()
  {}

  UInt8& operator=(const UInt8& v)
  {
    this->_invalid = v._invalid;
    this->_value = v._value;
    return (*this);
  }
  int read(const uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      _invalid = false;
      for(size_t i = 0; i != size; ++i)
      {
        if(0x20 == buff[pos + i])
        {
          _invalid = true;
          break;
        }
      }
      if(!_invalid)
      {
        size_t sz = fromHexChars(&_value, sizeof(_value), (buff + pos), (length - pos));
        pos += (2 * sz);
        return (2 * sz);
      }
      else
      {
        pos += size;
        return size;
      }
    }
    return 0;
  }

  int write(uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      if(_invalid)
      {
        for(size_t i = 0; i != size; ++i)
        {
          buff[pos + i] = 0x20;
        }
        pos += size;
        return size;
      }
      else
      {
        size_t sz = toHexChars((buff + pos), (length - pos), &_value, sizeof(_value));
        pos += sz;
        return sz;
      }
    }
    return 0;
  }

  Ptr duplicate()
  {
    UInt8* p = new UInt8();
    p->_invalid = _invalid;
    p->_value = _value;
    Ptr ptr(p);
    return ptr;
  }

  bool operator==(const Serializable& n)
  {
    if(_type == n._type)
    {
      const UInt8& np = dynamic_cast<const UInt8&>(n);
      return (_invalid == np._invalid && _value == np._value);
    }
    return false;
  }

  uint8_t _value;
};

// UInt16
struct UInt16 : public Serializable
{
  explicit UInt16(uint16_t v)
    : Serializable(SER_UINT16, false)
    , _value(v)
  {}

  explicit UInt16()
    : Serializable(SER_UINT16, true)
    , _value(0)
  {}

  virtual ~UInt16()
  {}

  int read(const uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      _invalid = false;
      for(size_t i = 0; i != size; ++i)
      {
        if(0x20 == buff[pos + i])
        {
          _invalid = true;
          break;
        }
      }
      if(!_invalid)
      {
        // FIXME: BE-to-LE - non-portable code, just LE machines..
        uint16_t v;
        uint8_t *src = reinterpret_cast<uint8_t *>(&v);
        uint8_t *dst = reinterpret_cast<uint8_t *>(&_value);
        size_t sz = fromHexChars(src, sizeof(v), (buff + pos), (length - pos));
        for(size_t i = 0; i != sizeof(_value); ++i)
        {
          dst[sizeof(_value) - (i + 1)] = src[i];
        }
        pos += (2 * sz);
        return (2 * sz);
      }
      else
      {
        pos += size;
        return size;
      }
    }
    return 0;
  }

  int write(uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      if(_invalid)
      {
        for(size_t i = 0; i != size; ++i)
        {
          buff[pos + i] = 0x20;
        }
        pos += size;
        return size;
      }
      else
      {
        // FIXME: LE-to-BE - non-portable code, just for LE machines..
        uint16_t v;
        const uint8_t *src = reinterpret_cast<const uint8_t *>(&_value);
        uint8_t *dst = reinterpret_cast<uint8_t *>(&v);
        for(size_t i = 0; i != sizeof(_value); ++i)
        {
          dst[sizeof(_value) - (i + 1)] = src[i];
        }
        size_t sz = toHexChars((buff + pos), (length - pos), dst, sizeof(v));
        pos += sz;
        return sz;
      }
    }
    return 0;
  }

  Ptr duplicate()
  {
    UInt16* p = new UInt16();
    p->_invalid = _invalid;
    p->_value = _value;
    Ptr ptr(p);
    return ptr;
  }

  bool operator==(const Serializable& n)
  {
    if(_type == n._type)
    {
      const UInt16& np = dynamic_cast<const UInt16&>(n);
      return (_invalid == np._invalid && _value == np._value);
    }
    return false;
  }

  uint16_t _value;
};

// UInt32
struct UInt32 : public Serializable
{
  explicit UInt32(uint32_t v)
    : Serializable(SER_UINT32, false)
    , _value(v)
  {}

  explicit UInt32()
    : Serializable(SER_UINT32, true)
    , _value(0)
  {}

  virtual ~UInt32()
  {}

  int read(const uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      _invalid = false;
      for(size_t i = 0; i != size; ++i)
      {
        if(0x20 == buff[pos + i])
        {
          _invalid = true;
          break;
        }
      }
      if(!_invalid)
      {
        // FIXME: BE-to-LE - non-portable code, just LE machines..
        uint32_t v;
        uint8_t *src = reinterpret_cast<uint8_t *>(&v);
        uint8_t *dst = reinterpret_cast<uint8_t *>(&_value);
        size_t sz = fromHexChars(src, sizeof(v), (buff + pos), (length - pos));
        for(size_t i = 0; i != sizeof(_value); ++i)
        {
          dst[sizeof(_value) - (i + 1)] = src[i];
        }
        pos += (2 * sz);
        return (2 * sz);
      }
      else
      {
        pos += size;
        return size;
      }
    }
    return 0;
  }

  int write(uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      if(_invalid)
      {
        for(size_t i = 0; i != size; ++i)
        {
          buff[pos + i] = 0x20;
        }
        pos += size;
        return size;
      }
      else
      {
        // FIXME: LE-to-BE - non-portable code, just for LE machines..
        uint32_t v;
        const uint8_t *src = reinterpret_cast<const uint8_t *>(&_value);
        uint8_t *dst = reinterpret_cast<uint8_t *>(&v);
        for(size_t i = 0; i != sizeof(_value); ++i)
        {
          dst[sizeof(_value) - (i + 1)] = src[i];
        }
        size_t sz = toHexChars((buff + pos), (length - pos), dst, sizeof(v));
        pos += sz;
        return sz;
      }
    }
    return 0;
  }

  Ptr duplicate()
  {
    UInt32* p = new UInt32();
    p->_invalid = _invalid;
    p->_value = _value;
    Ptr ptr(p);
    return ptr;
  }

  bool operator==(const Serializable& n)
  {
    if(_type == n._type)
    {
      const UInt32& np = dynamic_cast<const UInt32&>(n);
      return (_invalid == np._invalid && _value == np._value);
    }
    return false;
  }

  uint32_t _value;
};

// Float32
struct Float32 : public Serializable
{
  explicit Float32(float v)
    : Serializable(SER_FLOAT32, false)
    , _value(v)
  {}

  explicit Float32()
    : Serializable(SER_FLOAT32, true)
    , _value(0)
  {}

  virtual ~Float32()
  {}

  int read(const uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      _invalid = false;
      for(size_t i = 0; i != size; ++i)
      {
        if(0x20 == buff[pos + i])
        {
          _invalid = true;
          break;
        }
      }
      if(!_invalid)
      {
        // FIXME: assuming LE - non-portable code, just LE machines..
        uint8_t *src = reinterpret_cast<uint8_t *>(&_value);
        size_t sz = fromHexChars(src, sizeof(_value), (buff + pos), (length - pos));
        pos += (2 * sz);
        return (2 * sz);
      }
      else
      {
        pos += size;
        return size;
      }
    }
    return 0;
  }

  int write(uint8_t * buff, const size_t length, size_t& pos)
  {
    const size_t size = (2 * sizeof(_value));
    if((length > pos) && (length - pos) >= size)
    {
      if(_invalid)
      {
        for(size_t i = 0; i != size; ++i)
        {
          buff[pos + i] = 0x20;
        }
        pos += size;
        return size;
      }
      else
      {
        // FIXME: assuming LE - non-portable code, just for LE machines..
        const uint8_t *dst = reinterpret_cast<const uint8_t *>(&_value);
        size_t sz = toHexChars((buff + pos), (length - pos), dst, sizeof(_value));
        pos += sz;
        return sz;
      }
    }
    return 0;
  }

  Ptr duplicate()
  {
    Float32* p = new Float32();
    p->_invalid = _invalid;
    p->_value = _value;
    Ptr ptr(p);
    return ptr;
  }

  bool operator==(const Serializable& n)
  {
    if(_type == n._type)
    {
      const Float32& np = dynamic_cast<const Float32&>(n);
      return (_invalid == np._invalid && _value == np._value);
    }
    return false;
  }

  float _value;
};

// list of serializable
// 1. fixed length, pre-loaded std::list, etc.
// 2. replicate N-times.

// List
struct List : public Serializable
{
  explicit List()
    : Serializable(SER_LIST, false)
    , _value()
  {}

  virtual ~List()
  {}

  int read(const uint8_t * buff, const size_t length, size_t& pos)
  {
    size_t sz = 0;
    for(auto it = _value.begin(); it != _value.end(); ++it)
    {
      sz += (*it)->read(buff + sz, length - sz, pos);
    }
    return sz;
  }

  int write(uint8_t * buff, const size_t length, size_t& pos)
  {
    size_t sz = 0;
    for(auto it = _value.begin(); it != _value.end(); ++it)
    {
      sz += (*it)->write(buff + sz, length - sz, pos);
    }
    return sz;
  }

  Ptr duplicate()
  {
    List* p = new List();
    p->_invalid = _invalid;
    for(auto it = _value.begin(); it != _value.end(); ++it)
    {
      p->_value.push_back((*it)->duplicate());
    }
    Ptr ptr(p);
    return ptr;
  }

  bool operator==(const Serializable& n)
  {
    if(_type == n._type)
    {
      const List& np = dynamic_cast<const List&>(n);
      return (_invalid == np._invalid && _value == np._value);
    }
    return false;
  }

  std::list<Serializable::Ptr> _value;
};

// Replicatable
struct Replicatable : public Serializable
{
  explicit Replicatable()
    : Serializable(SER_REPLICATABLE, false)
    , _count()
    , _value(new List())
    , _replica(new List())
  {}

  virtual ~Replicatable()
  {}

  int read(const uint8_t * buff, const size_t length, size_t& pos)
  {
    size_t sz = 0;
    sz += _count.read(buff + sz, length - sz, pos);
    if(!_count._invalid)
    {
      for(size_t i = 0; i != _count._value; ++i)
      {
        //auto pv = _value->
        //sz += (*it)->read(buff + sz, length - sz, pos);
      }
    }
    return sz;
  }

  int write(uint8_t * buff, const size_t length, size_t& pos)
  {
    return 0;
  }

  Ptr duplicate()
  {
    Replicatable* p = new Replicatable();
    p->_invalid = _invalid;
    // assuming non-null smart-pointers.
    p->_count = _count;
    p->_value = _value->duplicate();
    p->_replica = _replica->duplicate();
    Ptr ptr(p);
    return ptr;
  }

  bool operator==(const Serializable& n)
  {
    if(_type == n._type)
    {
      const Replicatable& np = dynamic_cast<const Replicatable&>(n);
      return (_invalid == np._invalid && _value == np._value);
    }
    return false;
  }

  UInt8 _count;
  Serializable::Ptr _value;
  Serializable::Ptr _replica;
};

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
    if(buf.element_count() != _length) result = false; // inconsistency
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
      if( (0x30 <= b && b <= 0x39)
       || (0x41 <= b && b <= 0x46)
       || (0x61 <= b && b <= 0x46)
       || (0x20 == b))
      {
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
          uint16_t count = 0;
          count = fromHexChars(chksumBytes, sizeof(chksumBytes), _bufferContent + _buffer.element_count() - 4, 4);
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

