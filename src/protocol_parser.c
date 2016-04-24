/*
 * Laboratorio Nacional de informatica Avanzada LANIA A.C.
 * - Laboratorio de sensores y sistemas embebidos
 *
 * API para el desarrollo de aplicaciones utilizando la tarjeta
 * adaptadora de protocolo ZigBee a RS232
 *
 * Codigo basado en programas de ejemplo de la API de MOXA
 *
*/

#include "protocol_parser.h"

static int recv_frame_len;
command_t cmd_temp;
uint8_t *prepare_command(uint8_t cmd, uint8_t id, uint8_t *data, int len);

void analize_recv_data(void)
{
	if (serial_buffer_cont >= 3) {
		if (cmd_state == WAIT_FOR_CMD_STATE) {
			if (app_serial_buffer[0] == HEADER_FROM_CARD) {
				recv_frame_len = (int) app_serial_buffer[1] << 8;
				recv_frame_len |= (int) app_serial_buffer[2];
				if (recv_frame_len >= 6 && recv_frame_len <= 261)
					cmd_state = CMD_DETECTED_STATE;
			}
		}
		cmd_state = WAIT_FOR_CMD_STATE;
		serial_buffer_cont = 0;
	}
	return;
}

int recv_data()
{
	unsigned char buffer_data[256];
	unsigned char tmp;
	int buffer_pos = 0;
	memset(buffer_data, 0, 256);
	cmd_state = WAIT_FOR_CMD_STATE;

	while(serial_read_byte(&tmp, 1)) {
		printf("C : %x\n", tmp);
		buffer_data[buffer_pos] = tmp;
		if (buffer_pos > 3) {
			if (cmd_state == WAIT_FOR_CMD_STATE) {
				if (buffer_data[0] == HEADER_FROM_CARD) {
					recv_frame_len = (int) buffer_data[1] << 8;
					recv_frame_len = (int) buffer_data[2];
					if (recv_frame_len >= 6 && recv_frame_len < 256)
						cmd_state = CMD_DETECTED_STATE;
					else
						return INCORRECT_FRAME_SIZE;
				}
				else {
					return INCORRECT_HEADER;
				}
			}
			else if (cmd_state == CMD_DETECTED_STATE) {
				if (buffer_pos == recv_frame_len - 1) {
					if (buffer_data[recv_frame_len - 1] == FOOTER_FROM_CARD) {
						cmd_temp = parse_recv_command(buffer_data, recv_frame_len);
						printf("Command received!\n");
						return COMMAND_RECEIVED;
					}
					else {
						printf("Entering here : %d\n", buffer_data[recv_frame_len - 1]);
						return INCORRECT_FOOTER;
					}
				}
			}
		}
		buffer_pos++;
	}
	return TIMEOUT_ERROR;
}

command_t parse_recv_command(unsigned char *buffer, int recv_len)
{
	command_t cmd_t;
	int i;
	if (recv_len >= 6) {
		cmd_t.cmd = *(buffer + 4);
		cmd_t.id = *(buffer + 3);
		if (recv_len >= 6) {
			cmd_t.len = recv_len - 6;
			if (cmd_t.len > 0) {
				cmd_t.data = (unsigned char*) malloc(sizeof(unsigned char)*cmd_t.len);
				for (i=0; i<cmd_t.len; i++)
					*(cmd_t.data + i) = *(buffer + 5 + i);
			}
		}
	}
	return cmd_t;
}
/* send_command : Send commands with the specified parameters
 * unsigned char cmd : The command to be send.
 * unsigned chat *data : The data to be sent. 
 * int len: The length of *data
 * unsigned char id : The id message field.  
 *
 */
int send_command(uint8_t cmd, uint8_t *data, int len, uint8_t id)
{
	uint8_t *tmp;
	int i;
	// Min length 6
	
	tmp = prepare_command(cmd, id, data, len);
	      
	if (tmp == NULL)
		return -ENOMEM;
	
	i = write_data_frame(tmp, len + 6);
	print_data_frame(tmp, len + 6);
	
	free(tmp);
	
	return i;
}

uint8_t *prepare_command(uint8_t cmd, uint8_t id, uint8_t *data, int len)
{
	uint8_t *tmp;
	int i;
	tmp = (uint8_t*) malloc(sizeof(uint8_t) * (len + 6));
	if (tmp == NULL)
		return tmp;
	memset(tmp, 0, sizeof(uint8_t) * (len + 6));

	*tmp = HEADER_TO_CARD;
	*(tmp + 1) = (uint8_t) ((len + 6) >> 8) & 0x00ff;
	*(tmp + 2) = (uint8_t) (6 + len) & 0x00ff;
	*(tmp + 3) = id;
	*(tmp + 4) = cmd;
	
	for (i=0; i<len; i++)
		*(tmp + 5 + i) = (uint8_t) *(data + i);

	*(tmp + 5 + len) = FOOTER_TO_CARD;

	return tmp;
}

/*
 * Send incorrect data to device for testing purposes. 
 */
int _send_bad_command(unsigned char header, unsigned char footer, unsigned char cmd, int len) {
	unsigned char *msg;
	int ret;
	// This messages always be 6 bytes length
	msg = (unsigned char *) malloc(sizeof(char) * 6);
	*msg = header;
	*(msg + 1) = (unsigned char) (len >> 8) & 0x00ff;
	*(msg + 2) = (unsigned char) len & 0x00ff;
	*(msg + 3) = 0;
	*(msg + 4) = cmd;
	*(msg + 5) = footer;
	ret = write_data_frame(msg, 6);

	return ret;
}

void print_data_frame(unsigned char *data, int len)
{
	int i;
	if (len >= 6) {
		printf("Data\n");
		for (i=0; i<len; i++)
			printf("%x ", *(data + i));
		printf("\n");
	}
}

int write_data_frame(unsigned char *data, int len)
{
	int i;
	if (len >= 6) {
		for (i=0; i<len; i++)
			serial_write_byte(*(data + i));
	}
	else {
		return INCORRECT_MSG_SIZE;
	}
	return COMMAND_SENT;
}
