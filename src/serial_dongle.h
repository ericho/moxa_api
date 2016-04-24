/*
 * Laboratorio Nacional de informatica Avanzada LANIA A.C.
 * - Laboratorio de sensores y sistemas embebidos
 * 
 * API para el desarrollo de aplicaciones utilizando la tarjeta
 * adaptadora de protocolo ZigBee a RS232
 * 
 * Codigo basado en programas de ejemplo de la API de MOXA
 * 
 *
 * National laboratory of advanced informatics (LANIA)
 * - Sensors and embedded systems laboratory
 *
 * API for the development of applications using the ZigBee to RS232
 * protocol adapter.
 * Some code was based on MOXA API examples in the use of serial communication
*/


/* Manual switching for compiler selection */
#define USE_GCC

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
//#include <stropts.h>
#ifndef USE_GCC
	#include <asm/ioctls.h>
	#include <moxadevice.h>
#endif

#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

void *serial_recv(void *arg);

typedef struct command_t{
	int id;
	unsigned char cmd;
	unsigned char *data;
	int len;
}command_t;

unsigned char app_serial_buffer[1024];
int serial_buffer_cont;
int cmd_state;

/* Define the serial port used. 
 * If GCC is not used, this API assumes that this was run
 * from a cross compiler and the ttyM0 device should be used.
 */

#ifndef USE_GCC
#define SERIAL_PORT    "/dev/ttyM0"
#else
#define SERIAL_PORT    "/dev/pts/3"
#endif

/* Open the device defined in SERIAL_PORT  */
int open_device(void);

/* Close port device. 
 * fd : The file descriptor of the open port
 */
int close_device (int fd);


/* From protocol parser */
int send_serial_command(unsigned char *dat, int len);
void analize_recv_data(void);
int recv_data(void);
int send_command(uint8_t cmd, uint8_t *data, int len, uint8_t id);
void print_data_frame(unsigned char *data, int len);
command_t parse_recv_command(unsigned char *buffer, int recv_len);
int _send_bad_command(unsigned char header, unsigned char footer, unsigned char cmd, int len);


unsigned char device_status(void);
int get_children_amount(void);
int get_network_status(void);
int get_children_list(void);
int get_lqi_rssi(void);
int send_data(uint16_t address, uint8_t *data, int len);
int write_data_frame(unsigned char *data, int len);
int enable_data_reception(void);
