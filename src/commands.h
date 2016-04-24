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

#include "serial_dongle.h"

extern command_t cmd_temp;

#define DEVICE_STATUS		0x01
#define GET_NETWORK_STATUS 	0x02
#define GET_CHILDREN_AMOUNT 	0x03
#define GET_CHILDREN_LIST       0x04
#define GET_LQI_RSSI            0x05
#define SEND_DATA_NODE          0x06
#define DATA_FROM_NODE          0x07
#define SET_RECV_MODE           0x08
#define CONFIGURE_PIN           0x09

#define BAD_PARAMETERS		0x56
#define DEVICE_UP		0x57
#define IN_NETWORK_STATUS       0x58
#define OUT_NETWORK_STATUS      0x59
#define ENABLE_DATA_RECEPTION   0x5a
#define DISABLE_DATA_RECEPTION  0x5b
#define DATA_RECEPTION_CHANGED  0x5c
#define SET_REQUEST_MODE        0x5d
#define SET_SLEEP_MODE          0x5e
#define SET_RF_MODE             0x5f
#define NODE_COMMAND_SENT       0x60

#define COMMAND_SENT		0
#define COMMAND_RECEIVED        0
#define INCORRECT_FRAME_SIZE    1
#define INCORRECT_HEADER        2
#define INCORRECT_FOOTER        3
#define UNKNOWN_COMMAND         4
#define TIMEOUT_ERROR           5
