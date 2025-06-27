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
	uint8_t
}SC16IS740cfg_t;

typedef struct
{
	SC16IS740cfg_t config;
	void (*regWrite)(uint8_t regAddr, uint8_t regVal);
	uint8_t (*regRead)(uint8_t regAddr);
	uint8_t state;
}SC16IS740handle_t;

#endif /* SC16IS740_SC16IS740_H_ */
