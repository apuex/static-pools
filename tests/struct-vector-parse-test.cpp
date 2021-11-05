#include <apuex/basic_parser.hpp>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>

using namespace std;
using namespace apuex;

struct NodeId {
  NodeId() : bb(0), cc(0), dd(0) {}
  NodeId(uint32_t b, uint32_t c, uint32_t d) : bb(b), cc(c), dd(d) {}
  NodeId(const NodeId& v) : bb(v.bb), cc(v.cc), dd(v.dd) {}
  uint32_t dd: 11;
  uint32_t cc: 8;
  uint32_t bb: 13;

  NodeId& operator=(const NodeId& v) {
    bb = v.bb;
    cc = v.cc;
    dd = v.dd;
    return *this;
  }

  bool operator==(const NodeId& v) const {
    return (  bb == v.bb
           && cc == v.cc
           && dd == v.dd);
  }
};

inline std::ostream& operator<<(std::ostream& os, const NodeId& v) {
  os << reinterpret_cast<const int32_t&>(v)
     << "("
     << v.bb
     << v.cc
     << v.dd
     << ")";
  return os;
}

typedef apuex::BasicParser<NodeId> NodeIdParser;

bool testEncode() {
  bool result = true;
  size_t i, size;
  std::vector<NodeId> input;
  for(i = 0; i != 5; ++i) {
    input.push_back(NodeId(0, 0, i + 1));
  }

  uint8_t expected[] = {
    0x05, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00
  };
  uint8_t output[24] = { 0 };

  BasicVectorParser<NodeId, int32_t> parser(input);
  size = parser.encode(output, sizeof(output));
  cout << "[ ";
  i = 0;
  for(; i != size; ++i) {
    cout 
        << std::hex 
        << std::uppercase
        << std::setw(2) 
        << std::setfill('0')
        << (0xff & output[i])
        << " ";
  }
  cout << "]" << endl;
  cout << std::dec << i << " bytes total." << endl;

  if(sizeof(expected) != size) {
    result = false;
  } else {
    for(i = 0; i != sizeof(expected); ++i) {
      result = result && (expected[i] == output[i]);
    }
  }

  return result;
}

bool testDecode() {
  bool result = true;
  size_t i, size;
  std::vector<NodeId> expected;
  for(i = 0; i != 5; ++i) {
    expected.push_back(NodeId(0, 0, i + 1));
  }

  uint8_t input[] = {
    0x05, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00
  };
  std::vector<NodeId> output;

  BasicVectorParser<NodeId, int32_t> parser(output);
  size = parser.decode(input, sizeof(input));

  return (expected == output);
}

int main(int argc, char *argv[]) {
  return !( testEncode()
         && testDecode()
         );
}
