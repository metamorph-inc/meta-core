// Universal Asynchronous Receiver Transmitter 0
// UART0's memory-mapped I/O base address is 0xE000C000
(*(volatile unsigned long *)(0xE000C000 + 0x0C)) = 0x83;  // UART0 Line Control Reg, 
U0DLL = divisor & 0xFF;
U0DLM = (divisor >> 8) & 0xFF;
U0LCR &= ~0x80;
U0FCR = 1;