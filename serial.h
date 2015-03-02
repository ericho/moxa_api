/*
 *  Laboratorio Nacional de informatica Avanzada LANIA A.C.
 * - Laboratorio de sensores y sistemas embebidos
 * 
 * API para el desarrollo de aplicaciones utilizando la tarjeta
 * adaptadora de protocolo ZigBee a RS232
 * 
 * Codigo basado en programas de ejemplo de la API de MOXA
 * 
*/

#include "main.h"

#define SERIAL_OK                   0
#define SERIAL_ERROR_FD             -1
#define SERIAL_ERROR_OPEN           -2
#define SERIAL_PARAMETER_ERROR      -3

#define APP_BUFFER_UPDATED          0
#define MAX_APP_BUFFER_SIZE_REACHED 1
#define MAX_SERIAL_BUFFER_SIZE      1024

#ifndef USE_GCC
#define SERIAL_PORT     "/dev/ttyM0"
#else
#define SERIAL_PORT     "/dev/pts/2"
#endif
#define BAUDRATE        B38400

int serial_open();
int serial_write_byte(unsigned char byte);
int serial_close ();
int serial_write(char *str, int len);

int serial_read_byte (char *buf, int len);
void init_variables(void);
int add_byte_to_buffer(unsigned char data);
void flush_serial_buffer();

void *serial_recv(void *arg);


