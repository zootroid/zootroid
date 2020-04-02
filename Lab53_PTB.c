/****************************************************************************
 *  Lab53_PTB.c
 *  4 x 4 matrix keypad scan program with LCD display for the 18F8722 on a 
 *   PIC TRAINER REV-1.28.
 * 
 *  Author:  Christopher Calvert
 *           <ccalvert1@ncstudents.niagaracollege.ca>
 *
 *  Scan the entire matrix keypad and display the key value on the 
 *   LCD dot matrix display (DMD).
 * 
 *  Set the SW3-1 DIP switch (KEYSC-F) to RB5.
 * 
 *  Important Note!
 *  ==============
 *  Both the pushbuttons and the SW1 DIP switches share PORTJ.  Make sure
 *   that ALL SW1 DIP switches are in the OFF position (1) BEFORE you run
 *   _any_ program that uses the matrix keypad.
 *
 ****************************************************************************
 *  Originally written by Professor Mark Csele 2006/09
 *  as LCD Test program for the 18F452 on a PICPROTO-II (up to 20MHz)
 ****************************************************************************
 *  Modification History for LCDTest.c:
 *  ----------------------------------
 *  2011.02.14  Merged with template for CTEC1904/2011W
 *               to update CONFIG directives, etc.
 *  2011.02.15  Modified to work with Lab #3 counter application.
 *              Converted to 8-bit mode.
 *  2013.01.07  Successfully tested on PIC TRAINER REV-1.28
 *  2013.01.16  (MB) Created template from Buttons.asm (Pushbuttons_CtoF_PTB)
 *  2013.01.27  (MB) Changed oscillator setting from XT to HS; added tables
 *  2014.01.15  (MB) Updated for 2014W
 *  2014.03.12  (MB) Added possible hardware configs and libraries
 *  2019.01.14  (MB) Translated to C and ported to MPLAB X
 *  2019.01.19  (MB) Ported LCDTest18.asm and LCD.asm to C
 *  2019.01.19  (MB) Use printf() with LCD definition of putch()
 ****************************************************************************
 *  Modification History:
 *  --------------------
 *  2013.01.16  (MB) Created template from Buttons.asm (Pushbuttons_CtoF_PTB)
 *  2013.01.27  (MB) Changed oscillator setting from XT to HS; added tables
 *  2014.01.15  (MB) Updated for 2014W
 *  2014.03.12  (MB) Added possible hardware configs and libraries
 *  2019.01.14  (MB) Translated to C and ported to MPLAB X
 *  2019.01.18  (MB) Ported from KeypadCtoF.asm (Lab 2, Program 13, 2018W)
 *  2019.01.19  (MB) Combined Lab43_PTB.c, Keypad.asm (Lab3, Program 15, 
 *		      2018W) and Lab34_PTB.c (for 7-seg. codes)
 *  2019.01.21  (MB) Implement the standard C getch() function.
 *  2019.02.12  (MB) Merged with LCDTest.c to combine getch() with putch()
 *  2019.02.24  (MB) This is now CTEC1904/2019W Lab 6.2
 *  2020.02.09  (MB) Re-written as CTEC1904/2020W Lab 5.3
 * 
 *  2020.03.31  (CC) Started Lab 5.3
 *  2020.04.01  (CC) Completed Lab 5.3 with comments
 *
 ****************************************************************************
 *  MPLAB X Version: 	5.30
 *  XC8 Version:        2.10
 *  Debugger:           PICkit3
 *
 *        1         2         3         4         5         6         7
 *23456789012345678901234567890123456789012345678901234567890123456789012345678
 *
 ****************************************************************************
 *  Hardware:
 *  --------
 *  PIC18F8722 MCU
 *  CLK = 4.9152 MHz (f_osc)
 *
 *  RH0..RH7	=>	LCD DATA 0..7 (handled by LCD_Init in LCD.c)
 *  RE0         =>	LCD RS
 *  RE1         =>	LCD E
 *  RE2         =>	LCD R/nW
 *
 *  RB2..RB5	=>	MATRIX KEYPAD ROWS 1..4 (handled by MatrixKeypadInit in
 *  RJ0..RJ3	<=	MATRIX KEYPAD COLS 1..4  MatrixKeypad.c)
 *
 *  Board Configuration DIP Switch Settings
 *  =======================================
 *  SW2-1..SW2-4	x
 *  SW3-1 (KEYSC-F)	RB5
 *  SW3-2..SW3-4	x
 *
 *  IMPORTANT NOTE!!!
 *  --------------
 *  PICTRAINER BOARD uses SHARED I/O -- PORT J is shared between SW1
 *   DIP switches, which are wired ACTIVE LOW with 10K pull-up resistors.
 * 
 *  -> Attempting to assert rows active HIGH with DIP switch(es) closed
 *      will short PORT B pins to ground, while reading constant LOW on
 *      PORT J.
 *
 ****************************************************************************/

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "MatrixKeypad.h"


/*** Programming Configuration Information ***/

				/* PIC18F8722 is the target processor */
#pragma config OSC = HS     	/* High-speed oscillator (4.9152MHz) */
#pragma config FCMEN = OFF      /* Fail-Safe Clock Monitor disabled */
#pragma config IESO = OFF       /* Two-Speed Start-up disabled */
#pragma config PWRT = OFF       /* PWRT disabled */
#pragma config BOREN = ON  	/* Brown-out Reset enabled and controlled by
				    software (SBOREN is enabled) */
#pragma config BORV = 2		/* Brown-out at 2V */
#pragma config WDT = OFF    	/* WDT disabled (control is placed on the
				    SWDTEN bit) */
#pragma config STVREN = ON  	/* Stack full/underflow will cause Reset */
#pragma config LVP = OFF	/* Single-Supply ICSP disabled */
#pragma config DEBUG = ON       /* Background debugger enabled, RB6 and RB7
				    are dedicated to In-Circuit Debug */

/*** Programming Configuration ENDS ***/

/****************************************************************************
 * Constant Defines
 ***************************************************************************/

#define _XTAL_FREQ	4915200		/* 4.9152 MHz oscillator frequency */

#define LCD_CHARS_PER_LINE	16


/****************************************************************************
 * Subroutines
 ****************************************************************************/

/* (none) */

/****************************************************************************
 * Global Variables
 ***************************************************************************/

/* (none) */


/****************************************************************************
 * Main Program Begins
 ***************************************************************************/

int main (void)
{
	
    /*LOCAL_VARIABLES_HERE*/

	ADCON1 = 0x0f ;		/* All analog I/O pins are digital */

	/* CONFIGURE_I/O_PORTS_HERE */
   
    MatrixKeypadInit();                 // Initialize Matrix Keypad
    LCD_Init();                         // Initialize LCD
	
	/* MAIN_PROGRAM_GOES_HERE */

	while (1)	/* operate continuously */
	{
        
        /*for (int i = 0; i < 16; i++)    // Loop used to keep the first row of
                                        // key strokes on the first line.
        {
            LCD_Char(getch());          // Sends key stroke to LCD
        }
        
        LCD_Line2();                    // Go to line 2, space 1
        
        for (int x = 0; x < 17; x++)    // Loop used to keep the second row of
                                        // key strokes on the second line.
        {
            LCD_Char(getch());          // Sends key stroke to LCD
        }
        
        LCD_Clear();                    // Clears LCD
        LCD_Home();                     // Sends input back to Line 1, space 1*/
        
        LCD_Char(getch());
        DebounceDelay();
        
        
	} /* end while */
		
	return 0 ;
}

/*** END ***/
