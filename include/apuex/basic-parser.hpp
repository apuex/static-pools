#ifndef __APUEX_BASIC_PARSER_CXX_INCLUDED_
#define __APUEX_BASIC_PARSER_CXX_INCLUDED_

#include <apuex/stddef-config.h>


namespace apuex {
  enum CodecState { Consumed, Produced, Completed, NoContent, Rejected };

  template <typename Type, typename Predicate, bool BigEndian = false>
  class BasicParser {
  public:
    typedef Type value_type;
    typedef Type* pointer;
    typedef const Type* const_pointer;
    typedef Type& reference;
    typedef const Type& const_reference;

    BasicParser(pointer p, const Predicate pred)
      : _size(sizeof(value_type))
      , _pointer(reinterpret_cast<uint8_t*>(p))
      , _predicate(pred)
      , _pos(BigEndian ? _size : 0)
    {
      _pos = BigEndian ? _size : 0;
    }
    BasicParser(const BasicParser& r)
      : _size(r._size)
      , _pointer(r._pointer)
      , _predicate(r._predicate)
      , _pos(r._pos) {
    }
    virtual ~BasicParser() { }

    BasicParser& operator=(const BasicParser& r) {
      this->_size = r._size;
      this->_pointer = r._pointer;
      this->_predicate = r._predicate;
      this->_pos = r._pos;
      return *this;
    }

    void assign(pointer p) {
      _pointer = reinterpret_cast<uint8_t*>(p);
      _pos = BigEndian ? _size : 0;
    }

    void assign(pointer p, const Predicate pred) {
      assign(p);
      _predicate = pred;
    }

    CodecState decode(const uint8_t& b) {
      if (BigEndian) {
        if (0 == _pos) return NoContent;
        *(_pointer + _pos - 1) = b;
        _pos--;
        if (0 == _pos) {
          if (_predicate(*_pointer)) return Completed;
          else return Rejected;
        }
        else return Consumed;
      }
      else {
        if (_size == _pos) return NoContent;
        *(_pointer + _pos) = b;
        _pos++;
        if (_size == _pos) {
          if (_predicate(*_pointer)) return Completed;
          else return Rejected;
        }
        else return Consumed;
      }
    }

    CodecState encode(uint8_t& b) {
      if (BigEndian) {
        if (0 == _pos) return NoContent;
        b = *(_pointer + _pos - 1);
        _pos--;
        if (0 == _pos) return Completed;
        else return Consumed;
      }
      else {
        if (_size == _pos) return NoContent;
        b = *(_pointer + _pos);
        _pos++;
        if (_size == _pos) return Completed;
        else return Consumed;
      }
    }

    const_reference value() { return reinterpret_cast<const_reference>(*_pointer); }
    void value(const_reference v) { reinterpret_cast<reference>(*_pointer) = v; }

  private:
    const size_t _size;
    uint8_t *_pointer;
    Predicate _predicate;
    size_t _pos;
  };

}

#endif /* __APUEX_BASIC_PARSER_CXX_INCLUDED_ */

