/* Ce programme a pour but de programmer la DEL du robot comme demande dans le sujet par polling.
 * Date: 2019-01-15
 * Auteurs: ABDO	Joe			1939689
			CHRITIN Mathurin 	1883619
 * probleme2_1939689_1883619.cpp


	EP		B	ES		Sortie
	---------------------------
	INIT	0	INIT	00		
	INIT	1	AMBRE	00
	---------------------------
	AMBRE	0	VERT	01/10
	AMBRE   1	AMBRE	01/10
	---------------------------
	VERT	0	VERT	10
	VERT  	1	ROUGE2	10
	---------------------------
	ROUGE2	0	ETEINT	01
	ROUGE2  1	VERT2	01
	---------------------------
	VERT2	0	INIT	10
	VERT2   1	VERT2	10
	---------------------------
	
*EB = ETAT PRESENT 
*B = BUTTON (entree) 
*ES = ETAT SUIVANT
*Sortie (DEL)  

La DEL est branchee sur le PORTB les broches 1 et 2

*/

#define F_CPU 8000000 // 8 MHz -- Define toujours au debut
#include <avr/io.h> 
#include <util/delay.h>


bool debounce()
// Fonction qui debounce le boutton pressoire
// Cette fonction retourne true si le boutton a ete presse SI il n'est pas presse elle retourne false
{
	if(PIND & 0x04)
	{
		_delay_ms(10);
		if(PIND & 0x04)
			return true;
		else
			return false;
	}
	
	else
		return false;
}

void init()
{
	DDRB = 0xff; // PORT B est en mode sortie pour envoyer les couleurs a la DEL
	DDRD = 0x00; // PORT D est en mode lecture pour lire le boutton pressoire	
}

int main()
{
	init();
	enum Etats {INIT=0, AMBRE=1, VERT=2, ROUGE2=3, ETEINT=4, VERT2=5}; 
	
	int compteur = 0; 
	// le compteur est initialise a 0 il indique le nbr de fois que le bouton poussoir 
	//	a ete presse

	PORTB = 0x01;
	
	for(;;)  // boucle sans fin
	{	
		switch(compteur)
		{
			case INIT: 
			{
				PORTB = 0x01;
				if(debounce())
					compteur++;
				break;
			}
			
			case AMBRE: 
			{
				while(debounce())
				{
					PORTB = 0x01;	
					_delay_ms(1);
					PORTB = ~PORTB;
					_delay_ms(7); 
					// RATIO DE 1:7 (ROUGE:VERT) POUR UN MEILLEUR COULEUR AMBREE
				}
				compteur++;
				break;
			}
			case VERT: 
			{
				while(debounce() == false)
					PORTB = ~0x01;
				if(debounce())
					compteur++;
				break;
			}
			case ROUGE2:
			{
				while(debounce())
					PORTB = 0x01;
				compteur++;
				break;
			}
			case ETEINT:
			{
				while(debounce() == false)
					PORTB = 0x00;
			}
			case VERT2:
			{
				while(debounce())
					PORTB= ~ 0x01;
				compteur = 0;
				break;
			}
		}
	}
	return 0; 
}
