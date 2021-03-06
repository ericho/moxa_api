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


/*
 * Sends command to retrieve the device status.
 *
 * Returns: Zero when the DEVICE_UP message has been
 * received from device. Otherwise will return a
 * negative value.
 */
unsigned char device_status(void)
{
	int recv;
	if (send_command(DEVICE_STATUS, NULL, 0, 0) == COMMAND_SENT) {
		recv = recv_data();
		DEBUG_PRINT("Data received : %d\n", recv);
		if (recv == COMMAND_RECEIVED) {
			DEBUG_PRINT("Command received DEVICE_STATUS\n");
			if (cmd_temp.cmd == DEVICE_UP)
				return 0;
			else
				return -ENODEV;
		} else {
			return -EINVAL;
		}
	} else {
		return -ECOMM;
	}
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

int get_children_list(void)
{
	return 0;
}

int get_lqi_rssi(void)
{
	return 0;
}

int send_data(uint16_t address, uint8_t *data, int len)
{
	return 0;
}

int enable_data_reception(void)
{
	return 0;
}


int data_from_network()
{
	if (recv_data() == COMMAND_RECEIVED) {
		DEBUG_PRINT("Command detected\n");
	}
}
