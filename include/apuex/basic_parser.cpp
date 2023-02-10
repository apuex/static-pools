#include <apuex/basic_parser.hpp>
#include <iostream>

namespace apuex {
  std::ostream& operator<<(std::ostream& os, const CodecState v) {
    switch(v) {
    case Consumed:
      os << "Consumed";
      break;
    case Produced:
      os << "Produced";
      break;
    case Completed:
      os << "Completed";
      break;
    case NoContent:
      os << "NoContent";
      break;
    case Rejected:
      os << "Rejected";
      break;
    default:
      os << "Unknown";
      break;
    }
    return os;
  }
}


