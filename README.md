# I2C/SPI to UART Bridge V0.1

A minimal driver for the NXP SC16IS740 I2C/SPI to UART bridge.

This is an introduction for me to writing custom drivers for external MCU peripherals so it will be focused on educational value for me and for use in my future projects.

## Planned Features

For the first version of the driver the goal is simply to get a working interface in order to write registers and configure the IC manually. Subsequent versions will include interfaces for various modes of communication. 

- [x] I2C register read/write
- [x] Basic UART init (baud + 8N1)
- [x] Byte-wise Tx/Rx
- [ ] Continuous Tx/Rx
- [ ] Loopback testing
- [ ] Interrupt support
- [ ] Software Flow-control
- [ ] Hardware Flow-control





## Implementation Steps

**NOTE: IS740 is used as an abbreviation for SC16IS740. In all references to functions within documentation, the "IS740_" prefix will be omitted.**

- Create read and write functions that use SPI or I2C to communicate with the IC. They must have no return value and take 3 parameters: register address, data buffer, and buffer size.
- Create a ```IS740handle_t``` instance and assign ```readFunc``` and ```writeFunc``` accordingly.
- Configure the desired baud rate within the internal ```config``` struct of the handle object.
- Use ```setBaudRate``` passing in the address of the handle and the IC's XTAL1 frequency.
- Call ```init``` with the handle address to configure the 8N1 settings.

Currently the driver only supports single byte communication with ```transmitByte``` and ```receiveByte```.

