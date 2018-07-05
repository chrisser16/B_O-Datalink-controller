/*
 * Beomaster6000_bluetooth.c
 *
 * Created: 18-06-2018 13:25:20
 * Author : Christian Lind Vie Madsen
 * Job    : Student
 * From   : The University of Aarhus
 */ 
#include <avr/io.h>
#define F_CPU 16000000
#include "util/delay.h"
#include <avr/io.h>


int main(void)
{
	/*
	Definer variabler
	*/
	
	char RX_CODE; // Til at gemme den modtaget kode. 
	int Relaystate = 0; // Gemmer relæ position.
	long BaudRate = 324; // Baudrate for UART
	DDRB =  0b00111111; // Sætter PB 0-5 til outputs.
	
	
	/*
	Definer Beolink hexkode på fjernbetjening. Derefter Baudrate.
	*/
	char REC = 0xA5; // Play.
	char Return = 0x99; // Skift indput.
	char Left = 0x85; // Forrige sang.
	char Right = 0xB9; // Næste sang.
	char Stop = 0xA9; // Stop sang.
	//long Baudrate = 5000; // Baudrate på B&O
	
	
	/*
	Definer hvad hver ben gør på MCU. Bemærk alle numre gælder for PORTB medmindre det ændres i "if" sætningerne og i "Definer variabler"!
	*/
	char Play = 0; // PB0 er play.
	char Phono_ONOFF = 4; // PB1 er til relæ/PHONO.
	char Previous = 2; // PB2 er til Forrige track.
	char Next = 3; // PB3 er til næste track.
	char Stop_blu = 1; // PB4 til stop.
	
/*
UART SETUP
*/
			
	UCSR0A = 0b00100000;
	
	UCSR0B = 0b00011000;
	
	UCSR0C = (8-5)<<1;
	
	UBRR0 = F_CPU/(16*BaudRate) - 1;
	
	
	/*
	Start program
	*/
	while(1){
		
	UCSR0B = (1 << RXEN0);
	
	 while ( (UCSR0A & (1<<7)) == 0 )
	 {}
		 
	RX_CODE = UDR0;
	
	_delay_ms(50);
	
	UCSR0B = (0 << RXEN0);
	
	
	
		if (RX_CODE == REC) // Check om den modtaget HEX kode er magen til REC.
		{
		
			PORTB = (1 << Play);
			_delay_ms(50);          // Hvis ja, toggle Play pin.
			PORTB = (0 << Play);
			RX_CODE = '0';
		}
		
		if((RX_CODE == Return) & (Relaystate == 0)){ // Check om den modtaget HEX kode er magen til Return kode og status på relæ.
			
			PORTB = (1 << Phono_ONOFF); // Hvis ja, tænd relæ på pin nummer Phono_ONOFF. 
			Relaystate = 1;
			RX_CODE = '0';
		}
		else if ( (RX_CODE == Return) & (Relaystate == 1)){ // Check om den modtaget HEX kode er magen til Return kode og status på relæ.
		PORTB = (0 << Phono_ONOFF); // Hvis ja, sluk relæ på pin nummer Phono_ONOFF. 
		Relaystate = 0;
		RX_CODE = '0';
		}

		if (RX_CODE == Left) // Check om den modtaget HEX kode er magen til koden left.
		{
			PORTB = (1 << Previous);
			_delay_ms(50);            // Hvis ja, toggle Previous pin.
			PORTB = (0 << Previous);
			RX_CODE = '0';
		}

		if (RX_CODE == Right) // Check om den modtaget HEX kode er magen til koden Right.
		{
			PORTB = (1 << Next);
			_delay_ms(50);          // Hvis ja, toggle Next pin.
			PORTB = (0 << Next);
			RX_CODE = '0';
		}

		if (RX_CODE == Stop) // Check om den modtaget HEX kode er magen til koden Stop.
		{
			PORTB = (1 << Stop_blu);
			_delay_ms(50);        // Hvis ja, toggle Stop_blu pin.
			PORTB = (0 << Stop_blu);
			RX_CODE = '0';
		}
	
		
	
}

}
