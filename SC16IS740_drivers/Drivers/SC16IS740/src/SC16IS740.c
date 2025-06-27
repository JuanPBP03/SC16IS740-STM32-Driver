/*
 * SC16IS740.cpp
 *
 *  Created on: Jun 23, 2025
 *      Author: JuanP
 */

#ifndef SC16IS740_SC16IS740_CPP_
#define SC16IS740_SC16IS740_CPP_

#include <stdint.h>
#include "SC16IS740.h"

static void IS740_registercallbacks(SC16IS740handle_t *hIS740);
void (*IS740_writeByte)(uint8_t regAddr, uint8_t regVal);
uint8_t (*IS740_readByte)(uint8_t regAddr);


/**
 * @brief Initializes the SC16IS740 UART bridge for basic operation.
 *
 * This function configures the baud rate, enables FIFOs, and sets the
 * line format to 8 data bits, no parity, and 1 stop bit (8N1).
 *
 *
 * @param		Pointer to IC handle struct
 * @retval None
 */
void IS740_init(SC16IS740handle_t *hIS740){

	IS740_registercallbacks(hIS740);
	uint8_t tempreg;

	tempreg = SC16IS740_LCR_DIVLATCHEN | SC16IS740_LCR_WORDLEN;

	//	1. Configure clock divisor

	// Configure line control register
	hIS740->writeByte(SC16IS740_LCR_ADDR, tempreg);

	// Set low byte
	hIS740->writeByte(SC16IS740_DLL_ADDR, hIS740->config.divLow);

	// Set high byte
	hIS740->writeByte(SC16IS740_DLH_ADDR, hIS740->config.divHigh);

	// Disable divisor latch
	tempreg ^= SC16IS740_LCR_DIVLATCHEN;
	hIS740->writeByte(SC16IS740_LCR_ADDR, tempreg);

}


/**
 * @brief  Transmits a single byte over UART using the SC16IS740.
 *
 * Writes the given byte to the Transmit Holding Register (THR).
 *
 *
 * @param		Byte to send
 * @retval None
 */
void IS740_transmitByte(uint8_t txByte){

}


/**
 * @brief  Transmits a single byte over UART using the SC16IS740.
 *
 * Reads byte from the Receive Holding Register (RHR).
 *
 *
 *
 * @retval 		byte read from RHR
 */
uint8_t IS740_receiveByte(void){

	return 0;
}


/**
 * @brief  Transmits a single byte over UART using the SC16IS740.
 *
 * Writes the given byte to the given register.
 *
 * @param  		register address to write to
 * @param  		register value to write
 *
 * @retval None
 */
inline void IS740_writeReg(uint8_t regAddr, uint8_t regVal){
	IS740_writeByte(regAddr, regVal);
}

/**
 * @brief  Transmits a single byte over UART using the SC16IS740.
 *
 * Reads from the given regsiter.
 *
* @param  		register address to read
* @param
 * @retval None
 */
inline uint8_t IS740_readReg(uint8_t regAddr){

	return IS740_readByte(regAddr);
}

static void IS740_registercallbacks(SC16IS740handle_t *hIS740){
	IS740_writeByte = hIS740->writeByte;
	IS740_readByte = hIS740->readByte;
}


#endif /* SC16IS740_SC16IS740_CPP_ */
