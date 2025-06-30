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
#include <stdio.h>
//static uint8_t IS740_getTXLVL(IS740handle_t *hIS740);
//static uint8_t IS740_getRXLVL(IS740handle_t *hIS740);
static void IS740_flushFIFO(IS740handle_t *hIS740);




/**
 * @brief Flushes the FIFO queues for reset
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @retval
 */
static void IS740_flushFIFO(IS740handle_t *hIS740){
	uint8_t temp;
	temp = IS740_FCR_RXRST | IS740_FCR_TXRST;

	IS740_writeByte(hIS740, IS740_FCR_ADDR_REGSEL, temp);
}

/**
 * @brief Enables or disables modem loopback
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @param		ENorDI ENABLE(1) or DISABLE(0)
 * @retval
 */
void IS740_LoopbackControl(IS740handle_t *hIS740, uint8_t ENorDI){
	uint8_t temp = IS740_readByte(hIS740, IS740_MCR_ADDR_REGSEL);
	if(ENorDI == ENABLE)
		temp |= IS740_MCR_LOOPBACKEN;
	else if(ENorDI == DISABLE)
		temp &= ~IS740_MCR_LOOPBACKEN;
	else
		return;

	IS740_writeByte(hIS740, IS740_MCR_ADDR_REGSEL, temp);
}

/**
 * @brief Enables or disables fifo queue
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @param		ENorDI ENABLE(1) or DISABLE(0)
 * @retval
 */
void IS740_FIFOControl(IS740handle_t *hIS740, uint8_t ENorDI){
	if(ENorDI == ENABLE)
		IS740_writeByte(hIS740, IS740_FCR_ADDR_REGSEL, 0x01);
	else if(ENorDI == DISABLE)
		IS740_writeByte(hIS740, IS740_FCR_ADDR_REGSEL, 0x00);
	else
		return;


}

/**
 * @brief Retrives line status register byte
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @retval		line status register
 */
uint8_t IS740_getStatus(IS740handle_t *hIS740){
	return IS740_readByte(hIS740, IS740_LSR_ADDR_REGSEL);
}


/**
 * @brief Calculates and sets clkDiv values for baud rate generator
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @param		sysclk System clock frequency in Hz
 * @param		desired baudrate
 * @retval
 */
void IS740_setBaudRate(IS740handle_t *hIS740, uint32_t sysclk){

	hIS740->state = IS740_STATE_BUSY;

	uint32_t temp = sysclk/(hIS740->config.baudRate*16);
	uint8_t temp2;

	// Set Div latch enable bit without altering other bits
	temp2 = IS740_readByte(hIS740, IS740_LCR_ADDR_REGSEL);
	temp2 = temp2 | IS740_LCR_DIVLATCHEN;

	IS740_writeByte(hIS740, IS740_LCR_ADDR_REGSEL, temp2);

	// Set div latch low byte
	IS740_writeByte(hIS740, IS740_DLL_ADDR_REGSEL, (uint8_t)(temp & 0xFF));

	// Set div latch high byte
	IS740_writeByte(hIS740, IS740_DLH_ADDR_REGSEL, (uint8_t)(temp >> 8));

	// Disable Divisor Latch
	temp2 &= ~IS740_LCR_DIVLATCHEN;
	IS740_writeByte(hIS740, IS740_LCR_ADDR_REGSEL, temp2);

	hIS740->state = IS740_STATE_RESET;

}
/**
 * @brief Overrides baud rate generator clock divisor in case error is too high when calculated automatically
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @param		divider 16 bit clock divider
 * @retval
 */
void IS740_setClkDiv(IS740handle_t *hIS740, uint16_t divider) {
	hIS740->state = IS740_STATE_BUSY;

	uint8_t temp = IS740_readByte(hIS740, IS740_LCR_ADDR_REGSEL);
	IS740_writeByte(hIS740, IS740_LCR_ADDR_REGSEL, temp | IS740_LCR_DIVLATCHEN);

	IS740_writeByte(hIS740, IS740_DLL_ADDR_REGSEL, (uint8_t)(divider & 0xFF));
	IS740_writeByte(hIS740, IS740_DLH_ADDR_REGSEL, (uint8_t)(divider >> 8));

	IS740_writeByte(hIS740, IS740_LCR_ADDR_REGSEL, temp  & ~IS740_LCR_DIVLATCHEN);

	hIS740->state = IS740_STATE_RESET;
}
/**
 * @brief Resets the IS740 UART bridge to its default values,
 * disables interrupts, flushes and disables FIFOs, lowers baud rate,
 * and configures line to 8N1 format.
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @retval 		None
 */
void IS740_deinit(IS740handle_t *hIS740) {
	hIS740->state = IS740_STATE_BUSY;

	// 1. Disable interrupts
	IS740_writeByte(hIS740, IS740_IER_ADDR_REGSEL, 0x00);

	// 2. Flush FIFOs
	IS740_flushFIFO(hIS740);

	// 3. Disable FIFO
	IS740_FIFOControl(hIS740, DISABLE);

	// 4. Disable loopback and modem features
	IS740_writeByte(hIS740, IS740_MCR_ADDR_REGSEL, 0x00);

	// 5. Drop baud rate to minimum safe value
	IS740_setClkDiv(hIS740, 1); // slows down activity

	// 6. Set line control to known default
	IS740_writeByte(hIS740, IS740_LCR_ADDR_REGSEL, IS740_WORDLEN_8 | IS740_PARITY_NONE | IS740_STOPLEN_1);

	// 7. Reset software state
	hIS740->fifoen = RESET;
	hIS740->errorcode = IS740_ERROR_NONE;
	hIS740->state = IS740_STATE_RESET;
}
/**
 * @brief Initializes the IS740 UART bridge for basic operation.
 *
 * This function configures the baud rate, enables FIFOs, and sets the
 * line word length, parity, and stop bits.
 *
 *
 * @param		hIS740 Pointer to IC handle struct
 * @retval 		None
 */
void IS740_init(IS740handle_t *hIS740){

	hIS740->state = IS740_STATE_BUSY;
	IS740_flushFIFO(hIS740);
	uint8_t tempreg = 0;


	tempreg |= hIS740->config.parity;
	tempreg |= hIS740->config.stopBits;
	tempreg |= hIS740->config.wordLen;

	// Configure line control register
	IS740_writeByte(hIS740, IS740_LCR_ADDR_REGSEL, tempreg);
	IS740_FIFOControl(hIS740, ENABLE);

	hIS740->fifoen = SET;
	hIS740->errorcode = IS740_ERROR_NONE;
	hIS740->state = IS740_STATE_READY;

}


/**
 * @brief  Transmits a single byte over UART using the IS740.
 *
 * Writes the given byte to the Transmit Holding Register (THR).
 *
 * @param		hIS740 Handle of bridge to transmit from
 * @param		txByte Byte to send
 * @retval 		IS740 state
 */
IS740state_t IS740_transmitByte(IS740handle_t *hIS740, uint8_t txByte){
	uint32_t status;
	if(hIS740->state != IS740_STATE_READY)
		return IS740_STATE_BUSY;

	if(!(IS740_getStatus(hIS740) & IS740_FLAG_THRE)){
		return IS740_STATE_BUSY_TX;
	}

	hIS740->state = IS740_STATE_BUSY_TX;
	IS740_FIFOControl(hIS740, ENABLE);
	status = IS740_writeByte(hIS740, IS740_THR_ADDR, txByte);
	if( status != IS740_ERROR_NONE ){
		hIS740->errorcode = status;
		return IS740_STATE_ERROR;
	}

	hIS740->state = IS740_STATE_READY;
	hIS740->errorcode = IS740_ERROR_NONE;
	return IS740_STATE_READY;
}


/**
 * @brief  Receives a single byte over UART using the IS740.
 *
 * Reads byte from the Receive Holding Register (RHR).
 *
 *
 * @param		hIS740 Handle of IS740 to receive from
 * @retval 		byte read from RHR or IS740 state if an exception occurs
 */
uint32_t IS740_receiveByte(IS740handle_t *hIS740){
	uint32_t byte;
	if(hIS740->state != IS740_STATE_READY)
		return IS740_STATE_BUSY;

	if(!(IS740_getStatus(hIS740) & IS740_FLAG_RXNE)){
		hIS740->errorcode = IS740_ERROR_RXEMPTY;
		return IS740_STATE_ERROR;
	}
	hIS740->state = IS740_STATE_BUSY_TX;
	byte = IS740_readByte(hIS740, IS740_RHR_ADDR);

	hIS740->errorcode = IS740_ERROR_NONE;
	hIS740->state = IS740_STATE_READY;
	return byte;
}


/**
 * @brief  Transmits a stream of up to 64 bytes over UART using the IS740.
 *
 * Writes the stream of bytes to the Transmit Holding Register (THR).
 *
 *
 * @param		hIS740 Handle of bridge to transmit from
 * @param		buff Array of bytes to transmit
 * @param		size Size of array
 * @retval 		IS740 state
 */
IS740state_t IS740_transmitStream(IS740handle_t *hIS740, uint8_t *buff, uint8_t size){

	uint32_t status;
	if(hIS740->state != IS740_STATE_READY)
		return IS740_STATE_BUSY;
	if(size>64){
		hIS740->errorcode = IS740_ERROR_OVERRUN;
		hIS740->state = IS740_STATE_ERROR;
		return IS740_STATE_ERROR;
	}
	if(!(IS740_getStatus(hIS740) & IS740_FLAG_TXE)){
		return IS740_STATE_BUSY_TX;
	}
	hIS740->state = IS740_STATE_BUSY_TX;

	status = hIS740->writeFunc(IS740_THR_ADDR, buff, size);

	if(status != IS740_ERROR_NONE){
		hIS740->errorcode = status;
		hIS740->state = IS740_STATE_ERROR;
		return IS740_STATE_ERROR;
	}

	hIS740->errorcode = IS740_ERROR_NONE;
	hIS740->state = IS740_STATE_READY;
	return IS740_STATE_READY;
}

/**
 * @brief  Receives a stream of up to 64 bytes over UART using the IS740.
 *
 * Reads the stream from the Receive Holding Register (RHR).
 *
 *
 * @param		hIS740 Handle of bridge to receive from
 * @param		buff Array of bytes to receive into
 * @param		size Size of array
 * @retval 		IS740 state
 */
IS740state_t IS740_receiveStream(IS740handle_t *hIS740, uint8_t *buff, uint8_t size){

	uint32_t status;
	if(hIS740->state != IS740_STATE_READY)
		return IS740_STATE_BUSY;
	if(size>64){
		hIS740->errorcode = IS740_ERROR_OVERRUN;
		hIS740->state = IS740_STATE_ERROR;
		return IS740_STATE_ERROR;
	}
	if(!(IS740_getStatus(hIS740) & IS740_FLAG_RXNE)){
		return IS740_STATE_READY;
	}
	hIS740->state = IS740_STATE_BUSY_RX;

	status = hIS740->readFunc(IS740_THR_ADDR, buff, size);

	if(status != IS740_ERROR_NONE){
		hIS740->errorcode = status;
		hIS740->state = IS740_STATE_ERROR;
		return IS740_STATE_ERROR;
	}
	hIS740->state = IS740_STATE_READY;
	return IS740_STATE_READY;
}
/**
 * @brief  Writes a single byte to a given register of the IS740.
 *
 *
 *
 *
 * @param		hIS740 Handle of bridge to receive from
 * @param		regAddr Address of IS740 internal register
 * @param		byte Byte too write to internal register
 * @retval 		Error code
 */
IS740error_t IS740_writeByte(IS740handle_t *hIS740, uint8_t regAddr, uint8_t byte){
	IS740error_t status = hIS740->writeFunc(regAddr, &byte, 1);
	if( status != IS740_ERROR_NONE ){
#ifdef DEBUG_IS740
			printf("ERROR: writeByte failed!\n");
#endif
			hIS740->errorcode = status;
			return status;
		}
	return IS740_ERROR_NONE;
}

/**
 * @brief  Reads a single byte from a given register of the IS740.
 *
 *
 *
 *
 * @param		hIS740 Handle of bridge to receive from
 * @param		regAddr Address of IS740 internal register
 * @retval 		data Byte read from RHR (first byte) or error code
 */
uint32_t IS740_readByte(IS740handle_t *hIS740, uint8_t regAddr){
	uint8_t data[1];
	uint32_t status = hIS740->readFunc(regAddr, data, 1);
	if( status != IS740_ERROR_NONE ){
#ifdef DEBUG_IS740
		printf("ERROR: readByte failed!\n");
#endif
		hIS740->errorcode = status;
		return status;
	}
	return data[0];
}


/*
static uint8_t IS740_getTXLVL(IS740handle_t *hIS740){

	return (IS740_readByte(hIS740, IS740_TXLVL_ADDR_REGSEL) & IS740_TXLVL);

}
static uint8_t IS740_getRXLVL(IS740handle_t *hIS740){

	return (IS740_readByte(hIS740, IS740_RXLVL_ADDR_REGSEL) & IS740_RXLVL);

}

*/
#endif /* SC16IS740_SC16IS740_CPP_ */
