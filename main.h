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


#define USE_GCC
//#undef USE_GCC

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <stropts.h>
#ifndef USE_GCC
	#include <asm/ioctls.h>
	#include <moxadevice.h>
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
