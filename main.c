#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "main.h"

int main (int argc, char* argv[]) 
{
	
	pthread_t thread_read_serial;
	
	printf("Iniciando aplicacion...\n");
	int i = serial_open();
	
	char temp;
	unsigned char *ptr_tmp;
	ptr_tmp = (unsigned char *) malloc(2*sizeof(unsigned char));
	*ptr_tmp = 0xEE;
	*(ptr_tmp + 1) = 0xBB;
	if (device_status() == 0)
		printf("Dispositivo presente\n");
	printf ("Direccion de red : %x\n", get_network_status());
	printf ("Nodos conectados : %d\n", get_children_amount());
	
	
	//i = recv_data();
	
	//printf("El valor de i : %d\n", i);
	
	//pthread_create(&thread_read_serial, NULL, serial_recv, NULL);
	//serial_write_byte('A');
	//printf("Se escribio el byte\n");
	
	while (1);
	return 0;
}
