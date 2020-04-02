/* 
 * File:        LCD.h
 * Description: Library of LCD routines for the 18F8722 on a PIC TRAINER 
 *               REV-1.28
 * Author:      Michael Boldin
 *              <mboldin@niagaracollege.ca>
 *
 * Created on January 19, 2019, 2:32 PM
 */

#ifndef LCD_H
#define	LCD_H

/*
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
    
/***************************************************************************
 * Constant Defines
 ***************************************************************************/

/* Port E register and bit definitions */

#define LCD_CtrlPort		LATE	/* LCD control port */
#define LCD_CtrlPort_Cfg	TRISE	 
#define LCD_RS              LATEbits.LE0    /* RE0 = LCD register select */
#define LCD_E               LATEbits.LE1	/* RE1 = LCD enable */
#define LCD_RW              LATEbits.LE2    /* RE2 = LCD read/nWrite */

/* Port H register and bit definitions */

#define LCD_DataPort		LATH    /* RH0-RH7 = LCD data port */
#define LCD_DataPort_Cfg	TRISH

#ifdef	__cplusplus
extern "C" {
#endif

void LCD_Init(void);
void LCD_Char(unsigned char);
void LCD_Cmd(unsigned char);
void LCD_Clear(void);
void LCD_Home(void);	
void LCD_Line1(void);
void LCD_Line2(void);	
void LCD_StringOut(const char *);

void putch(char);    /* Standard C library function, mapped to LCD_Char */
   
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

