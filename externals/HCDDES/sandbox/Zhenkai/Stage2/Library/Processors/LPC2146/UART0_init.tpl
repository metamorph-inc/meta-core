// Universal Asynchronous Receiver Transmitter 0
// UART0's memory-mapped I/O base address is 0xE000C000
divisor0 = peripheralClockFrequency / (16 * {{BAUDRATE}});
(*(volatile unsigned long *)(0xE000C000 + 0x0C)) = 0x83;  // UART0 Line Control Reg, in which Divisor Latch Access Bit is enabled
(*(volatile unsigned long *)(0xE000C000 + 0x00)) = divisor & 0xFF;  // UART0 Divisor Latch Least reg
(*(volatile unsigned long *)(0xE000C000 + 0x04)) = (divisor >> 8) & 0xFF;  // UART0 Divisor Latch Most reg
(*(volatile unsigned long *)(0xE000C000 + 0x0C)) &= ~0x80;  // UART0 Line Control Reg, disable Divisor Latch Access Bit
(*(volatile unsigned long *)(0xE000C000 + 0x08)) = 1;  // UART0 FIFO Control Reg, enable FIFOs for RX and TX