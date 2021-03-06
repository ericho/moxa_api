
/*
  Laboratorio nacional de informatica avanzada LANIA A.C.
 * - Laboratorio de sensores y sistemas embebidos
 * 
 * API para el desarrollo de aplicaciones utilizando la tarjeta
 * adaptadora de protocolo ZigBee a RS232
 * 
 * Codigo basado en programas de ejemplo de la API de MOXA
 * 
 * National laboratory of advanced informatics (LANIA)
 * Sensors and embedded systems laboratory
 *
 * API to develop applications using the RS232 to ZigBee adapter
 * Some code is based on MOXA API example programs 
*/

#include "serial.h"

int fd;
struct termios tio;
off_t offset;

int serial_write_byte(unsigned char byte);
int serial_write(char *str, int len);
int serial_read_byte (char *buf, int len);
void init_variables(void);
int add_byte_to_buffer(unsigned char data);
void flush_serial_buffer(void);
void *serial_recv(void *arg);

int open_device()
{
	printf("Opening port %s\n", SERIAL_PORT);
	fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
	if (fd < 0)
		return SERIAL_ERROR_OPEN;
	cfsetospeed(&tio, BAUDRATE);
	cfsetispeed(&tio, BAUDRATE);
    
	tio.c_cflag = (tio.c_cflag & ~CSIZE) | CS8;
	tio.c_cflag |= CLOCAL | CREAD;
	tio.c_cflag &= ~(PARENB | PARODD);
	tio.c_cflag &= ~CRTSCTS;
	tio.c_cflag &= ~CSTOPB;
	
	tio.c_iflag &= IGNBRK;
	tio.c_iflag &= ~( IXON | IXOFF | IXANY | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
	tio.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	tio.c_oflag &= ~OPOST;
	
	tio.c_cc[VTIME] = 30;
	tio.c_cc[VMIN] = 0;
	
	tcsetattr(fd, TCSANOW, &tio);
	tcflush(fd, TCIFLUSH);
#ifndef USE_GCC
	ioctl(fd, MOXA_SET_OP_MODE, RS232_MODE);
#endif

	init_variables();
	return fd;
}

int close(int fd)
{
	return close(fd);
}

void init_variables(void)
{
	serial_buffer_cont = 0;
	cmd_state = 0; 
	memset(app_serial_buffer, 0, MAX_SERIAL_BUFFER_SIZE);
}

int serial_write_byte(unsigned char byte)
{
	if (fd < 0)
		return SERIAL_ERROR_OPEN;
	return write(fd, &byte, 1);
}

int serial_read_file(char *buf, int len)
{
	if (fd < 0)
		return SERIAL_ERROR_OPEN;
	return read(fd, buf, 1);
}

int serial_read_byte (char *buf, int len)
{
	fcntl(fd, F_SETFL, FNDELAY);
	if (fd < 0)
		return SERIAL_ERROR_OPEN;
	fcntl(fd, F_SETFL, 0);
	return read(fd, buf, len);
}


void *serial_recv (void *arg)
{
	char buf[1024];
	unsigned char recv;
	int len;
	while (1) {
		len = serial_read_byte(&recv, 1);
		if (len > 0) {
			if (add_byte_to_buffer(recv) == APP_BUFFER_UPDATED)
				analize_recv_data();
			else
				flush_serial_buffer();
		}
	}
}

int add_byte_to_buffer (unsigned char data)
{
	if (serial_buffer_cont < MAX_SERIAL_BUFFER_SIZE) {
		app_serial_buffer[serial_buffer_cont] = data;
		serial_buffer_cont++;
		return APP_BUFFER_UPDATED;
	}
	else {
		return MAX_APP_BUFFER_SIZE_REACHED;
	}
}

void flush_serial_buffer ()
{
	serial_buffer_cont = 0;
	memset(app_serial_buffer, 0, MAX_SERIAL_BUFFER_SIZE);
}
