#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "comms_interface.h"
#include "data_structures.h"
#include "spi.h"

volatile logg_data_packet_t logg_data_packet;
volatile rc_data_packet_t rc_data_packet;

#define STATE_IDLE 0
#define STATE_RECEIVE_RC 1
#define STATE_SEND_LOGGING 2
volatile uint8_t spi_state = STATE_IDLE;
volatile uint8_t spi_byte_count = 0;

ISR(SPI_STC_vect)
{
  //puts("spi interrupt");
  //UDR0 = 'C';

  // We've just received a byte.
  uint8_t byte = SPDR;
  switch (spi_state) {
  case STATE_IDLE:
  default:
    // It's a command byte.
    switch (byte) {
    case CMD_RECEIVE_RC_INPUTS:
      // Prepare to receive byte 0 of RC data packet.
      spi_state = STATE_RECEIVE_RC;
      spi_byte_count = 0;
      break;
    case CMD_SEND_LOGGING_DATA:
      // Prepare to send byte 0 of log data packet.
      spi_state = STATE_SEND_LOGGING;
      spi_byte_count = 0;
      SPDR = logg_data_packet.roll.bytes[0];
      break;
    }
    break;

  case STATE_RECEIVE_RC:
    // We've just received byte n of RC data packet.
    ((uint8_t*) &rc_data_packet)[spi_byte_count] = byte;
    // Prepare to receive byte n+1.
    spi_byte_count++;
    if (spi_byte_count == sizeof(rc_data_packet)) {
      spi_state = STATE_IDLE;
    }
    break;

  case STATE_SEND_LOGGING:
    // We've just received a dummy value; however, we sent byte n at the same time.
    // Prepare to send byte n+1.
    spi_byte_count++;
    if (spi_byte_count == sizeof(logg_data_packet)) {
      spi_state = STATE_IDLE;
    }
    else {
      if (spi_byte_count <= 3) {
        SPDR = logg_data_packet.roll.bytes[spi_byte_count];
      }
      else if (spi_byte_count <= 7) {
        SPDR = logg_data_packet.pitch.bytes[spi_byte_count - 4];
      }
      else if (spi_byte_count <= 11) {
        SPDR = logg_data_packet.yaw_vel.bytes[spi_byte_count - 8];
      }
    }
    break;
  }
}
