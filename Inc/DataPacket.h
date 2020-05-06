/*
 * DataPacket.h
 *
 *  Created on: Apr 16, 2020
 *      Author: Nejc
 */

#ifndef DATAPACKET_H_
#define DATAPACKET_H_

#include "stm32f767xx.h"
#include "stdio.h"

typedef struct {
	uint8_t ID;
	uint16_t Azimuth;
	uint16_t X;
	uint16_t Y;
}Robot;

typedef enum {
	DataPacket_Error,
	DataPacket_ParameterError,
	DataPacket_OK
}DataPacket_Status;

DataPacket_Status generateDataPacket(Robot *robot, uint8_t *dataPacket);
DataPacket_Status disassembleDataPacket(Robot *robot, uint8_t *dataPacket);

#endif /* DATAPACKET_H_ */
