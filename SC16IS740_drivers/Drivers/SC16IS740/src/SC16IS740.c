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

	uint8_t tempreg;

	tempreg = SC16IS740_LCR_DIVLATCHEN | SC16IS740_LCR_WORDLEN;

	//	1. Configure clock divisor

	// Configure line control register
	hIS740->writeByte(SC16IS740_LCR_ADDR_REGSEL, tempreg);
	hIS740->readByte(SC16IS740_LCR_ADDR_REGSEL);
	// Set low byte
	hIS740->writeByte(SC16IS740_DLL_ADDR_REGSEL, hIS740->config.divLow);
	hIS740->readByte(SC16IS740_DLL_ADDR_REGSEL);
	// Set high byte
	hIS740->writeByte(SC16IS740_DLH_ADDR_REGSEL, hIS740->config.divHigh);
	hIS740->readByte(SC16IS740_DLH_ADDR_REGSEL);

	// Disable divisor latch
	tempreg ^= SC16IS740_LCR_DIVLATCHEN;
	hIS740->writeByte(SC16IS740_LCR_ADDR_REGSEL, tempreg);
	hIS740->readByte(SC16IS740_LCR_ADDR_REGSEL);

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
void IS740_transmitByte(SC16IS740handle_t *hIS740, uint8_t txByte){

}


/**
 * @brief  Receives a single byte over UART using the SC16IS740.
 *
 * Reads byte from the Receive Holding Register (RHR).
 *
 *
 *
 * @retval 		byte read from RHR
 */
uint8_t IS740_receiveByte(SC16IS740handle_t *hIS740){

	return 0;
}


#endif /* SC16IS740_SC16IS740_CPP_ */
