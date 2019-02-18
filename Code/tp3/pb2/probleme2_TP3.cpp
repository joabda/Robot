/*
 * Nom: Faire Avancer une Roue
 * Description: Ceci fait tourner une roue a 60 et 400hz a differents pourcentages 
 * Version: 1.1
 */


#define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>

void init()
{
	DDRB = 0xff; // PORT B est en mode sortie
}

void avancer()
{
	PORTB = 0b00000011;
}
void arreter()
{
	PORTB &= ~0b00000011; // donne 0b00000000 
}

void reculer()
{
	PORTB = 0b00000001;
}

void delay(const uint16_t nbSec)
{
	for(uint16_t k = 0; k < nbSec; k++)
		_delay_us(1);
}

void tournerRoue(const uint16_t& frequence)
{
	uint16_t nbRep2sec = 2 * frequence; // 2secondes / (1/frequence) = 2* frequence
					    // donne 800 pr 400hz et 120 pr 60hz

	for(uint8_t i = 0; i <= 4; i++) // i<4 car 25/100 = 1/4, 50/100=2/4, 75/100 = 3/4
		for(uint16_t j = 0; j < nbRep2sec; j++)
		{
			avancer();
			for(uint8_t k = 0; k < i; k++)
			{
				if(frequence == 60)
					delay(2083);
				else if(frequence == 400)
					delay(313);
			}
				
			arreter();
			for(uint8_t k = 0; k < (4-i); k++)
			{
				if(frequence == 60)
					delay(2083);
				else if(frequence == 400)
					delay(313);
			}
		}
}


int main()
{	
	init();
	
	while(true)
	{	
		avancer();
		
		uint16_t frequence = 60;
		tournerRoue(frequence);		

		frequence = 400;
		tournerRoue(frequence);	
	}
	return 0; 
}

