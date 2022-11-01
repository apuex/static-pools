#ifndef __APUEX_MODBUS_CXX_INCLUDED_
#define __APUEX_MODBUS_CXX_INCLUDED_
#include <apuex/basic_parser.hpp>
#include <iostream>
#include <iomanip>

namespace apuex {

namespace modbus {
  /* MODBUS request, response and byte array */
  struct ModbusRequest {
  public:
    ModbusRequest()
      : address(0)
      , command(0)
      , startOfRegister(0)
      , length(0)
      , crc16(0) { }
    explicit ModbusRequest(uint8_t addr, uint8_t cmd, uint16_t startOfReg, uint16_t len)
      : address(addr)
      , command(cmd)
      , startOfRegister(startOfReg)
      , length(len)
      , crc16(0) { }
    ModbusRequest(const ModbusRequest& r)
      : address(r.address)
      , command(r.command)
      , startOfRegister(r.startOfRegister)
      , length(r.length)
      , crc16(r.crc16) { }
    virtual ~ModbusRequest() { }
    uint8_t address;
    uint8_t command;
    uint16_t startOfRegister;
    uint16_t length;
    uint16_t crc16;

    ModbusRequest& operator=(const ModbusRequest& r) {
      address = r.address;
      command = r.command;
      startOfRegister = r.startOfRegister;
      length = r.length;
      crc16 = r.crc16;
      return *this;
    }
  };

  bool operator==(const ModbusRequest& l, const ModbusRequest& r) {
    return (  l.address == r.address
           && l.command == r.command
           && l.startOfRegister == r.startOfRegister
           && l.length == r.length
           && l.crc16 == r.crc16
           );
  }

  bool operator<(const ModbusRequest& l, const ModbusRequest& r) {
    if (l.address < r.address) return true; else if(l.address > r.address) return false;
    if (l.command < r.command) return true; else if (l.command > r.command) return false;
    if (l.startOfRegister < r.startOfRegister) return true; else if (l.startOfRegister > r.startOfRegister) return false;
    if (l.length < r.length) return true; else if (l.length > r.length) return false;
    if (l.crc16 < r.crc16) return true; else if (l.crc16 > r.crc16) return false;

    return false;
  }
 
  bool ModbusRequestEquals(const ModbusRequest& l, const ModbusRequest& r) {
    return (  l.address == r.address
           && l.command == r.command
           && l.startOfRegister == r.startOfRegister
           && l.length == r.length
           //&& l.crc16 == r.crc16
           );
  }

  struct ModbusByteArray {
  public:
    ModbusByteArray() : length(0) {}
    ModbusByteArray(size_t len) : length(static_cast<uint8_t>(len)) {}
    ModbusByteArray(const ModbusByteArray& r) : length(r.length) {
      for (size_t i = 0; i != r.length; ++i) {
        bytes[i] = r.bytes[i];
      }
    }

    void offer(const uint8_t* ba, size_t len) {
      length = static_cast<uint8_t>(len);
      for (size_t i = 0; i != len && i < 255; ++i) { // i must less than 255
        bytes[i] = ba[i];
      }
    }

    uint8_t length;
    uint8_t bytes[0xff];

    ModbusByteArray& operator=(const ModbusByteArray& r) { 
      length = r.length;
      for (size_t i = 0; i != r.length; ++i) {
        bytes[i] = r.bytes[i];
      }
      return *this; 
    }
  };

  bool operator==(const ModbusByteArray& l, const ModbusByteArray& r) {
    if (l.length != r.length) return false;
    for (size_t i = 0; i != l.length; ++i) {
      if (l.bytes[i] != r.bytes[i]) return false;
    }
    return true;
  }

  uint8_t responseLength(uint8_t cmd, uint16_t length) {
    switch(cmd) {
      case 1:
      case 2:
        {
          uint16_t p = length / 8;
          uint16_t q = (0 == length % 8) ? 0 : 1;
          return (0xff & (p + q));
        }
      case 3:
      case 4:
        return (length * 2);
      default:
        return 0;
    }
  }

  struct ModbusResponse {
    ModbusResponse(const ModbusRequest& req) 
      : address(req.address)
      , command(req.command)
      , byteArray(responseLength(req.command, req.length))
      , crc16(0)
      , request(req)
    { }
    ModbusResponse() 
      : address()
      , command()
      , byteArray()
      , crc16(0)
      , request()
    { }
    ModbusResponse(const ModbusResponse& r) 
      : address(r.address)
      , command(r.command)
      , byteArray(r.byteArray)
      , crc16(r.crc16)
      , request(r.request)
    { }
    virtual ~ModbusResponse() {}
    uint8_t address;
    uint8_t command;
    ModbusByteArray byteArray;
    uint16_t crc16;
    ModbusRequest request;

    ModbusResponse& operator=(const ModbusResponse& r)  {
      address = r.address;
      command = r.command;
      byteArray = r.byteArray;
      crc16 = r.crc16;
      request = r.request;
      return *this;
    }
  };

  bool operator==(const ModbusResponse& l, const ModbusResponse& r)  {
    return (l.address == r.address
      && l.command == r.command
      && l.byteArray == r.byteArray
      && l.crc16 == r.crc16
      && l.request == r.request
      );
  }

  /* VALIDATION predicates */
  struct resp_length_predicate {
    resp_length_predicate(const uint8_t r) : expected(r) { }
    bool operator()(const uint8_t& v) const { return (expected == (0x7f & v)); }
    resp_length_predicate& operator=(const resp_length_predicate& r) {
      expected = r.expected;
      return *this;
    }
    uint8_t expected;
  };
  typedef equal_predicate<uint8_t> byte_equal;
  typedef less_predicate<uint8_t>  byte_less;
  typedef equal_predicate<uint16_t> word_equal;
  typedef bool (*BytePredicate)(const uint8_t&);
  typedef bool (*WordPredicate)(const uint16_t&);

  bool DefaultBytePredicate(const uint8_t& v) {
    return true;
  }

  bool DefaultWordPredicate(const uint16_t& v) {
    return true;
  }

  bool DefaultLengthPredicate(const uint8_t& v) {
    return (255 > v);
  }

  bool DefaultLengthPredicate(const uint16_t& v) {
    return (128 > v);
  }

  /* CRC16 checksum */
  void CRC16(uint16_t& flag, uint16_t& crc16, uint8_t b)
  {
    //WORD  crc = 0xFFFF;
    crc16 = crc16 ^ b;
    for (int j = 0; j < 8; j++)
    {
      flag = crc16 % 2;
      crc16 >>= 1;
      if (flag == 1)
        crc16 ^= 0xA001;
    }
  }

  /* CODECS */
  typedef BasicParser<uint8_t, BytePredicate, false> DefaultByteParser;
  typedef BasicParser<uint16_t, WordPredicate, true> DefaultRegisterParser;
  typedef BasicParser<uint16_t, WordPredicate, false> DefaultCRC16Parser;

  class ModbusRequestParser {
  public:
    ModbusRequestParser()
      : _payload(NULL)
      , _pos(0)
      , _address(NULL, DefaultBytePredicate)
      , _command(NULL, DefaultBytePredicate)
      , _startOfRegister(NULL, DefaultWordPredicate)
      , _length(NULL, DefaultLengthPredicate)
      , _crc16(NULL, DefaultWordPredicate)
      , _crc16_flag(0)
      , _crc16_value(0xFFFF)
    { }
    ModbusRequestParser(ModbusRequest& payload)
      : _payload(&payload)
      , _pos(0)
      , _address(&payload.address, DefaultBytePredicate)
      , _command(&payload.command, DefaultBytePredicate)
      , _startOfRegister(&payload.startOfRegister, DefaultWordPredicate)
      , _length(&payload.length, DefaultLengthPredicate)
      , _crc16(&payload.crc16, DefaultWordPredicate)
      , _crc16_flag(0)
      , _crc16_value(0xFFFF)
    { }
    virtual ~ModbusRequestParser() { }

    void offer(ModbusRequest& payload) {
      _payload = &payload;
      _pos = 0;
      _address.offer(&payload.address);
      _command.offer(&payload.command);
      _startOfRegister.offer(&payload.startOfRegister);
      _length.offer(&payload.length);
      _crc16.offer(&payload.crc16);
      _crc16_flag = 0;
      _crc16_value = 0xFFFF;
    }

    CodecState encode(uint8_t& b) {
      CodecState state = NoContent;
      switch (_pos) {
      case 0:
        state = _address.encode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Produced;
        }
        else return state;
      case 1:
        state = _command.encode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Produced;
        }
        else return state;
      case 2:
        state = _startOfRegister.encode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Produced;
        }
        else return state;
      case 3:
        state = _length.encode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          _crc16.value(_crc16_value);
          ++_pos; return Produced;
        }
        else return state;
      case 4:
        state = _crc16.encode(b);
        if (Completed == state) {
          ++_pos;
        }
        return state;
      default:
        return NoContent;
      }
    }

    CodecState decode(const uint8_t& b) {
      CodecState state = Rejected;
      switch (_pos) {
      case 0:
        state = _address.decode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 1:
        state = _command.decode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 2:
        state = _startOfRegister.decode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 3:
        state = _length.decode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 4:
        state = _crc16.decode(b);
        if (Completed == state) {
          ++_pos;
          if (_crc16.value() != _crc16_value) {
            std::cout
              << "CRC16 expected: 0x"
              << std::hex
              << std::uppercase
              << std::setw(2)
              << std::setfill('0')
              << (0xffff & _crc16.value())
              << ", actual: 0x"
              << (0xffff & _crc16_value)
              << std::endl;
            return Rejected;
          }
        }
        return state;
      default:
        return Rejected;
      }
    }

    void reset() {
      _pos = 0;
      _address.reset();
      _command.reset();
      _startOfRegister.reset();
      _length.reset();
      _crc16.reset();
      _crc16_flag = 0;
      _crc16_value = 0xFFFF;
    }

    ModbusRequest* payload() const { return _payload; }
  private:
    ModbusRequestParser& operator=(const ModbusRequestParser&) { return *this; }
    ModbusRequest* _payload;
    size_t _pos;

    DefaultByteParser _address;
    DefaultByteParser _command;
    DefaultRegisterParser _startOfRegister;
    DefaultRegisterParser _length;
    DefaultCRC16Parser _crc16;
    uint16_t _crc16_flag;
    uint16_t _crc16_value;
  };

  class ModbusByteArrayParser {
  public:
    typedef BasicParser<uint8_t, resp_length_predicate, false> LengthParser;
    explicit ModbusByteArrayParser(ModbusByteArray& payload, const resp_length_predicate& pred)
      : _length(&payload.length, pred)
      , _payload(payload.bytes)
      , _pos(0)
      , _index(0) {}

    void offer(ModbusByteArray& payload) {
      _length.offer(&payload.length);
      _payload = payload.bytes;
      _pos = 0;
      _index = 0;
    }

    void offer(ModbusByteArray& payload, const resp_length_predicate& pred) {
      _length.offer(&payload.length, pred);
      _payload = payload.bytes;
      _pos = 0;
      _index = 0;
    }

    CodecState encode(uint8_t& b) {
      CodecState state = NoContent;
      switch (_pos) {
      case 0:
        state = _length.encode(b);
        if (Completed == state) {
          ++_pos; return Produced;
        }
        else return state;
      case 1:
        b = *(_payload + _index); ++_index;
        if (_length.value() == _index) {
          ++_pos;
          return Completed;
        }
        else {
          return Produced;
        }
      default:
        return NoContent;
      }
    }

    CodecState decode(const uint8_t& b) {
      CodecState state = Rejected;
      switch (_pos) {
      case 0:
        state = _length.decode(b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 1:
        *(_payload + _index) = b; ++_index;
        if (_length.value() == _index) {
          ++_pos;
          return Completed;
        }
        else {
          return Consumed;
        }
      default:
        return Rejected;
      }
    }

    void reset() {
      _length.reset();
      _pos = 0;
      _index = 0;
    }

  private:
    LengthParser _length;
    uint8_t* _payload;
    size_t _pos;
    size_t _index;
  };

  static ModbusByteArray defaultModbusByteArray;

  class ModbusResponseParser {
  public:
    typedef BasicParser<uint8_t, byte_equal, false> AddressParser;
    typedef BasicParser<uint8_t, byte_equal, false> CommandParser;
    ModbusResponseParser() 
      : _payload(NULL)
      , _pos(0)
      , _address(NULL, byte_equal(0))
      , _command(NULL, byte_equal(0))
      , _bytes(defaultModbusByteArray, resp_length_predicate(0))
      , _crc16(NULL, DefaultWordPredicate)
      , _crc16_flag(0)
      , _crc16_value(0xFFFF)
    {}
    ModbusResponseParser(ModbusResponse& payload) 
      : _payload(&payload)
      , _pos(0)
      , _address(&payload.address, byte_equal(payload.request.address))
      , _command(&payload.command, byte_equal(payload.request.command))
      , _bytes(payload.byteArray, resp_length_predicate(responseLength(payload.request.command, payload.request.length)))
      , _crc16(&payload.crc16, DefaultWordPredicate)
      , _crc16_flag(0)
      , _crc16_value(0xFFFF)
    {}
    ModbusResponseParser& operator=(const ModbusResponseParser&rhs) {
      _payload = rhs._payload;
      _pos = rhs._pos;
      _address = rhs._address;
      _command = rhs._command;
      _bytes = rhs._bytes;
      _crc16 = rhs._crc16;
      _crc16_flag = rhs._crc16_flag;
      _crc16_value = rhs._crc16_value;

      return *this;
    }

    void offer(ModbusResponse& payload) {
      _payload = &payload;
      _pos = 0;
      _address.offer(&payload.address, byte_equal(payload.request.address));
      _command.offer(&payload.command, byte_equal(payload.request.command));
      _bytes.offer(payload.byteArray, resp_length_predicate(responseLength(payload.request.command, payload.request.length)));
      _crc16.offer(&payload.crc16, DefaultWordPredicate);
      _crc16_flag = 0;
      _crc16_value = 0xFFFF;
    }

    CodecState encode(uint8_t& b) {
      CodecState state = NoContent;
      switch (_pos) {
      case 0:
        state = _address.encode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; state = Produced;
        }
        break;
      case 1:
        state = _command.encode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; state = Produced;
        }
        break;
      case 2:
        state = _bytes.encode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          _crc16.value(_crc16_value);
          ++_pos; state = Produced;
        }
        break;
      case 3:
        state = _crc16.encode(b);
        if (Completed == state) {
          ++_pos;
        }
        break;
      default:
        return NoContent;
      }
      return state;
    }

    CodecState decode(const uint8_t& b) {
      CodecState state = NoContent;
      switch (_pos) {
      case 0:
        state = _address.decode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 1:
        state = _command.decode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 2:
        state = _bytes.decode(b);
        CRC16(_crc16_flag, _crc16_value, b);
        if (Completed == state) {
          ++_pos; return Consumed;
        }
        else return state;
      case 3:
        state = _crc16.decode(b);
        if (Completed == state) {
          ++_pos;
          if (_crc16.value() != _crc16_value) return Rejected;
        }
        return state;
      default:
        return NoContent;
      }
    }

    void reset() {
      _pos = 0;
      _address.reset();
      _command.reset();
      _bytes.reset();
      _crc16.reset();
      _crc16_flag = 0;
      _crc16_value = 0xFFFF;
    }

    ModbusResponse* payload() const { return _payload; }
  private:
    ModbusResponse* _payload;
    size_t _pos;

    AddressParser _address;
    CommandParser _command;
    ModbusByteArrayParser _bytes;
    DefaultCRC16Parser _crc16;
    uint16_t _crc16_flag;
    uint16_t _crc16_value;
  };

} /* namespace modbus */

} /* namespace apuex */

#endif /* __APUEX_MODBUS_CXX_INCLUDED_ */

