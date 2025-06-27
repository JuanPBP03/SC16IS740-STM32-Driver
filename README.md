# I2C/SPI to UART Bridge

A minimal driver for the NXP SC16IS740 I2C/SPI to UART bridge.

This is an introduction for me to writing custom drivers for external MCU peripherals so it will be focused on educational value for me and for use in my future projects.

## Planned Features

For the first version of the driver the goal is simply to get a working interface in order to write registers and configure the IC manually.

- [ ] I2C register read/write
- [ ] Basic UART init (baud + 8N1)
- [ ] Byte-wise Tx/Rx
- [ ] Loopback testing
- [ ] Interrupt support
