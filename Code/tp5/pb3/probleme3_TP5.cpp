 #define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>
#include "memoire_24.h"
#include <stdio.h>

void initialisationUART () 
{
	UBRR0H = 0;
	UBRR0L = 0xCF;
	// 2400 bauds. Nous vous donnons la valeur des deux
	// premier registres pour vous éviter des complications

	UCSR0A = (1 << RXC0 ) | (1 << TXC0 );
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// permettre la reception et la transmission par le UART0

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	// Format des trames: 8 bits, 1 stop bits, none parity
		// p187
}



void transmissionUART ( uint8_t donnee ) 
    // De l'USART vers le PC
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	{
		// loop vide pour attendre que le buffer se vide
	}
	/* Put data  buffer, sends the data */
	UDR0 = donnee;
	// p 174
}

int main()
{
	initialisationUART();
	Memoire24CXXX memoireExterne;	
	
	unsigned char ff = 0xFF;
	memoireExterne.ecriture(0x10, &ff,1);
	
	unsigned char donneesLues = 0;
	uint8_t j = 0;
	
	while(donneesLues != 0xff)
	{
		memoireExterne.lecture(j, &donneesLues); // lire une donnee a la fois et verifier que ce n'est pas 0xFF
		// mettre un delay apres chaque transmission pour laisser le temps 
		_delay_ms(5); 
		transmissionUART (donneesLues);
		j++;
		_delay_ms(5); 
	}
	_delay_ms(5);
	char message [] = "\nCaractere Trouve a l'adresse :";
	char newLine = '\n';
	for(uint8_t i = 0; i < sizeof(message)-1; i ++)
		transmissionUART(message[i]);
	transmissionUART(j);
	transmissionUART(newLine);
}
