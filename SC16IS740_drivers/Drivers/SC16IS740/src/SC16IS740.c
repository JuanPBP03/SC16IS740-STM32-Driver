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
 * @retval 		array of low and high bytes of clock div value
 */
void IS740_setBaudRate(IS740handle_t *hIS740, uint32_t sysclk, uint32_t baudrate){
	uint16_t temp = sysclk/(baudrate*16);
	hIS740->config.clkDiv[0] = (uint8_t)temp;
	hIS740->config.clkDiv[1] = (uint8_t)(temp>>8);

	temp = hIS740->readByte(IS740_LCR_ADDR_REGSEL);
	temp = temp | IS740_LCR_DIVLATCHEN;

	hIS740->writeByte(IS740_LCR_ADDR_REGSEL, (uint8_t)temp);

	// Set low byte
	hIS740->writeByte(IS740_DLL_ADDR_REGSEL, hIS740->config.clkDiv[0]);

	// Set high byte
	hIS740->writeByte(IS740_DLH_ADDR_REGSEL, hIS740->config.clkDiv[1]);

	temp &= ~IS740_LCR_DIVLATCHEN;
	hIS740->writeByte(IS740_LCR_ADDR_REGSEL, (uint8_t)temp);


}

/**
 * @brief Initializes the IS740 UART bridge for basic operation.
 *
 * This function configures the baud rate, enables FIFOs, and sets the
 * line format to 8 data bits, no parity, and 1 stop bit (8N1).
 *
 *
 * @param		Pointer to IC handle struct
 * @retval None
 */
void IS740_init(IS740handle_t *hIS740){

	uint8_t tempreg;

	tempreg = IS740_LCR_DIVLATCHEN | IS740_LCR_WORDLEN;

	//	1. Configure clock divisor

	// Configure line control register
	hIS740->writeByte(IS740_LCR_ADDR_REGSEL, tempreg);

	// Disable divisor latch
	tempreg ^= IS740_LCR_DIVLATCHEN;
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
