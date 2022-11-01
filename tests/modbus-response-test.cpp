#include <apuex/modbus.hpp>
#include <stdio.h>

using namespace apuex;
using namespace apuex::modbus;

bool parseRegister02() {
  size_t i;
  uint8_t bytes[] = { 0xff, 0xff };
  uint8_t bytes_in[7] = { 0x01, 0x02, 0x02, 0xff, 0xff, 0xB8, 0x08 };
  uint8_t bytes_out[7];
  uint8_t b = 0xff;
  CodecState state;

  struct ModbusRequest req(0x01, 0x02, 0x045f, 0x0009);
  
  struct ModbusResponse input(req);
  input.byteArray.offer(bytes, sizeof(bytes));
  input.crc16 = 0x08b8;

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

  parser.offer(output);
  for (i = 0, state = Consumed; i != sizeof(bytes_in) && Completed != state && Rejected != state; ++i ) {
    state = parser.decode(bytes_in[i]);
  } 
  eq = (Completed == state) && eq;
  return (eq && output == input);
}

bool parseRegister03() {
  size_t i;
  uint8_t bytes[] = { 0x01, 0x7F, 0x01, 0x80, 0x01, 0x7F, 0x01, 0x7F, 0x00, 0xDE, 0x00, 0xDC, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xDD };
  uint8_t bytes_in[23] = { 0x01, 0x03, 0x12, 0x01, 0x7F, 0x01, 0x80, 0x01, 0x7F, 0x01, 0x7F, 0x00, 0xDE, 0x00, 0xDC, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xDD, 0x9A, 0x6C };
  uint8_t bytes_out[23];
  uint8_t b = 0xff;
  CodecState state;

  struct ModbusRequest req(0x01, 0x03, 0x045f, 0x0009);
  
  struct ModbusResponse input(req);
  input.byteArray.offer(bytes, sizeof(bytes));
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

  parser.offer(output);
  for (i = 0, state = Consumed; i != sizeof(bytes_in) && Completed != state && Rejected != state; ++i ) {
    state = parser.decode(bytes_in[i]);
  } 
  eq = (Completed == state) && eq;
  return (eq && output == input);
}

bool parseRegister04() {
  size_t i;
  uint8_t bytes[] = { 0x01, 0x7F, 0x01, 0x80, 0x01, 0x7F, 0x01, 0x7F, 0x00, 0xDE, 0x00, 0xDC, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xDD };
  uint8_t bytes_in[23] = { 0x01, 0x04, 0x12, 0x01, 0x7F, 0x01, 0x80, 0x01, 0x7F, 0x01, 0x7F, 0x00, 0xDE, 0x00, 0xDC, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xDD, 0x2F, 0xDB };
  uint8_t bytes_out[23];
  uint8_t b = 0xff;
  CodecState state;

  struct ModbusRequest req(0x01, 0x04, 0x045f, 0x0009);
  
  struct ModbusResponse input(req);
  input.byteArray.offer(bytes, sizeof(bytes));
  input.crc16 = 0xDB2E;

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

  parser.offer(output);
  for (i = 0, state = Consumed; i != sizeof(bytes_in) && Completed != state && Rejected != state; ++i ) {
    state = parser.decode(bytes_in[i]);
  } 
  eq = (Completed == state) && eq;
  return (eq && output == input);
}

int main(int argc, char *argv[]) {
  return !(parseRegister02() && parseRegister03() && parseRegister04());
}


