# I2C/SPI to UART Bridge V0.8.0

A minimal driver for the NXP SC16IS740 I2C/SPI to UART bridge.

This is an introduction for me to writing custom drivers for external MCU peripherals so it will be focused on educational value for me and for use in my future projects.

## Features

The driver currently supports transmission and reception in bursts of up to 64 bytes, or single bytes and frames can be configured as desired.

- [x] I2C register read/write testing
- [x] Complete UART frame config 
- [x] Byte-wise Tx/Rx
- [x] 64-Byte burst Tx/Rx
- [x] Loopback testing
- [ ] Continuous blocking Tx/Rx
- [ ] Interrupt support
- [ ] Software Flow-control
- [ ] Hardware Flow-control

## Quick Start

**NOTE: IS740 is used as an abbreviation for SC16IS740. In all references to functions within documentation, the "IS740_" prefix will be omitted.**

1. Create read and write functions that use SPI or I2C to communicate with the IC. They must return an IS740error_t error code and take 3 parameters: register address, data buffer, and buffer size.
2. Create a ```IS740handle_t``` instance and assign ```readFunc``` and ```writeFunc``` accordingly.
3. Use the config struct within the handle to configure the parity, stop bits, word length, and desired baud rate.
4. Use ```setBaudRate``` passing in the address of the handle and the IC's XTAL1 frequency. If the calculated baud rate has too much error, the divisor can be overriden using ```setClkDiv```.
5. Call ```init``` with the handle address to configure the frame settings.
6. Profit.

**EXAMPLE USAGE:**
```c
	IS740handle_t bridge;
	bridge.readFunc= I2CReadFromBridge;		// IS740error_t I2CReadFromBridge(uint8_t regaddr, uint8_t *buffer, uint8_t size);
	bridge.writeFunc = I2CWriteToBridge;	// IS740error_t I2CWriteToBridge(uint8_t regaddr, uint8_t *buffer, uint8_t size);

	bridge.config.baudRate = 9600;
	bridge.config.parity = IS740_PARITY_NONE;
	bridge.config.stopBits = IS740_STOPLEN_1;
	bridge.config.wordLen = IS740_WORDLEN_8;
	
	// Get peripheral clock frequency (XTAl1 is supplied by PCLK1)
	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	IS740_setBaudRate(&bridge, pclk1);
	IS740_init(&bridge);

	uint8_t string[] = "Hello World!\n";
	uint8_t rxbuf[64];


	IS740_transmit64(&bridge, string, sizeof(string));
	// ...
	IS740_receive64(&bridge, rxbuf, sizeof(string));
```

