// System Control Block (SCB) modules consist of 
// Memory Accelerator Module, 
// Phase Locked Loop Module (PLL), 
// VLSI Peripheral Bus Divider (VPB),
// Power Control,
// External Interrupt,
// Reset,
// Code Debugging
// SCB's memory-mapped I/O base address is 0xE01FC000

// Initialize Memory Accelerator Module
(*(volatile unsigned long *)(0xE01FC000 + 0x000)) = 0x02;  // MAM Control Reg
(*(volatile unsigned long *)(0xE01FC000 + 0x004)) = 0x04;  // MAM Timing Reg

// Initialize VLSI Peripheral Bus Divider
(*(volatile unsigned long *)(0xE01FC000 + 0x100)) = 0x01;  // Divider is 1, namely peripheral's clock == cpu's clock

// Initialize Phrase Clocked Loop Module
(*(volatile unsigned long *)(0xE01FC000 + 0x080)) = 0x03;  // PLL Control Reg
(*(volatile unsigned long *)(0xE01FC000 + 0x084)) = 0x23;  // PLL Config Reg
(*(volatile unsigned long *)(0xE01FC000 + 0x08C)) = 0xAA;  // PLL Feed Reg (a sequence here 0xAA 0x55)
(*(volatile unsigned long *)(0xE01FC000 + 0x08C)) = 0x55;

// Every processor has to initialize its interrupt mechanism
// Initialize Vector Interrupt Controller (VIC)
// VIC's memeory-mapped I/O base address is 0xFFFFF000
(*(volatile unsigned long *)(0xFFFFF000 + 0x014)) = 0xffffffff;  // Interrupt Enable Clear Reg, when wirte 1 to 1 bit, it clears this correspoding bit in IntEnable Reg. // Disable these 32 interrupts
(*(volatile unsigned long *)(0xFFFFF000 + 0x030)) = 0x00000000;  // Interrupt Vector List Base Address
(*(volatile unsigned long *)(0xFFFFF000 + 0x00C)) = 0x00000000;  // Decide each interrupt is a FIQ or IRQ, 1 is FIQ, 0 is IRQ