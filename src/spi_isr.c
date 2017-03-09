#include <avr/interrupt.h>

#include "comms_interface.h"
#include "data_structures.h"
#include "spi.h"

volatile logg_data_packet_t logg_data_packet;
volatile rc_data_packet_t rc_data_packet;

ISR(SPI_STC_vect)
{
  //printf("Inside interrupt.\n");
  uint8_t tempByte = spi_read_data_register();
  switch(tempByte)
  {
    case CMD_RECEIVE_RC_INPUTS: 
      receive_slave_data_packet();   //May need to modify argument name.
      //printf("%d %d %d %d\n", rc_data_packet.channel_0, rc_data_packet.channel_1, rc_data_packet.channel_2, rc_data_packet.channel_3);
      break;
    case CMD_SEND_LOGGING_DATA:
      //printf("Attempt to send.\n");
      send_slave_data_packet();    //May need to modify argument name.
      //printf("pitch: %d, roll: %d, yaw_vel: %d\n", (int)logg_data_packet.pitch.value, (int)logg_data_packet.roll.value, (int)logg_data_packet.yaw_vel.value);
      break;
    // default:
    //   printf("%d\n", tempByte);
    //   break;
  }
}
