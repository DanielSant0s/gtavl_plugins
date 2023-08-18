/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2005 Dan Peori <peori@oopo.net>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/

#ifndef __PACKET_H__
#define __PACKET_H__

 #include <tamtypes.h>

 #include <dma.h>

#ifdef __cplusplus
extern "C" {
#endif

 typedef struct { int size, count; u8 *data; } PACKET;

 //////////////////////
 // PACKET FUNCTIONS //
 //////////////////////

 int packet_allocate(PACKET *packet, int size);
 // Allocate a new packet for use.

 int packet_reset(PACKET *packet);
 // Reset the packet, clearing its contents.

 int packet_append_8(PACKET *packet, u8 data);
 // Append an 8-bit value to the packet.

 int packet_append_16(PACKET *packet, u16 data);
 // Append a 16-bit value to the packet.

 int packet_append_32(PACKET *packet, u32 data);
 // Append a 32-bit value to the packet.

 int packet_append_64(PACKET *packet, u64 data);
 // Append a 64-bit value to the packet.

 int packet_send(PACKET *packet, int dma_channel, int dma_flags);
 // Send the packet to the specified dma channel.

 int packet_free(PACKET *packet);
 // Free the packet.

#ifdef __cplusplus
}
#endif

#endif
