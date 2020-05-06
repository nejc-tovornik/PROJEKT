/*
 * DataPacket.c
 *
 *  Created on: Apr 16, 2020
 *      Author: Nejc
 */

#include "DataPacket.h"

static uint8_t checkAzimuth(uint16_t azimuth) {
	if (azimuth > 360)
		azimuth = 360;
	if (azimuth < 0)
		azimuth = 0;
	return azimuth;
}

static uint16_t checkCoord(uint16_t coord) {
	if (coord > 1023)
		coord = 1023;
	if (coord < 0)
		coord = 0;
	return coord;
}
/*
 * Preveri če so parametri robotov ok
 */
static uint8_t validateRobotParameters(Robot *robot) {
	for (uint8_t i = 0; i < 7; i++) {
		robot[i].X = checkCoord(robot[i].X);
		robot[i].Y = checkCoord(robot[i].Y);
		robot[i].Azimuth = checkAzimuth(robot[i].Azimuth);
		if (robot[i].ID > 7 || robot[i].ID < 0) {
			return 1;
		}
	}

	for (uint8_t i = 0; i < 7; i++) {
		for (uint8_t j = i + 1; j < 7; j++) {
			if (robot[i].ID == robot[j].ID) {
				return 1;
			}
		}
	}

	return 0;
}

/*
 * Funkcija preveri če prejeti paket ustreza
 */
static DataPacket_Status validateDataPacket(uint8_t *dataPacket) {
	for (uint8_t i = 0; i < 7; i++) {
		if ((dataPacket[i * 4] >> 5) > 7) {		//ID check
			return DataPacket_ParameterError;
		} else if (dataPacket[i * 4] == 'E' && dataPacket[i * 4 + 1] == 'R') {
			return DataPacket_Error;
		}
	}
	return DataPacket_OK;
}



/*
 * Funkcija sestavi in shrani podatkovni paket v array dataPacket
 */
DataPacket_Status generateDataPacket(Robot *robot, uint8_t *dataPacket) {

	if (validateRobotParameters(robot)) {
		sprintf((char*) dataPacket, "ERROR in parameters!");
		return DataPacket_Error;
	}

	for (uint8_t i = 0; i < 7; i++) {
		dataPacket[i * 4] = (robot[i].ID << 5)
				| (robot[i].Azimuth >> 4);	// ID in zgornji del azimuta
		dataPacket[i * 4 + 1] = ((checkAzimuth(robot[i].Azimuth) & 0xF) << 4)
				| (robot[i].X >> 6); 	//Spodnji del azimuta in zgornji del X
		dataPacket[i * 4 + 2] = (robot[i].X << 2) | (robot[i].Y >> 8);
		dataPacket[i * 4 + 3] = robot[i].Y & 0xFF;
	}
	return DataPacket_OK;
}

/*
 * Funkcija razstavi paket in shrani podatke v objekt Robot
 */

DataPacket_Status disassembleDataPacket(Robot *robot, uint8_t *dataPacket) {
	DataPacket_Status status = validateDataPacket(dataPacket);

	if (status == DataPacket_Error || status == DataPacket_ParameterError) {
		return status;
	}
	for (uint8_t i = 0; i < 7; i++) {
		if ((dataPacket[i * 4] >> 5) == robot->ID) {
			robot->Azimuth = ((dataPacket[i * 4] & 0x1F) << 4) | (dataPacket[i * 4 + 1] >> 4);
			robot->X = ((dataPacket[i * 4 + 1] & 0xF) << 6) | (dataPacket[i * 4 + 2] >> 2);
			robot->Y = ((dataPacket[i * 4 + 2] & 0x2) << 8) | dataPacket[i * 4 + 3];
		}
	}

	return status;
}
