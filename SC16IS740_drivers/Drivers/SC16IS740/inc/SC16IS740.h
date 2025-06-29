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
 * 	Helper macros
 */


#define IS740_PARITY_NONE				(0U<<SC16IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_ODD				(1U<<SC16IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_EVEN				(3U<<SC16IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_FORCE1				(5U<<SC16IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_FORCE0				(7U<<SC16IS740_LCR_PARITYEN_POS)

#define IS740_STOPLEN_1					(0U<<SC16IS740_LCR_STOP_POS)
#define IS740_STOPLEN_1HALF				(1U<<SC16IS740_LCR_STOP_POS)
#define IS740_STOPLEN_2					(1U<<SC16IS740_LCR_STOP_POS)

#define IS740_WORDLEN_5					(0U<<SC16IS740_LCR_WORDLEN_POS)
#define IS740_WORDLEN_6					(1U<<SC16IS740_LCR_WORDLEN_POS)
#define IS740_WORDLEN_7					(2U<<SC16IS740_LCR_WORDLEN_POS)
#define IS740_WORDLEN_8					(3U<<SC16IS740_LCR_WORDLEN_POS)




/*
 * 	Handle/Context and Config Structs
 */


typedef struct
{
	uint8_t clkDiv[2];
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

uint8_t* IS740_setClkDiv(SC16IS740handle_t *hIS740, uint32_t sysclk, uint32_t baudrate);

void IS740_init(SC16IS740handle_t *hIS740);

void IS740_transmitByte(SC16IS740handle_t *hIS740, uint8_t txByte);

uint8_t IS740_receiveByte(SC16IS740handle_t *hIS740);

#endif /* SC16IS740_SC16IS740_H_ */
