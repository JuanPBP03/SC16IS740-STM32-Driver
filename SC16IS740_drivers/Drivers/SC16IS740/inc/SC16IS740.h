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
	IS740_STATE_RESET = 0x20010000UL,
	IS740_STATE_READY = 0x20020000UL,
	IS740_STATE_BUSY = 0x20040000UL,
	IS740_STATE_BUSY_TX = 0x20080000UL,
	IS740_STATE_BUSY_RX = 0x20100000UL,
	IS740_STATE_BUSY_TXRX = 0x20110000UL,
	IS740_STATE_ERROR = 0x20120000UL,

}IS740state_t;

#define IS740_FLAG_TXE				IS740_LSR_TXE
#define IS740_FLAG_THRE				IS740_LSR_THRE
#define IS740_FLAG_RXNE				IS740_LSR_RXNE
#define IS740_FLAG_FIFOERR			IS740_LSR_FIFOERR
#define IS740_FLAG_OVRERR			IS740_LSR_OVR

typedef enum{
	IS740_ERROR_NONE = 0xE0010000UL,
	IS740_ERROR_BREAK = 0xE0020000UL,
	IS740_ERROR_FRAMING = 0xE0040000UL,
	IS740_ERROR_PARITY = 0xE0080000UL,
	IS740_ERROR_TIMEOUT = 0xE0100000UL,
	IS740_ERROR_OVERRUN = 0xE0110000UL,
	IS740_ERROR_UNKNOWN = 0xE0120000UL,
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
	IS740error_t errorcode;
	uint8_t fifoen;
}IS740handle_t;


/*
 * 	Helper Macros
 */

#define DISABLE							0U
#define ENABLE							!DISABLE
#define SET								ENABLE
#define RESET							DISABLE


/*
 * Config Macros
 */
#define IS740_PARITY_NONE				(0U<<IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_ODD				(1U<<IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_EVEN				(3U<<IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_FORCE1				(5U<<IS740_LCR_PARITYEN_POS)
#define IS740_PARITY_FORCE0				(7U<<IS740_LCR_PARITYEN_POS)

#define IS740_STOPLEN_1					(0U<<IS740_LCR_STOP_POS)
#define IS740_STOPLEN_1HALF				(1U<<IS740_LCR_STOP_POS)
#define IS740_STOPLEN_2					(1U<<IS740_LCR_STOP_POS)

#define IS740_WORDLEN_5					(0U<<IS740_LCR_WORDLEN_POS)
#define IS740_WORDLEN_6					(1U<<IS740_LCR_WORDLEN_POS)
#define IS740_WORDLEN_7					(2U<<IS740_LCR_WORDLEN_POS)
#define IS740_WORDLEN_8					(3U<<IS740_LCR_WORDLEN_POS)

void IS740_LoopbackControl(IS740handle_t *hIS740, uint8_t ENorDI);

void IS740_FIFOControl(IS740handle_t *hIS740, uint8_t ENorDI);

uint8_t IS740_getStatus(IS740handle_t *hIS740);

void IS740_setBaudRate(IS740handle_t *hIS740, uint32_t sysclk);
void IS740_setClkDiv(IS740handle_t *hIS740, uint16_t divisor);

void IS740_init(IS740handle_t *hIS740);

IS740state_t IS740_transmitByte(IS740handle_t *hIS740, uint8_t txByte);

uint32_t IS740_receiveByte(IS740handle_t *hIS740);

IS740state_t IS740_transmitStream(IS740handle_t *hIS740, uint8_t *buff, uint8_t size);
IS740state_t IS740_receiveStream(IS740handle_t *hIS740, uint8_t *buff, uint8_t size);
IS740error_t IS740_writeByte(IS740handle_t *hIS740, uint8_t regAddr, uint8_t byte);
uint32_t IS740_readByte(IS740handle_t *hIS740, uint8_t regAddr);


#endif /* SC16IS740_SC16IS740_H_ */
