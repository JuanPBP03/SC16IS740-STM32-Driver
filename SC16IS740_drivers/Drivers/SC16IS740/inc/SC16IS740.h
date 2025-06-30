/*
 * SC16IS740.h
 *
 *  Created on: Jun 23, 2025
 *      Author: JuanP
 */

#ifndef SC16IS740_SC16IS740_H_
#define SC16IS740_SC16IS740_H_

#include <stdint.h>
#include "SC16IS740_defines.h"


typedef enum{
	IS740_STATE_READY = 0,
	IS740_STATE_BUSYTX,
	IS740_STATE_ERROR,
}IS740state_t;

#define IS740_FLAG_TXE				IS740_LSR_TXE_POS
#define IS740_FLAG_THRE				IS740_LSR_THRE_POS
#define IS740_FLAG_RXNE				IS740_LSR_RXNE_POS
#define IS740_FLAG_FIFOERR			IS740_LSR_FIFOERR_POS
#define IS740_FLAG_OVRERR			IS740_LSR_OVR__POSPOS

typedef enum{
	IS740_ERROR_NONE = 0,
	IS740_ERROR_BREAK,
	IS740_ERROR_FRAMING,
	IS740_ERROR_PARITY,
	IS740_ERROR_COMMS,
}IS740error_t;



/*
 * 	Handle/Context and Config Structs
 */


typedef struct
{
	uint32_t baudRate;
	uint8_t parity;
	uint8_t stopBits;
	uint8_t wordLen;
}IS740cfg_t;


typedef struct
{
	IS740cfg_t config;
	IS740error_t (*writeFunc)(uint8_t regAddr, uint8_t *buffer, uint8_t size);		// function used to write from bridge (I2C/SPI)
	IS740error_t (*readFunc)(uint8_t regAddr, uint8_t *buffer, uint8_t size);		// function used to read from bridge (I2C/SPI)
	IS740state_t state;
}IS740handle_t;





IS740error_t IS740_getFlag(IS740handle_t *hIS740, uint8_t flag);

void IS740_setBaudRate(IS740handle_t *hIS740, uint32_t sysclk, uint32_t baudrate);

void IS740_init(IS740handle_t *hIS740);

void IS740_transmitByte(IS740handle_t *hIS740, uint8_t txByte);

uint8_t IS740_receiveByte(IS740handle_t *hIS740);

void IS740_transmitStream(IS740handle_t *hIS740, uint8_t *buff);
void IS740_receiveStream(IS740handle_t *hIS740, uint8_t *buff);
void IS740_writeByte(IS740handle_t *hIS740, uint8_t regAddr, uint8_t byte);
uint8_t IS740_readByte(IS740handle_t *hIS740, uint8_t regAddr);


#endif /* SC16IS740_SC16IS740_H_ */
