/*
 * DataPacket.c
 *
 *  Created on: Apr 16, 2020
 *      Author: Nejc
 */

#include "DataPacket.h"

/*
 * Preveri če so parametri robotov ok
 */
static uint8_t validateRobotParameters(Robot *robot) {
	for (uint8_t i = 0; i < 10; i++) {
		if (robot[i].ID > 10 || robot[i].ID < 0) {
			return 1;
		}
	}

	for (uint8_t i = 0; i < 10; i++) {
		for (uint8_t j = i + 1; j < 10; j++) {
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
	for (uint8_t i = 0; i < 10; i++) {
		if ((dataPacket[i * 3] >> 4) > 10) {		//ID check
			return DataPacket_ParameterError;
		} else if ((dataPacket[i * 3] & 0xF) > 15) {	//Azimuth check
			return DataPacket_ParameterError;
		} else if (dataPacket[i * 3] == 'E' && dataPacket[i * 3 + 1] == 'R') {
			return DataPacket_Error;
		}
	}
	return DataPacket_OK;
}

/*
 * Funkcija pripravi rotacijo za podatkovni paket
 */
static uint8_t encodeAzimuth(uint16_t azimuth) {
	if (azimuth > 360)
		azimuth = 360;
	return azimuth / 24;
}

/*
 * Funkcija pripravi rotacijo za podatkovni paket
 */
static uint16_t decodeAzimuth(uint8_t azimuth) {
	return azimuth * 24;
}

/*
 * Funkcija sestavi in shrani podatkovni paket v array dataPacket
 */
DataPacket_Status generateDataPacket(Robot *robot, uint8_t *dataPacket) {

	if (validateRobotParameters(robot)) {
		sprintf((char*) dataPacket, "ERROR in parameters!");
		return DataPacket_Error;
	}

	for (uint8_t i = 0; i < 10; i++) {
		dataPacket[i * 3] = (robot[i].ID << 4)
				| encodeAzimuth(robot[i].Azimuth);
		dataPacket[i * 3 + 1] = robot[i].X;
		dataPacket[i * 3 + 2] = robot[i].Y;
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
	for (uint8_t i = 0; i < 10; i++) {
		if ((dataPacket[i * 3] >> 4) == robot->ID) {
			robot->Azimuth = decodeAzimuth(dataPacket[i * 3] & 0xF);
			robot->X = dataPacket[i * 3 + 1];
			robot->Y = dataPacket[i * 3 + 2];
		}
	}

	return status;
}
