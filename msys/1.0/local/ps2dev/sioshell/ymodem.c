#include <kernel.h>
#include <sio.h>
#include <string.h>
#include "sio_shell.h"
#include "ymodem.h"

/* Some important defines for YModem */
#define YMODEM_SOH	0x01
#define YMODEM_STX	0x02
#define YMODEM_EOT	0x04
#define YMODEM_ACK	0x06
#define YMODEM_NAK	0x15
#define YMODEM_CAN	0x18
#define YMODEM_C	0x43
#define YMODEM_GET_FAILED	-1

#define CPUCLK      294912000

u32 get_sio_errors(void)
{
	u32 errors;

	errors = _lw(SIO_LSR) & (SIO_LSR_OE | SIO_LSR_PE | SIO_LSR_FE);
	/* Clear the errors */
	_sw(SIO_LSR_FE | SIO_LSR_PE | SIO_LSR_OE, SIO_LSR);

	return errors;
}

int ymodem_getc(void)
{
	int i;
	int ch;

	for(i = 0; i < 10000000; i++)
	{
		ch = sio_getc();
		if(ch >= 0)
		{
			return ch;
		}
	}

	return YMODEM_GET_FAILED;
}

/* Get a character with a timeout in seconds */
int ymodem_getc_timeout(int timeout)
{
	int i;
	int ch = YMODEM_GET_FAILED;

	for(i = 0; i < timeout; i++)
	{
		ch = ymodem_getc();
		if(ch >= 0)
		{
			return ch;
		}
	}

	return YMODEM_GET_FAILED;
}

#define ymodem_getc_minute() ymodem_getc_timeout(60)

const char *u32_to_str(u32 val)
{
	/* Maximum number of decimal digits in u32 is 10 */
	static char num_str[11];
	int  pos = 10;
	num_str[10] = 0;

	if(val == 0)
	{
		/* If already zero then just return zero */
		return "0";
	}

	while((val != 0) && (pos > 0))
	{
		num_str[--pos] = (val % 10) + '0';
		val /= 10;
	}

	return &num_str[pos];
}

int calcrc(const char *ptr, int count)
   {
       int crc, i;

       crc = 0;

       while (--count >= 0) {
           crc = crc ^ (int)*ptr++ << 8;

           for (i = 0; i < 8; ++i)
               if (crc & 0x8000)
                   crc = crc << 1 ^ 0x1021;
               else
                   crc = crc << 1;
           }
       return (crc & 0xFFFF);
   }


void ymodem_send_block(const u8 *data, int block_no)
{
	int count;
	int crc;
	unsigned char block[128];

	memcpy(block, data, 128);
	crc = calcrc(block, 128);
	sio_putc(YMODEM_SOH);
	sio_putc(block_no & 0xFF);
	sio_putc(~block_no & 0xFF);
	for(count = 0; count < 128; count++)
	{
		sio_putc(block[count]);
	}
	sio_putc((crc >> 8) & 0xFF);
	sio_putc(crc & 0xFF);
}

/* Send block 0 (the filename block) */
void ymodem_send_block0(const char* filename, u32 size)
{
	int count;
	unsigned char block[128];
	const char* num;

	memset(block, 0, 128);

	if(filename != NULL)
	{
		count = 0;
		while(*filename)
		{
			block[count++] = *filename++;
		}
		block[count++] = 0;

		num = u32_to_str(size);
		while(*num)
		{
			block[count++] = *num++;
		}
	}

	ymodem_send_block(block, 0);
}


void ymodem_send_data_blocks(const u8* data, u32 size)
{
	int blockno = 1;
	int send_size;
	int ch;

	while(size > 0)
	{
		if(size > 128)
		{
			send_size = 128;
		}
		else
		{
			send_size = size;
		}

		ymodem_send_block(data, blockno);

		ch = ymodem_getc_minute();
		if(ch == YMODEM_ACK)
		{
			blockno++;
			data += send_size;
			size -= send_size;
		}
		else if((ch == YMODEM_CAN) || (ch == -1))
		{
			return;
		}
	}

	do
	{
		sio_putc(YMODEM_EOT);
		ch = ymodem_getc_minute();
	}
	while((ch != YMODEM_ACK) && (ch != -1));

	/* Send last data packet */
	if(ch == YMODEM_ACK)
	{
		ch = ymodem_getc_minute();
		if(ch == YMODEM_C)
		{
			do
			{
				ymodem_send_block0(NULL, 0);
				ch = ymodem_getc_minute();
			} while((ch != YMODEM_ACK) && (ch != -1));
		}
	}
}

void ymodem_send_data(const u8* data, u32 size, const char* filename)
{
	int ch;
	int done;
	/* Assume parameters are all valid... */
	done = 0;
	/* Flush the RX FIFO */
	while(sio_getc() != -1);
	ch = ymodem_getc_minute();
	if(ch == YMODEM_C)
	{
		do
		{
			ymodem_send_block0(filename, size);
			ch = ymodem_getc_minute();
			if(ch == YMODEM_ACK)
			{
				ch = ymodem_getc_minute();
				if(ch == YMODEM_C)
				{
					ymodem_send_data_blocks(data, size);
					done = 1;
				}
			}
			else if(ch != YMODEM_NAK)
			{
				/* Abort */
				sio_putc(YMODEM_CAN);
				sio_putc(YMODEM_CAN);
				done = 1;
			}
		}
		while(!done);
	}
	else
	{
		/* Abort transfer */
	}
}
/* Size of a YMODEM Block, SOH + BLOCKNO + ~BLOCKNO + DATA + CRC */
#define YMODEM_BLOCK_SIZE 133

/* This reads in a single block, checks for errors etc. */
int ymodem_read_block_data(u8* blockdata)
{
	int count;
	int ret = 1;
	int errors;

	for(count = 0; count < YMODEM_BLOCK_SIZE; count++)
	{
		int ch;

		/* 10 seconds timeout */
		ch = ymodem_getc();
		if(ch == -1)
		{
			/* Timeout */
			//printf("Timeout %d\n", count);
			break;
		}
	}

	//errors = get_sio_errors();
	errors = 0;
	if((errors) || (count < YMODEM_BLOCK_SIZE))
	{
		/* Flush the RX FIFO */
		//printf("%d %d\n", count, errors);
		errors = get_sio_errors();
		//printf("%d\n", errors);
		ret = 0;
	}

	/* Flush any remaining characters */
	while(ymodem_getc() != -1);

	return ret;
}

/* Check for data errors */
int ymodem_check_block(u8* block)
{
	u8 bno;
	u8 bno_cmp;
	int ret = 0;
	int crc_val;
	int crc_calc;

	bno = block[1];
	bno_cmp = (~block[2]) & 0xFF;
	/* Check if block number is ok */
	if(bno == bno_cmp)
	{
		crc_val = (block[YMODEM_BLOCK_SIZE-2] << 8) | block[YMODEM_BLOCK_SIZE-1];
		crc_calc = calcrc(&block[3], 128);
		if(crc_val == crc_calc)
		{
			ret = 1;
		}
		else
		{
			//printf("Invalid CRC\n");
		}
	}
	else
	{
		//printf("Invalid block number\n");
	}

	return ret;
}

/* Block should be a pointer to the data bytes */
int ymodem_get_block0_size(u8* block)
{
	int size;
	int count;

	count = 0;
	while((count < 128) && (block[count]))
	{
		count++;
	}

	if((count == 128) || (count == 0))
	{
		return 0;
	}

	count++;
	size = 0;
	while((count < 128) && (block[count] >= '0') && (block[count] <= '9'))
	{
		size *= 10;
		size += (block[count] = '0');
		count++;
	}

	//printf("Receiving file %s %d\n", block, size);

	return size;
}

int recv_data_stream(u8* block, int size)
{
	int i, j;

	j = 0;
	i = 0;
	while((i < size) && (j < 50000000))
	{
		u32 fifo = 0;

		fifo = (_lw(SIO_ISR) & 0xf00) >> 8;
		while(fifo > 0)
		{
			block[i++] = _lb(SIO_RXFIFO);
			fifo--;
		}
		j++;
	}

	return i;
}

#define YMODEM_STATE_SEND_ACK	1
#define YMODEM_STATE_SEND_ACKC	2
#define YMODEM_STATE_SEND_NAK	3

int ymodem_recv_data(u8* data, int max_size)
{
	int ch;
	int blockno;
	int received_block0;
	int sizeleft;
	u8 recv_block[YMODEM_BLOCK_SIZE];
	int count;
	int state = YMODEM_STATE_SEND_NAK;
	int can_received = 0;

	/* Flush the RX FIFO */
	while(sio_getc() != -1);

	received_block0 = 0;
	sizeleft = 0;
	blockno = 1;
	/* Infinite loop */
	while(1)
	{
		/* Clear the sio errors */
		get_sio_errors();
		//while(ymodem_getc() != -1);
		while(sio_getc() != -1);

		if(!received_block0)
		{
			sio_putc(YMODEM_C);
		}
		else
		{
			switch(state)
			{
				case YMODEM_STATE_SEND_ACK: sio_putc(YMODEM_ACK);
											break;
				case YMODEM_STATE_SEND_ACKC: sio_putc(YMODEM_ACK);
											 sio_putc(YMODEM_C);
											 break;
				case YMODEM_STATE_SEND_NAK: sio_putc(YMODEM_NAK);
											break;
				default:					/* WTF */
											break;
			};
		}

		count = recv_data_stream(recv_block, YMODEM_BLOCK_SIZE);

		if(count == 0)
		{
			continue;
		}

		ch = recv_block[0];
		//printf("Block count %d, ch %d\n", count, ch);

		/* Only handle 128 byte blocks, very naughty :P */
		if((ch == YMODEM_SOH) && (count == YMODEM_BLOCK_SIZE))
		{
			//printf("Received Block\n");
			if(ymodem_check_block(recv_block))
			{
				if(!received_block0)
				{
					int filesize;

					filesize = ymodem_get_block0_size(&recv_block[3]);
					if(filesize == 0)
					{
						break;
					}
					else
					{
						sizeleft = filesize > max_size ? max_size : filesize;
						state = YMODEM_STATE_SEND_ACKC;
						received_block0 = 1;
						blockno = 1;
					}
				}
				else
				{
					if((blockno & 0xFF) == recv_block[1])
					{
						if(sizeleft >= 128)
						{
							memcpy(data, &recv_block[3], 128);
							sizeleft -= 128;
							data += 128;
						}
						else if(sizeleft > 0)
						{
							memcpy(data, &recv_block[3], sizeleft);
							sizeleft = 0;
						}

						blockno++;
						state = YMODEM_STATE_SEND_ACK;
					}
					else
					{
						//printf("Invalid block number curr %d, actual %d\n", blockno & 0xFF, recv_block[1]);
						state = YMODEM_STATE_SEND_NAK;
					}
				}
			}
			else
			{
				//printf("Invalid block\n");
				while(ymodem_getc() != -1);
				state = YMODEM_STATE_SEND_NAK;
			}
		}
		else if(ch == YMODEM_STX) /* Don't think it is even legal on the first block */
		{
			//printf("Received STX\n");
			/* Flush serial data */
			while(ymodem_getc() != -1);
			/* Try and send a cancel */
			sio_putc(YMODEM_CAN);
			sio_putc(YMODEM_CAN);
			sio_putc(YMODEM_CAN);
			state = YMODEM_STATE_SEND_NAK;
		}
		else if((ch == YMODEM_CAN) && (get_sio_errors() == 0))
		{
			//printf("Received CAN\n");
			if(count > 1)
			{
				if(recv_block[1] == YMODEM_CAN)
				{
					return 0;
				}
			}
			else
			{
				if(can_received)
				{
					return 0;
				}
				else
				{
					can_received = 1;
					/* Skip back to start to avoid resetting the can state */
					continue;
				}
			}
		}
		else if((ch == YMODEM_EOT) && (get_sio_errors() == 0))
		{
			//printf("Recevied EOT\n");
			sio_putc(YMODEM_ACK);
			received_block0 = 0;
		}
		else
		{
			while(ymodem_getc() != -1);
			state = YMODEM_STATE_SEND_NAK;
		}

		can_received = 0;
	}

	return 0;
}
