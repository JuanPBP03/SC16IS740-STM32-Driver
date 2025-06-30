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


/*
 * 	Handle/Context and Config Structs
 */


typedef struct
{
	uint8_t clkDiv[2];
	uint8_t parity;
	uint8_t stopBits;
	uint8_t wordLen;
}IS740cfg_t;


typedef struct
{
	uint8_t addr;
	IS740cfg_t config;
	void (*writeByte)(uint8_t regAddr, uint8_t regVal);
	uint8_t (*readByte)(uint8_t regAddr);
	uint8_t state;
}IS740handle_t;

void IS740_setBaudRate(IS740handle_t *hIS740, uint32_t sysclk, uint32_t baudrate);

void IS740_init(IS740handle_t *hIS740);

void IS740_transmitByte(IS740handle_t *hIS740, uint8_t txByte);

uint8_t IS740_receiveByte(IS740handle_t *hIS740);

#endif /* SC16IS740_SC16IS740_H_ */
