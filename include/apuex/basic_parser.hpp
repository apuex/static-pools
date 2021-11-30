#ifndef __APUEX_BASIC_PARSER_CXX_INCLUDED_
#define __APUEX_BASIC_PARSER_CXX_INCLUDED_

#include <apuex/stddef_config.h>
#include <vector>

namespace apuex {
  enum CodecState { Consumed, Produced, Completed, NoContent, Rejected };

  /* VALIDATION predicates */
  template<typename T>
  struct equal_predicate {
    equal_predicate(const T r) : expected(r) { }
    bool operator()(const T& v) const { return (expected == v); }
    equal_predicate& operator=(const equal_predicate& r) {
      expected = r.expected;
      return *this;
    }
    T expected;
  };

  template<typename T>
  struct less_predicate {
    explicit less_predicate(const T r) : expected(r) { }
    bool operator()(const T& v) const { return (v < expected); }
    less_predicate& operator=(const less_predicate& r) {
      expected = r.expected;
      return *this;
    }
    T expected;
  };

  template <typename T>
  struct NullPredicate {
    NullPredicate() {}
    NullPredicate(T v) {}
    bool operator()(const T& value) const { return true; }
  };

  template <typename Type, typename Predicate=NullPredicate<Type>, bool BigEndian = false>
  class BasicParser {
  public:
    typedef Type value_type;
    typedef Type* pointer;
    typedef const Type* const_pointer;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef NullPredicate<value_type> AllSuffice;

    BasicParser(reference p, const Predicate pred=AllSuffice())
      : _size(sizeof(value_type))
      , _pointer(reinterpret_cast<uint8_t*>(&p))
      , _predicate(pred)
      , _pos(BigEndian ? _size : 0)
    {
      _pos = BigEndian ? _size : 0;
    }
    BasicParser(pointer p, const Predicate pred=AllSuffice())
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

    inline void reset() {
      _pos = BigEndian ? _size : 0;
    }

    inline void offer(pointer p) {
      _pointer = reinterpret_cast<uint8_t*>(p);
      reset();
    }

    inline void offer(pointer p, const Predicate pred) {
      offer(p);
      _predicate = pred;
    }

    inline value_type take() { return value(); }

    inline CodecState decode(const uint8_t& b) {
      if (BigEndian) {
        if (0 == _pos) return NoContent;
        *(_pointer + _pos - 1) = b;
        _pos--;
        if (0 == _pos) {
          _pos = _size;
          if (_predicate(*reinterpret_cast<pointer>(_pointer))) return Completed;
          else return Rejected;
        }
        else return Consumed;
      }
      else {
        if (_size == _pos) return NoContent;
        *(_pointer + _pos) = b;
        _pos++;
        if (_size == _pos) {
          _pos = 0;
          if (_predicate(*reinterpret_cast<pointer>(_pointer))) return Completed;
          else return Rejected;
        }
        else return Consumed;
      }
    }

    inline CodecState encode(uint8_t& b) {
      if (BigEndian) {
        if (0 == _pos) return NoContent;
        b = *(_pointer + _pos - 1);
        _pos--;
        if (0 == _pos) {
          _pos = _size;
          return Completed;
        }
        else return Consumed;
      }
      else {
        if (_size == _pos) return NoContent;
        b = *(_pointer + _pos);
        _pos++;
        if (_size == _pos) {
          _pos = 0;
          return Completed;
        }
        else return Consumed;
      }
    }

  inline size_t encode(uint8_t* const bytes, const size_t maxBytesLength) {
    size_t i = 0;
    for(CodecState state = Produced;
        Completed != state && i != maxBytesLength;
        ++i) state = encode(*(bytes + i));
    return i;
  }

  inline size_t decode(const uint8_t* const bytes, const size_t maxBytesLength) {
    size_t i = 0;
    for(CodecState state = Consumed;
        Completed != state && i != maxBytesLength;
        ++i) state = decode(*(bytes + i));
    return i;
  }

    inline const_reference value() { return reinterpret_cast<const_reference>(*_pointer); }
    inline void value(const_reference v) { reinterpret_cast<reference>(*_pointer) = v; }

  private:
    const size_t _size;
    uint8_t *_pointer;
    Predicate _predicate;
    size_t _pos;
  };

  template <typename Type, typename LengthType, typename Predicate=NullPredicate<std::vector<Type> >, bool BigEndian = false>
  class BasicVectorParser {
   public:
    typedef Type ElementType;
    typedef BasicParser<Type, NullPredicate<Type>, BigEndian> ElementParser;
    typedef BasicParser<LengthType, NullPredicate<LengthType>, BigEndian> LengthParser;
    typedef std::vector<Type> value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef NullPredicate<value_type> AllSuffice;

    BasicVectorParser(reference p, const Predicate pred=AllSuffice())
      : _size(static_cast<LengthType>(p.size()))
      , _element()
      , _index(0)
      , _pointer(&p)
      , _predicate(pred)
      , _pos(0)
      , _lengthParser(_size)
      , _elementParser(_element)
    {
    }
    BasicVectorParser(pointer p, const Predicate pred=AllSuffice())
      : _size(p->size())
      , _element()
      , _index(0)
      , _pointer(p)
      , _predicate(pred)
      , _pos(0)
      , _lengthParser(_size)
      , _elementParser(_element)
    {
    }
    BasicVectorParser(const BasicVectorParser& r)
      : _size(r._size)
      , _element(r._element)
      , _index(r._index)
      , _pointer(r._pointer)
      , _predicate(r._predicate)
      , _pos(r._pos) 
      , _lengthParser(_size)
      , _elementParser(_element) {
    }
    virtual ~BasicVectorParser() { }

    BasicVectorParser& operator=(const BasicVectorParser& r) {
      this->_size = r._size;
      this->_pointer = r._pointer;
      this->_predicate = r._predicate;
      this->_pos = r._pos;
      return *this;
    }

    inline CodecState decode(const uint8_t& b) {
      CodecState state = Rejected;
      switch(_pos) {
        case 0: // length
          state = _lengthParser.decode(b);
          if (Completed == state) {
            if (_size == _index) {
              reset();
              return Completed;
            }
            else {
              _pointer->resize(_size);
              _element = (*_pointer)[_index];
            }
            ++_pos; return Consumed;
          }
          return state;
        case 1: // elements
          state = _elementParser.decode(b);
          if (Completed == state) {
            (*_pointer)[_index] = _element;
            ++_index;
            _elementParser.reset();
            if (_size == _index) {
              reset(); return Completed;
            }
            state = Consumed;
          }
          return state;
        default:
          reset();
          break;
      }
      return Rejected;
    }

    inline CodecState encode(uint8_t& b) {
      CodecState state = NoContent;
      switch(_pos) {
        case 0: // length
          state = _lengthParser.encode(b);
          if(Completed == state) {
            if (_size == _index) {
              reset();
              return Completed;
            }
            else {
              _element = (*_pointer)[_index];
            }
            ++_pos; return Produced;
          }
          return state;
        case 1: // elements
          state = _elementParser.encode(b);
          if (Completed == state) {
            ++_index;
            _elementParser.reset();
            if (_size == _index) {
              reset(); return Completed;
            }
            else {
              _element = (*_pointer)[_index];
            }
            state = Produced;
          }
          return state;
        default:
          reset();
          break;
      }
      return NoContent;
    }

    void reset() {
      _pos = 0;
      _size = 0;
      _index = 0;
      _lengthParser.reset();
      _elementParser.reset();
    }

    inline void offer(pointer p) {
      _pointer = reinterpret_cast<uint8_t*>(p);
      _pos = BigEndian ? _size : 0;
    }

    inline void offer(pointer p, const Predicate pred) {
      offer(p);
      _predicate = pred;
    }

    inline value_type take() { return value(); }

    inline size_t encode(uint8_t* const bytes, const size_t maxBytesLength) {
      size_t i = 0;
      for(CodecState state = Produced;
          Completed != state && i != maxBytesLength;
          ++i) state = encode(*(bytes + i));
      return i;
    }

    inline size_t decode(const uint8_t* const bytes, const size_t maxBytesLength) {
      size_t i = 0;
      for(CodecState state = Consumed;
          Completed != state && i != maxBytesLength;
          ++i) state = decode(*(bytes + i));
      return i;
    }

    inline const_reference value() { return reinterpret_cast<const_reference>(*_pointer); }
    inline void value(const_reference v) { reinterpret_cast<reference>(*_pointer) = v; }

   private:
    LengthType _size;
    ElementType _element;
    LengthType _index;
    pointer _pointer;
    Predicate _predicate;
    size_t _pos;
    LengthParser _lengthParser;
    ElementParser _elementParser;
  };
}

#endif /* __APUEX_BASIC_PARSER_CXX_INCLUDED_ */

