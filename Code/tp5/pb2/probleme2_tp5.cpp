 #define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>

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
	/* Put data into buffer, sends the data */
		UDR0 = donnee;
		// p 174
}

int main()
{
	initialisationUART();
	char mots[21] = "Le robot en INF1900\n";
	for (uint8_t i = 0; i < 100; i++ )
	{
		for ( uint8_t j=0; j < 20; j++ ) 
			transmissionUART ( mots[j] );
		_delay_ms(5); 
		// mettre un delay apres chaque transmission pour laisser le temps 
	}y
}
