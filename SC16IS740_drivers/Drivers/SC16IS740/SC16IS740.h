/*
 * SC16IS740.h
 *
 *  Created on: Jun 23, 2025
 *      Author: JuanP
 */

#ifndef SC16IS740_SC16IS740_H_
#define SC16IS740_SC16IS740_H_


/*
 * 	Handle/Context and Config Structs
 */



typedef struct
{
	uint16_t baudRate;
	uint8_t parity;
	uint8_t stopBits;
	uint8_t wordLen;
}SC16IS740cfg_t;


typedef struct
{
	uint8_t addr;
	SC16IS740cfg_t config;
	void (*writeByte)(uint8_t regAddr, uint8_t regVal);
	uint8_t (*readByte)(uint8_t regAddr);
	uint8_t state;
}SC16IS740handle_t;


void IS740_init(SC16IS740handle_t *hIS740);

void IS740_transmitByte(uint8_t addr, uint8_t txByte);

uint8_t IS740_receiveByte(uint8_t addr);

void IS740_writeReg(uint8_t regAddr, uint8_t regVal);
uint8_t IS740_readReg(uint8_t regAddr, uint8_t regVal);

#endif /* SC16IS740_SC16IS740_H_ */
