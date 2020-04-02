#ifndef XC_PTB_CONFIG_H
#define	XC_PTB_CONFIG_H

/* 
 * File:        config_PTB.h
 * Author:      Mike Boldin, Niagara College Canada
 *              <mboldin@niagaracollege.ca>
 * Comments:    Configuration directives for the Niagara College 
 *              "PIC18F TRAINER_1" board (PTB) with PIC18F8722 MCU.
 * Revision history: 
 * ----------------
 * 2019.02.04   (MB)    Created from template (CTEC1904 2019 Winter)
 * 
 */

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

#endif	/* XC_PTB_CONFIG_H */
