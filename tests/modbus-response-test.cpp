#include <apuex/modbus.hpp>
#include <stdio.h>

using namespace apuex;

int main(int argc, char *argv[]) {
  size_t i;
  uint8_t bytes[] = { 0x01, 0x7F, 0x01, 0x80, 0x01, 0x7F, 0x01, 0x7F, 0x00, 0xDE, 0x00, 0xDC, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xDD };
  uint8_t bytes_in[23] = { 0x01, 0x03, 0x12, 0x01, 0x7F, 0x01, 0x80, 0x01, 0x7F, 0x01, 0x7F, 0x00, 0xDE, 0x00, 0xDC, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xDD, 0x9A, 0x6C };
  uint8_t bytes_out[23];
  uint8_t b = 0xff;
  CodecState state;

  struct ModbusRequest req;
  
  req.address = 0x01;
  req.command = 0x03;
  req.startOfRegister = 0x045f;
  req.length = 0x0009;
  req.crc16 = 0xeeb4;

  struct ModbusResponse input(req);
  input.byteArray.assign(bytes, sizeof(bytes));
  input.crc16 = 0x6c9a;

  struct ModbusResponse output(req);
  ModbusResponseParser parser(input);

  printf("[ ");
  bool eq = true;
  for (i = 0, state = Produced; i != sizeof(bytes_out) && Completed != state && NoContent != state; ++i ) {
    state = parser.encode(bytes_out[i]);
    printf("%02X ", (0xff & bytes_out[i]));
    eq = (eq && (bytes_in[i] == bytes_out[i]));
  } 
  printf("]\n");

  parser.assign(output);
  for (i = 0, state = Consumed; i != sizeof(bytes_in) && Completed != state && Rejected != state; ++i ) {
    state = parser.decode(bytes_in[i]);
  } 

  return !(eq && output == input);

  return 0;
}


