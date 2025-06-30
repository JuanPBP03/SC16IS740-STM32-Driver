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
 * @brief Calculates and sets clkDiv values for baud rate generator
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @param		sysclk System clock frequency in Hz
 * @param		desired baudrate
 * @retval
 */
void IS740_setBaudRate(IS740handle_t *hIS740, uint32_t sysclk, uint32_t baudrate){

	uint32_t temp = sysclk/(baudrate*16);
	uint8_t temp2;

	// Set Div latch enable bit without altering other bits
	temp2 = hIS740->readByte(IS740_LCR_ADDR_REGSEL);
	temp2 = temp2 | IS740_LCR_DIVLATCHEN;

	hIS740->writeByte(IS740_LCR_ADDR_REGSEL, temp2);

	// Set div latch low byte
	hIS740->writeByte(IS740_DLL_ADDR_REGSEL, (uint8_t)temp);

	// Set div latch high byte
	hIS740->writeByte(IS740_DLH_ADDR_REGSEL, (uint8_t)(temp>>8));

	// Disable Divisor Latch
	temp2 &= ~IS740_LCR_DIVLATCHEN;
	hIS740->writeByte(IS740_LCR_ADDR_REGSEL, temp2);


}

/**
 * @brief Initializes the IS740 UART bridge for basic operation.
 *
 * This function configures the baud rate, enables FIFOs, and sets the
 * line word length, parity, and stop bits.
 *
 *
 * @param		Pointer to IC handle struct
 * @retval None
 */
void IS740_init(IS740handle_t *hIS740){

	uint8_t tempreg;

	tempreg = IS740_LCR_DIVLATCHEN | IS740_LCR_WORDLEN;



	// Configure line control register
	hIS740->writeByte(IS740_LCR_ADDR_REGSEL, tempreg);




}


/**
 * @brief  Transmits a single byte over UART using the IS740.
 *
 * Writes the given byte to the Transmit Holding Register (THR).
 *
 *
 * @param		Byte to send
 * @retval None
 */
void IS740_transmitByte(IS740handle_t *hIS740, uint8_t txByte){
	hIS740->writeByte(IS740_THR_ADDR, txByte);
}


/**
 * @brief  Receives a single byte over UART using the IS740.
 *
 * Reads byte from the Receive Holding Register (RHR).
 *
 *
 *
 * @retval 		byte read from RHR
 */
uint8_t IS740_receiveByte(IS740handle_t *hIS740){

	return 0;
}


#endif /* SC16IS740_SC16IS740_CPP_ */
