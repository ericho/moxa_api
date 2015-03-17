#define	HEADER_TO_CARD	        0x55
#define FOOTER_TO_CARD	        0xAA
#define HEADER_FROM_CARD	0x55
#define HEADER_ERROR_FROM_CARD	0xAA
#define FOOTER_FROM_CARD	0xAA

#define WRONG_HEADER		0x50
#define INCORRECT_MSG_SIZE	0x51
#define DUPLICATED_MSG_ID	0x52
#define WRONG_FOOTER		0x53
#define ACK_MSG			0x54

#define COMMAND_SENT		0

#define MIN_MSG_SIZE		5
#define NULL			0
#define WAIT_FOR_CMD_STATE      0
#define CMD_DETECTED_STATE      1

#define COMMAND_RECEIVED        0
#define INCORRECT_FRAME_SIZE    1
#define INCORRECT_HEADER        2
#define INCORRECT_FOOTER        3
#define UNKNOWN_COMMAND         4
#define TIMEOUT_ERROR           5

#include "main.h"

int send_serial_command(unsigned char *dat, int len);
void analize_recv_data(void);
int recv_data();
int send_command(unsigned char cmd, unsigned char *data, int len, unsigned char id);
void print_data_frame(unsigned char *data, int len);
command_t parse_recv_command(unsigned char *buffer, int recv_len);
int _send_bad_command(unsigned char header, unsigned char footer, unsigned char cmd, int len);
