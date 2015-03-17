
/*
   To use this code without a card you need first to create
   a virtual serial port with socat program.

   This can be done with:

   socat -d -d pty,raw,echo=0 pty,raw,echo=0

   This will create two interfaces in /dev/pts/N
   where N is the number of the interface.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "main.h"

void tests() {
	int i;
	// Testing with bad length
	printf("Testing with bad length\n"); 
	i = _send_bad_command(0x55, 0xaa, 0x01, 0xffff);
	// Testing with bad header
	printf("Testing with bad header\n"); 
	i = _send_bad_command(0x33, 0xaa, 0x01, 0x0006);
	// Testing with bad footer
	printf("Testing with bad footer\n"); 
	i = _send_bad_command(0x55, 0xee, 0x01, 0x0006);
}

int main (int argc, char* argv[])
{

	pthread_t thread_read_serial;

	printf("Starting application...\n");
	int i = serial_open();
	printf("Serial port open %d\n", i);
	printf("=> Send device_status() : ");
	i = device_status();
	printf("%d\n", i);
	return 0;
	i = serial_write_byte('a');
	char temp;
	unsigned char *ptr_tmp;
	ptr_tmp = (unsigned char *) malloc(2*sizeof(unsigned char));
	*ptr_tmp = 0xEE;
	*(ptr_tmp + 1) = 0xBB;

	//data_from_network();
	char c[20];
	//serial_read_file(c, 1);

	/*
	if (device_status() == 0)
		printf("Dispositivo presente\n");
	printf ("Direccion de red : %x\n", get_network_status());
	printf ("Nodos conectados : %d\n", get_children_amount());
	*/

	while(!(i = serial_read_byte(c, 1)));


	//i = recv_data();

	printf("El valor de i : %d c %x\n", i, c[0]);

	pthread_create(&thread_read_serial, NULL, serial_recv, NULL);
	//serial_write_byte('A');
	//printf("Se escribio el byte\n");

	while (1)
		sleep(1);
	return 0;
}
