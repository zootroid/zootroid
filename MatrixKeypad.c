/****************************************************************************
 *  Matrix_Keypad.c
 *  4 x 4 matrix keypad scan routines for the 18F8722 on a 
 *   PIC TRAINER REV-1.28.
 * 
 *  Author:  Mike Boldin
 *           <mboldin@niagaracollege.ca>
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
 *  Modification History:
 *  --------------------
 *  2019.02.24  (MB) This is now CTEC1904/2019W Lab 6.2
 *  2020.02.09  (MB) Created as a code library for 2020W
 *
 ****************************************************************************
 *  MPLAB X Version: 	5.30
 *  XC8 Version:        2.10
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
 *  RB2..RB5	=>	MATRIX KEYPAD ROWS 1..4
 *  RJ0..RJ3	<=	MATRIX KEYPAD COLS 1..4
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


/****************************************************************************
 * Constant Defines
 ***************************************************************************/

#if !defined(_XTAL_FREQ)
#define _XTAL_FREQ	4915200		/* 4.9152 MHz oscillator frequency */
#endif


/****************************************************************************
 * Subroutines
 ****************************************************************************/

void		DebounceDelay	(void) ;
int         ReadKeys        (unsigned char) ;


/****************************************************************************
 * Global Variables
 ***************************************************************************/

/* (none) */


/**************************************************************************
 *  Function:	  MatrixKeypadInit     
 *  Description:  I/O initialization for the matrix keypad
 *  Parameters:   none	
 *  Returns:      none
 *  Notes:        Uses RB2..RB5 and RJ0..RJ3
 *                
 **************************************************************************/
void
MatrixKeypadInit(void) 
{
	LATB = 0xff ;           /* All PORT B pins are OFF (active low) */
	TRISB = 0b11000011 ;	/* RB2..RB5 are outputs; RB0 & RB1 are unused;
            				   RB6 & RB7 are dedicated to ICD */
	
	TRISJ = 0xff ;          /* RJ0..RJ3 are inputs; RJ4..RJ7 are unused */
}


/**************************************************************************
 *  Function:	  getch     
 *  Description:  Read a character from standard input -- remapped to the
 *                 matrix keypad
 *  Parameters:   none	
 *  Returns:      ASCII character -- one of:  '0'-'9', 'A', 'B', 'C', 'D',
 *	               'E', or 'F'
 *  Notes:        Incomplete implementation.  No way to indicate EOF.
 *                
 **************************************************************************/

int
getch(void)
{
	unsigned char rowCount ;	/* row count 0..3 */
	unsigned char colCount ;	/* column count 0..3 */
	unsigned char rowBits ;		/* row assertion bits */
	unsigned char colBits ;		/* expected column bits for keypress */
	int rawKey ;			/* key ID (row x 4 + column) */
	int key ;			/* ASCII code for pressed key */
	int keyPressed_b ;		/* Boolean a key was pressed */
	
	key = -1 ;
	keyPressed_b = 0 ;

	while ( ! keyPressed_b )	/* operate continuously */
	{
		rowBits = 0b11111011 ;	/* bit 2 clear initially */
		for ( rowCount = 0 ; rowCount < 4 ; rowCount++ )
		{
			LATB = rowBits ;	/* assert row */
			colBits = 0b11111110 ;	/* bit 0 clear initially */
			for ( colCount = 0 ; colCount < 4 ; colCount++ )
			{
				if ( ReadKeys( colBits ) )
				{
					/* on potential key press,
					   debounce and confirm */
					
					DebounceDelay();
					if ( ReadKeys( colBits ) )
					{
						rawKey = rowCount << 2 ;
						rawKey |= colCount ;
 
						keyPressed_b = 1 ;
						break ;
					}
				}
				
				/* rotate column bits left */
				colBits = ( colBits << 1 ) | 
					  ( colBits >> 7 ) ;
			} /* end foreach column */
			
			if ( keyPressed_b )
			{
				while ( ReadKeys( colBits ) )
				{
					; /* wait for key release */
				}
				
				break ;
			}
			
			/* rotate row bits left */
			rowBits = ( rowBits << 1 ) | ( rowBits >> 7 ) ;
		} /* end foreach row */
	} /* end while */
	
	if ( keyPressed_b )	/* decode key */
	{
		if ( rawKey > 9 ) 
		{
			switch ( rawKey )
			{
				case 10 :	key = 'A' ;	
						break ;
						
				case 11 :	key = 'B' ;
						break ;
				
				case 12 :	key = 'C' ;
						break ;
						
				case 13 :	key = 'D' ;
						break ;
						
				case 14 :	key = 'E' ;
						break ;
						
				case 15 :	key = 'F' ;
						break ;
			}
		}
		else	/* digit */
		{
			key = rawKey + '0' ;
		}
	}
	
	return key;
}

/****************************************************************************
 * Subroutines
 ****************************************************************************/

/**************************************************************************
 *  Function:	  ReadKeys
 *  Description:  Read all keypad column pins and confirm potential keypress
 *  Parameters:   bitMask  The current column bits (one is active low)	
 *  Returns:      1	if the column result matches the column bits,
 *	              0	otherwise
 *  Notes:        Reads PORTJ
 *                
 **************************************************************************/
int
ReadKeys( unsigned char bitMask )
{
	unsigned char inBits ;
	
	inBits = PORTJ | 0xf0 ;
	
	return ( inBits == bitMask ) ;
}


/**************************************************************************
 *  Function:	  DebounceDelay     
 *  Description:  20ms delay to debounce keys/switches
 *  Parameters:   none	
 *  Returns:      none
 *  Notes:        Dependent on _XTAL_FREQ (must be defined for _this_ 
 *		   board)
 *                
 **************************************************************************/
void 
DebounceDelay(void)
{
	__delay_ms(20);
}

/* End of MatrixKeypad.c */
