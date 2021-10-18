#include <apuex/modbus.hpp>
#include <stdio.h>

using namespace apuex;

int main(int argc, char *argv[]) {
  size_t i;
  uint8_t bytes[] = { 0x01, 0x03, 0x04, 0x4B, 0x00, 0x05, 0xEF, 0xF4 };
  uint8_t bytes_in[] = { 0x08, 0x01, 0x03, 0x04, 0x4B, 0x00, 0x05, 0xEF, 0xF4 };
  uint8_t bytes_out[9];
  CodecState state;

  struct ModbusByteArray input;
  struct ModbusByteArray output;
 
  input.offer(bytes, sizeof(bytes));

  ModbusByteArrayParser parser(input, byte_equal(0x08));

  printf("[ ");
  bool eq_bytes_out = true;
  for (i = 0, state = Produced; i != sizeof(bytes_in) && Completed != state && NoContent != state; ++i ) {
    state = parser.encode(bytes_out[i]);
    printf("%02X ", (0xff & bytes_out[i]));
    eq_bytes_out = (eq_bytes_out && (bytes_in[i] == bytes_out[i]));
  } 
  printf("]\n");

  parser.offer(output, byte_equal(0x08));
  for (i = 0, state = Consumed; i != sizeof(bytes_in) && Completed != state && Rejected != state; ++i ) {
    state = parser.decode(bytes_in[i]);
  } 
  bool eq_bytes_in = true;
  for (i = 0; i != output.length; ++i) {
    eq_bytes_in = (eq_bytes_in && (bytes[i] == output.bytes[i]));
  }

  bool result = !(eq_bytes_out && eq_bytes_in && output == input);
  return result;
}


