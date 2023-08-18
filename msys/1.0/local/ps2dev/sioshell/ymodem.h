#ifndef __YMODEM_H__
#define __YMODEM_H__

void ymodem_send_data(const u8* data, u32 size, const char* filename);
int ymodem_recv_data(u8* data, int max_size);

#endif
