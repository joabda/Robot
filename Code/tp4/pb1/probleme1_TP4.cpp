/* Ce programme a pour but de programmer la DEL du robot comme demande dans le sujet par interrupt.
 * Date: 2019-01-29
 * Auteurs: ABDO	Joe			1939689
			CHRITIN Mathurin 	1883619
 * probleme3_1939689_1883619.cpp


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
#include <avr/interrupt.h>

enum Etats {INIT=0, AMBRE=1, VERT=2, ROUGE2=3, ETEINT=4, VERT2=5}; 
volatile uint8_t currState = 0;
// Volatile pour une variable qu'on veut que le compilateur ne change pas

void initialisation() 
{
	cli();
// cli est une routine qui bloque toutes les interruptions.
// Il serait bien mauvais d'être interrompu alors que
// le microcontroleur n'est pas prêt...
	
	DDRB = 0xff; // PORT B est en mode sortie pour envoyer les couleurs a la DEL
	DDRD = 0x00; // PORT D est en mode lecture pour lire le boutton pressoire	
// Configurer et choisir les ports pour les entrées et les sorties. DDRx... 
// Initialisez bien vos variables

	EIMSK |= (1 << INT0) ; // Decale vers la gauche de 1 bit.
//Cette procédure ajuste le registre EIMSK de l’ATmega324PA pour permettre les interruptions externes
		//EIMSK est egale a 0 et contient 8 bits en debut de programme gere les INT
		//INT0 vaut 0, INT1 vaut 1, INT2 vaut 2.
		// en utilisant ISCn1 en remplacant n par le numero de INT qu'on utilise

	EICRA |= (1 << ISC00) ; // Int de ISC00 a 1
	//EICRA |= (1 << ISC01) ;	// Int de ISC01 a 1
		// Apres cete operation EICRA est egale a 3  
		// EICRA Gere les ISC
// il faut sensibiliser les interruptions externes aux changements 
// de niveau du bouton-poussoir en ajustant le registre EICRA

	sei();
// sei permet de recevoir à nouveau des interruptions.
}

ISR(INT0_vect) 
{
	_delay_ms(30);
	// laisser un delai avant de confirmer la réponse du bouton-poussoir: 
	//environ 30 ms (anti-rebond)
	
	switch(currState)
	{
		case INIT:		_delay_ms(30); // on delay pr voir si le bouton est tjr appuiye
		if( (PIND & 0x04))
			currState++;
		break;
		
		case AMBRE: 	currState++;
		break;
		
		case VERT:		_delay_ms(30);
		if( (PIND & 0x04))
			currState++;
		break;
		
		case ROUGE2:	currState++;
		break;
		
		case ETEINT:	_delay_ms(30);
		if((PIND & 0x04))
			currState++;
		break;
		
		case VERT2:		currState = 0;
		break;
	}
	EIFR |= (1 << INTF0);
}

int main()
{
	initialisation();
	
	while(true)
	{
		switch(currState)
		{
			case INIT: 		PORTB = 0x01;
			break;
			PORTB = 0x00;
			case AMBRE: 
			{
				PORTB = 0x01;	
				_delay_ms(1);
				PORTB = ~PORTB;
				_delay_ms(7); 
				// RATIO DE 1:7 (ROUGE:VERT) POUR UN MEILLEUR COULEUR AMBREE
			}
			break;
			case VERT: 		PORTB = ~0x01;
			break;
			
			case ROUGE2: 	PORTB = 0x01;
			break;
			
			case ETEINT:	PORTB = 0x00;
			break;
			
			case VERT2: 	PORTB= ~ 0x01;
			break;
		}
	}
	return 0; 
}
