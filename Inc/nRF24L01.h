/*
 * nRF24L01.h
 *
 *  Created on: Mar 27, 2020
 *      Author: Nejc
 */

#ifndef NRF_H_
#define NRF_H_

#include "stm32f7xx.h"
#include "nRF24L01_RegMap.h"

// Retransmit delay
enum {
	nRF24_ARD_NONE   = (uint8_t)0x00, // Dummy value for case when retransmission is not used
	nRF24_ARD_250us  = (uint8_t)0x00,
	nRF24_ARD_500us  = (uint8_t)0x01,
	nRF24_ARD_750us  = (uint8_t)0x02,
	nRF24_ARD_1000us = (uint8_t)0x03,
	nRF24_ARD_1250us = (uint8_t)0x04,
	nRF24_ARD_1500us = (uint8_t)0x05,
	nRF24_ARD_1750us = (uint8_t)0x06,
	nRF24_ARD_2000us = (uint8_t)0x07,
	nRF24_ARD_2250us = (uint8_t)0x08,
	nRF24_ARD_2500us = (uint8_t)0x09,
	nRF24_ARD_2750us = (uint8_t)0x0A,
	nRF24_ARD_3000us = (uint8_t)0x0B,
	nRF24_ARD_3250us = (uint8_t)0x0C,
	nRF24_ARD_3500us = (uint8_t)0x0D,
	nRF24_ARD_3750us = (uint8_t)0x0E,
	nRF24_ARD_4000us = (uint8_t)0x0F
};

// Data rate
enum {
	nRF24_DR_250kbps = (uint8_t)0x20, // 250kbps data rate
	nRF24_DR_1Mbps   = (uint8_t)0x00, // 1Mbps data rate
	nRF24_DR_2Mbps   = (uint8_t)0x08  // 2Mbps data rate
};

// RF output power in TX mode
enum {
	nRF24_TXPWR_18dBm = (uint8_t)0x00, // -18dBm
	nRF24_TXPWR_12dBm = (uint8_t)0x02, // -12dBm
	nRF24_TXPWR_6dBm  = (uint8_t)0x04, //  -6dBm
	nRF24_TXPWR_0dBm  = (uint8_t)0x06  //   0dBm
};

// CRC encoding scheme
enum {
	nRF24_CRC_off   = (uint8_t)0x00, // CRC disabled
	nRF24_CRC_1byte = (uint8_t)0x08, // 1-byte CRC
	nRF24_CRC_2byte = (uint8_t)0x0c  // 2-byte CRC
};

// nRF24L01 power control
enum {
	nRF24_PWR_UP   = (uint8_t)0x02, // Power up
	nRF24_PWR_DOWN = (uint8_t)0x00  // Power down
};

// Transceiver mode
enum {
	nRF24_MODE_RX = (uint8_t)0x01, // PRX
	nRF24_MODE_TX = (uint8_t)0x00  // PTX
};

enum {
	nRF24_DPL_ON = (uint8_t)0x01, // PRX
	nRF24_DPL_OFF = (uint8_t)0x00  // PTX
} ;

// Enumeration of RX pipe addresses and TX address
enum {
	nRF24_PIPE0  = (uint8_t)0x00, // pipe0
	nRF24_PIPE1  = (uint8_t)0x01, // pipe1
	nRF24_PIPE2  = (uint8_t)0x02, // pipe2
	nRF24_PIPE3  = (uint8_t)0x03, // pipe3
	nRF24_PIPE4  = (uint8_t)0x04, // pipe4
	nRF24_PIPE5  = (uint8_t)0x05, // pipe5
	nRF24_PIPETX = (uint8_t)0x06  // TX address (not a pipe in fact)
};

// State of auto acknowledgment for specified pipe
enum {
	nRF24_AA_OFF = (uint8_t)0x00,
	nRF24_AA_ON  = (uint8_t)0x01
};

// Status of the RX FIFO
enum {
	nRF24_STATUS_RXFIFO_DATA  = (uint8_t)0x00, // The RX FIFO contains data and available locations
	nRF24_STATUS_RXFIFO_EMPTY = (uint8_t)0x01, // The RX FIFO is empty
	nRF24_STATUS_RXFIFO_FULL  = (uint8_t)0x02, // The RX FIFO is full
	nRF24_STATUS_RXFIFO_ERROR = (uint8_t)0x03  // Impossible state: RX FIFO cannot be empty and full at the same time
};

// Status of the TX FIFO
enum {
	nRF24_STATUS_TXFIFO_DATA  = (uint8_t)0x00, // The TX FIFO contains data and available locations
	nRF24_STATUS_TXFIFO_EMPTY = (uint8_t)0x01, // The TX FIFO is empty
	nRF24_STATUS_TXFIFO_FULL  = (uint8_t)0x02, // The TX FIFO is full
	nRF24_STATUS_TXFIFO_ERROR = (uint8_t)0x03  // Impossible state: TX FIFO cannot be empty and full at the same time
};

// Result of RX FIFO reading
typedef enum {
	nRF24_RX_PIPE0  = (uint8_t)0x00, // Packet received from the PIPE#0
	nRF24_RX_PIPE1  = (uint8_t)0x01, // Packet received from the PIPE#1
	nRF24_RX_PIPE2  = (uint8_t)0x02, // Packet received from the PIPE#2
	nRF24_RX_PIPE3  = (uint8_t)0x03, // Packet received from the PIPE#3
	nRF24_RX_PIPE4  = (uint8_t)0x04, // Packet received from the PIPE#4
	nRF24_RX_PIPE5  = (uint8_t)0x05, // Packet received from the PIPE#5
	nRF24_RX_EMPTY  = (uint8_t)0xff  // The RX FIFO is empty
} nRF24_RXResult;


// Addresses of the RX_PW_P# registers
static const uint8_t nRF24_RX_PW_PIPE[6] = {
		nRF24_REG_RX_PW_P0,
		nRF24_REG_RX_PW_P1,
		nRF24_REG_RX_PW_P2,
		nRF24_REG_RX_PW_P3,
		nRF24_REG_RX_PW_P4,
		nRF24_REG_RX_PW_P5
};

// Addresses of the address registers
static const uint8_t nRF24_ADDR_REGS[7] = {
		nRF24_REG_RX_ADDR_P0,
		nRF24_REG_RX_ADDR_P1,
		nRF24_REG_RX_ADDR_P2,
		nRF24_REG_RX_ADDR_P3,
		nRF24_REG_RX_ADDR_P4,
		nRF24_REG_RX_ADDR_P5,
		nRF24_REG_TX_ADDR
};

typedef struct {
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *CSN_GPIO_PORT;
	uint16_t CSN_PIN;
}NRF;

// Function prototypes
void nRF24_Init(NRF *dev);
uint8_t nRF24_Check(NRF *dev);

void nRF24_SetPowerMode(NRF *dev, uint8_t mode);
void nRF24_SetOperationalMode(NRF *dev, uint8_t mode);
void nRF24_SetRFChannel(NRF *dev, uint8_t channel);
void nRF24_SetAutoRetr(NRF *dev, uint8_t ard, uint8_t arc);
void nRF24_SetAddrWidth(NRF *dev, uint8_t addr_width);
void nRF24_SetAddr(NRF *dev, uint8_t pipe, const uint8_t *addr);
void nRF24_SetTXPower(NRF *dev, uint8_t tx_pwr);
void nRF24_SetDataRate(NRF *dev, uint8_t data_rate);
void nRF24_SetCRCScheme(NRF *dev, uint8_t scheme);
void nRF24_SetRXPipe(NRF *dev, uint8_t pipe, uint8_t aa_state, uint8_t payload_len);
void nRF24_ClosePipe(NRF *dev, uint8_t pipe);
void nRF24_EnableAA(NRF *dev, uint8_t pipe);
void nRF24_DisableAA(NRF *dev, uint8_t pipe);
void nRF24_SetDynamicPayloadLength(NRF *dev, uint8_t mode);
void nRF24_SetPayloadWithAck(NRF *dev, uint8_t mode);

uint8_t nRF24_GetStatus(NRF *dev);
uint8_t nRF24_GetIRQFlags(NRF *dev);
uint8_t nRF24_GetStatus_RXFIFO(NRF *dev);
uint8_t nRF24_GetStatus_TXFIFO(NRF *dev);
uint8_t nRF24_GetRXSource(NRF *dev);
uint8_t nRF24_GetRetransmitCounters(NRF *dev);
uint8_t nRF24_GetFeatures(NRF *dev);

void nRF24_ResetPLOS(NRF *dev);
void nRF24_FlushTX(NRF *dev);
void nRF24_FlushRX(NRF *dev);
void nRF24_ClearIRQFlags(NRF *dev);
void nRF24_ActivateFeatures(NRF *dev);
void nRF24_WritePayload(NRF *dev, uint8_t *pBuf, uint8_t length);
void nRF24_WriteAckPayload(NRF *dev, nRF24_RXResult pipe, char *payload, uint8_t length);
nRF24_RXResult nRF24_ReadPayload(NRF *dev, uint8_t *pBuf, uint8_t *length);
nRF24_RXResult nRF24_ReadPayloadDpl(NRF *dev, uint8_t *pBuf, uint8_t *length);

#endif /* NRF_H_ */
