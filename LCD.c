/* 
 * File:        LCD.c
 * Description: Library of LCD routines for the 18F8722 on a PIC TRAINER 
 *               REV-1.28
 * Author:      Michael Boldin
 *              <mboldin@niagaracollege.ca>
 *
 * Created on January 19, 2019, 2:32 PM
 * 
 ****************************************************************************
 *  Originally written by Professor Mark Csele 2006/09
 *  as LCD Test program for the 18F452 on a PICPROTO-II (up to 20MHz)
 ****************************************************************************
 *  Modification History:
 *  --------------------
 *  2013.01.16  (MB) Created template from Buttons.asm (Pushbuttons_CtoF_PTB)
 *  2013.01.27  (MB) Changed oscillator setting from XT to HS; added tables
 *  2013.03.04  (MB) Copied from LCDTest18F.asm and modified;
 *                    converted to re-entrant code (save BSR, WREG, and STATUS)
 *  2019.01.19  (MB) Ported to C
 *  2019.01.19  (MB) Added putch()
 *
 ***************************************************************************
 * 
 * MPLAB X Version: 	5.10
 * XC8 Version: 	2.00
 * Editor:  		tab size = 8, keep tabs,
 *			line end ruler position = 78
 *
 *        1         2         3         4         5         6         7
123456789012345678901234567890123456789012345678901234567890123456789012345678
 *
 ****************************************************************************
 *  Hardware:
 *  --------
 *  PIC18F8722 MCU
 *  CLK = 4.9152 MHz (f_osc)
 *  RH0-7 - LCD Data Lines (to D0-D7)
 *  RE2   - LCD R/W Line
 *  RE1   - LCD Enable Line
 *  RE0   - LCD RS (register select) Line
 * 
 ****************************************************************************
 *  Public Subroutines
 *  ==================
 *  LCD_Init    Initialize the LCD I/O lines and display for normal, 
 *               8-bit operation. 
 *
 *  LCD_Char    Send a character to the LCD
 *
 *  LCD_Cmd     Send a command to the LCD
 *
 *  LCD_Clear, LCD_Home	
 *              Clear Screen and/or Move to Home (Line 1, Column 1)
 *
 *  LCD_Line1, LCD_Line2	
 *              Position cursor at start of line 1 or line 2 on the LCD
 *
 *  LCD_StringOut	
 *              Output a string to the LCD
 *
 ***************************************************************************/

#include <xc.h>
#include "LCD.h"

void putch(char);	/* Local implementation to enable printf and other
			 *  functions from stdio.h to output directly to
			 *  LCD
			 */

#if !defined(LCD_H)
/***************************************************************************
 * Constant Defines
 ***************************************************************************/

/* Port E register and bit definitions */

#define LCD_CtrlPort		LATE		/* LCD control port */
#define LCD_CtrlPort_Cfg	TRISE	 
#define LCD_RS			LATEbits.LE0    /* RE0 = LCD register select */
#define LCD_E			LATEbits.LE1	/* RE1 = LCD enable */
#define LCD_RW			LATEbits.LE2    /* RE2 = LCD read/nWrite */

/* Port H register and bit definitions */

#define LCD_DataPort		LATH    /* RH0-RH7 = LCD data port */
#define LCD_DataPort_Cfg	TRISH

#endif /* !defined(LCD_H) */

#if !defined(_XTAL_FREQ)
#define _XTAL_FREQ	4915200		/* 4.9152 MHz oscillator frequency */
#endif

/* Forward Declarations */

void LCD_Delay_40us(void) ; 
void LCD_Delay_5ms(void) ; 
void LCD_Delay_16ms(void) ; 


static inline void 
pulseE(void)		/* Pulse the LCD Enable line for 40us */
{
	LCD_E = 1 ; 
	LCD_Delay_40us() ; 
	LCD_E = 0 ;
}


/***************************************************************************
 *  Initialize LCD
 ***************************************************************************
 *  Initialize the LCD I/O lines and display for normal, 8-bit operation. 
 ***************************************************************************/

void 
LCD_Init(void)
{
	LCD_DataPort = 0 ;	/* All PORT H pins are low (OFF) */
	LCD_DataPort_Cfg = 0 ;	/* RH0..RH7 are outputs */

	LCD_CtrlPort = 0 ;	/* All PORT E pins are low (OFF) */
	LCD_CtrlPort_Cfg = 0 ;	/* RE0..RE7 are outputs */

	LCD_Delay_16ms( ) ;	/* Wait more than 15ms after power on */
	
	LCD_RW = 0 ;		/* Set LCD R/nW line to write */

	for ( int i = 0 ; i < 3 ; i++ )
	{
		LCD_Cmd( 0x30 ) ;	/* Function set - 8-bit interface */
		LCD_Delay_5ms( ) ;	/* Wait more than 4.1ms */
	}
	
	LCD_Cmd( 0x38 ) ;	/* 8-bit data; 2 lines; 5x7 characters */
	LCD_Cmd( 0x0C ) ;	/* Display ON, Cursor OFF, Blink OFF */
	LCD_Cmd( 0x06 ) ;	/* Entry Mode: AutoIncrement */
	
	LCD_Clear( ) ;
	LCD_Home( ) ; 
}


/*****************************************************************************
 * LCD_Cmd, LCD_Char
 *****************************************************************************
 * Send a command or character to the LCD
 *****************************************************************************/

void 
LCD_Char(unsigned char ch)
{
	LCD_DataPort = ch ;	/* Send character to LCD */
	LCD_RS = 1 ;		/* Set the LCD Register Select for a char. */
	pulseE( ) ;
	LCD_Delay_40us( ) ;
}

void 
LCD_Cmd(unsigned char cmd)
{
	LCD_DataPort = cmd ;	/* Send command to LCD */
	LCD_RS = 0 ;		/* Set the LCD Register Select for a cmd. */
	pulseE( ) ;
	LCD_Delay_40us( ) ;
}


/***************************************************************************
 *  LCD_Clear, LCD_Home
 ***************************************************************************
 *  Clear Screen or Move to Home (Line 1, Column 1)
 ***************************************************************************/

void 
LCD_Clear(void)
{
	LCD_Cmd( 0x01 ) ;	/* Clear Display */
	LCD_Delay_16ms( ) ;	/* Execution takes 15.2 ms */
}

void 
LCD_Home(void)
{
	LCD_Cmd( 0x02 ) ;	/* Home Display */
	LCD_Delay_16ms( ) ;	/* Execution takes 15.2 ms */
}


/***************************************************************************
 *  LCD_Line1, LCD_Line2
 ***************************************************************************
 *  Position cursor at start of line 1 or line 2 on the LCD display
 ***************************************************************************/

void 
LCD_Line1(void)
{
	LCD_Cmd( 0x40 ) ;	/* Position cursor on line 1 */
}

void 
LCD_Line2(void)
{
	LCD_Cmd( 0xC0 ) ;	/* Position cursor on line 2 */
}


/***************************************************************************
 *  LCD_StringOut
 ***************************************************************************
 *  Output a preloaded, zero-terminated string to the LCD
 **************************************************************************/

void 
LCD_StringOut(const char * s)
{
	register const char * p ;
	
	if ( s == NULL )
	{
		return ;
	}
	
	for ( p = s ; *p ; p++ )
	{
		putch( *p ) ;
	}
}


void 
LCD_Delay_40us(void)
{
	__delay_us(40);
}

void 
LCD_Delay_5ms(void)
{
	__delay_ms(5);
}

void 
LCD_Delay_16ms(void)
{
	__delay_ms(16);
}



/*** FROM XC8 COMPILER sources:
 *   Directory:
 *   C:\Program Files (x86)\Microchip\xc8\v2.00\pic\sources\c99\common
 * 
 *   File:
 *   putch.c
 * 
 */

/*
 * Function: putch
 * Weak implementation.  User implementation may be required
 */

void 
putch(char c)
{
	LCD_Char( c ) ;
}


/*** END LCD.c */
