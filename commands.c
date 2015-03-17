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

#include "commands.h"

unsigned char device_status();
int get_network_status();
int get_children_amount();


unsigned char device_status()
{
	int recv;
	if (send_command(DEVICE_STATUS, NULL, 0, 0) == COMMAND_SENT) {
		recv = recv_data();
		printf("Data received : %d\n", recv);
		if (recv == COMMAND_RECEIVED) {
			printf("Command received DEVICE_STATUS\n");
			if (cmd_temp.cmd == DEVICE_UP)
				return 0;
		}
	}
	return 1;
}

// Si la red esta iniciada devuelve la direccion de red
// En caso contrario devuelve 0

int get_network_status()
{
	int nwk_dir;
	if (send_command(GET_NETWORK_STATUS, NULL, 0,0) == COMMAND_SENT) {
		if (recv_data() == COMMAND_RECEIVED) {
			if (cmd_temp.cmd == IN_NETWORK_STATUS) {
				if (cmd_temp.len == 2) { // Longitud esperada para dir de red
					nwk_dir = (int) (*(cmd_temp.data) << 8) & 0xff00;
					nwk_dir |= (int) (*(cmd_temp.data + 1) & 0x00ff);
					free(cmd_temp.data);
					return nwk_dir;
				}
			}
		}
	}
	return 0;
}

int get_children_amount()
{
	if (send_command(GET_CHILDREN_AMOUNT, NULL, 0, 0) == COMMAND_SENT) {
		if (recv_data() == COMMAND_RECEIVED) {
			if (cmd_temp.cmd == GET_CHILDREN_AMOUNT) {
				if (cmd_temp.len == 1) {
					return (int) *cmd_temp.data;
				}
			}
		}
	}
}

int data_from_network()
{
	if (recv_data() == COMMAND_RECEIVED) {
		printf("Command detected\n");
	}
}
