/*********************************************************************************/
//                              16C554 Header file
/*********************************************************************************/
#ifndef _16C554_H_
#define _16C554_H_
#include "global.h"
#define UARTA_BASE_ADDR 0x7000
#define UARTB_BASE_ADDR 0xB000
#define UARTC_BASE_ADDR 0xD000
#define UARTD_BASE_ADDR 0xE000

// Registers of 16C554
// Registers for UART A
#define UC_554A_THR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x000)) // Transmit Holding Register (write only)
#define UC_554A_RBR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x000)) // Receive Buffer Register (read only)
#define UC_554A_IER  (*(volatile u8 *)(UARTA_BASE_ADDR+0x100)) // Interrupt Enable Register (write only)
#define UC_554A_IIR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x200)) // Interrupt Identification Register (read only)
#define UC_554A_FCR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x200)) // FIFO control register (write only)
#define UC_554A_LCR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x300)) // Line control Register (write only)
#define UC_554A_MCR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x400)) // Modem Control Register (write only)
#define UC_554A_LSR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x500)) // Line Status Register  (read only)
#define UC_554A_MSR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x600)) // Modem Status Register  (read only)
#define UC_554A_SPR  (*(volatile u8 *)(UARTA_BASE_ADDR+0x700)) // Scratchpad Register

#define UC_554A_DLL  (*(volatile u8 *)(UARTA_BASE_ADDR+0x000))
#define UC_554A_DLM  (*(volatile u8 *)(UARTA_BASE_ADDR+0x100))

// Registers for UART B
#define UC_554B_THR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x000)) // Transmit Holding Register (write only)
#define UC_554B_RBR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x000)) // Receive Buffer Register (read only)
#define UC_554B_IER  (*(volatile u8 *)(UARTB_BASE_ADDR+0x100)) // Interrupt Enable Register (write only)
#define UC_554B_IIR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x200)) // Interrupt Identification Register (read only)
#define UC_554B_FCR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x200)) // FIFO control register (write only)
#define UC_554B_LCR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x300)) // Line control Register (write only)
#define UC_554B_MCR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x400)) // Modem Control Register (write only)
#define UC_554B_LSR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x500)) // Line Status Register  (read only)
#define UC_554B_MSR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x600)) // Modem Status Register  (read only)
#define UC_554B_SPR  (*(volatile u8 *)(UARTB_BASE_ADDR+0x700)) // Scratchpad Register

#define UC_554B_DLL  (*(volatile u8 *)(UARTB_BASE_ADDR+0x000))
#define UC_554B_DLM  (*(volatile u8 *)(UARTB_BASE_ADDR+0x100))

// Registers for UART C
#define UC_554C_THR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x000)) // Transmit Holding Register (write only)
#define UC_554C_RBR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x000)) // Receive Buffer Register (read only)
#define UC_554C_IER  (*(volatile u8 *)(UARTC_BASE_ADDR+0x100)) // Interrupt Enable Register (write only)
#define UC_554C_IIR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x200)) // Interrupt Identification Register (read only)
#define UC_554C_FCR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x200)) // FIFO control register (write only)
#define UC_554C_LCR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x300)) // Line control Register (write only)
#define UC_554C_MCR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x400)) // Modem Control Register (write only)
#define UC_554C_LSR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x500)) // Line Status Register  (read only)
#define UC_554C_MSR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x600)) // Modem Status Register  (read only)
#define UC_554C_SPR  (*(volatile u8 *)(UARTC_BASE_ADDR+0x700)) // Scratchpad Register

#define UC_554C_DLL  (*(volatile u8 *)(UARTC_BASE_ADDR+0x000))
#define UC_554C_DLM  (*(volatile u8 *)(UARTC_BASE_ADDR+0x100))

// Registers for UART D
#define UC_554D_THR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x000)) // Transmit Holding Register (write only)
#define UC_554D_RBR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x000)) // Receive Buffer Register (read only)
#define UC_554D_IER  (*(volatile u8 *)(UARTD_BASE_ADDR+0x100)) // Interrupt Enable Register (write only)
#define UC_554D_IIR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x200)) // Interrupt Identification Register (read only)
#define UC_554D_FCR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x200)) // FIFO control register (write only)
#define UC_554D_LCR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x300)) // Line control Register (write only)
#define UC_554D_MCR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x400)) // Modem Control Register (write only)
#define UC_554D_LSR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x500)) // Line Status Register  (read only)
#define UC_554D_MSR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x600)) // Modem Status Register  (read only)
#define UC_554D_SPR  (*(volatile u8 *)(UARTD_BASE_ADDR+0x700)) // Scratchpad Register

#define UC_554D_DLL  (*(volatile u8 *)(UARTD_BASE_ADDR+0x000))
#define UC_554D_DLM  (*(volatile u8 *)(UARTD_BASE_ADDR+0x100))

extern void Reset_554(void);
extern void Init_554(void);
void prints(u8*, u8, char);
                                
extern u8 RFlagA;
extern u8 RFlagB;
extern u8 RFlagC;
extern u8 RFlagD;
extern u8 RFlagPC;
#endif