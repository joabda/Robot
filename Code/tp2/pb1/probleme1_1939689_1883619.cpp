/* Ce programme a pour but de programmer la DEL du robot comme demande dans le sujet.
 * Date: 2019-01-15
 * Auteurs: ABDO	Joe			1939689
	  CHRITIN Mathurin 			1883619
 * probleme1_1939689_1883619.cpp


	EP		B	ES		Sortie
	---------------------------
	INIT	0	INIT	00		
	INIT	1	PUSH1	00
	---------------------------
	PUSH1	0	PUSH1	00
	PUSH1   1	PUSH2	00
	---------------------------
	PUSH2	0	PUSH2	00
	PUSH2   1	PUSH3	00
	---------------------------
	PUSH3	0	PUSH3	00
	PUSH3   1	PUSH4	00
	---------------------------
	PUSH4	0	PUSH4	00
	PUSH4   1	ALLUME	00
	---------------------------
	ALLUME	X	INIT	01 // A REVOIR 01 OU 10
	---------------------------
	
*EB = ETAT PRESENT 
*B = BUTTON (entree) 
*ES = ETAT SUIVANT
*Sortie (DEL)  

Brancher la DEL sur le PORTB broches 1 et 2

*/
#define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>

void init()
// Fonction qui initialize les PORT
{
	DDRB = 0xff; // PORT B est en mode sortie pour envoyer les couleurs a la DEL
	DDRD = 0x00; // PORT D est en mode lecture pour lire le boutton pressoire	
}

int main()
{
	init();
	enum Etats {INIT=0, PUSH1=1, PUSH2=2, PUSH3=3, PUSH4=4, ALLUME=5}; 
	
	int compteur = 0; 
	// le compteur est initialise a 0 il indique le nbr de fois que le bouton poussoir 
	//	a ete presse

	PORTB = 0x00;	// on initialise la lampe a eteinte au depart
	
	for(;;)  // boucle sans fin
	{
		if(PIND & 0x04)
		{
			_delay_ms(10);
			if((PIND & 0x04) == 0)
				compteur++;
		}
		
		switch(compteur)
		{
			case INIT: 
			{
				PORTB = 0x00;	// lampe eteinte 
				break;
			}
			
			case PUSH1: 
			{
				PORTB = 0x00;	// lampe eteinte
				break;
			}
			
			case PUSH2: 
			{
				PORTB = 0x00;	// lampe eteinte 
				break;
			}
			
			case PUSH3: 
			{
				PORTB = 0x00;	// lampe eteinte 
				break;
			}
			
			case PUSH4: 
			{
				PORTB = 0x00;	// lampe eteinte
				break;
			}
			
			case ALLUME: 
			{
				PORTB = 0x01;	// Ici la lampe s'allume en rouge 
				_delay_ms(1000);	// pour une seconde (1000ms)
				compteur = 0;
				break;
			}
		}
	}
	return 0; 
}
