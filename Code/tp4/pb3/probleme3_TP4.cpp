 #define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

void ajustementPWM (const uint8_t palier) 
{
	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	// page 177 de la description technique du ATmega324PA)
	
	OCR1A = palier;
	OCR1B = palier;
	// si on veut tourner les roues a la meme vitesse on met les deux paliers de 
	// OCR1A ET OCR1B a la meme valeure
	// SINON on set chacun a une valeure

	TCCR1A |= (1 << COM1A1)| (1 << COM1B1) | (1 << COM1A0) | (1 << COM1B0) | (1 << WGM10);
	// D'apres page 129 tableau 16.4
	// Le compteur de 16 bits (TCNT1 est fait de compteurs de 8 bits (TCCR1A ET TCCR1B))

	TCCR1B |= 1 << CS11;
	// division d'horloge par 8 - implique une frequence de PWM fixe

	TCCR1C = 0;
}

int main()
{
	DDRD = 0xff;
	while (true)
	{
		for(uint8_t pourcentageVitesse = 255; pourcentageVitesse >= 0; pourcentageVitesse-= 63) // 63 car 255/4 = 63.5 
		{
			ajustementPWM(pourcentageVitesse);
			_delay_ms(2000);
		}
	}
	return 0; 
}
