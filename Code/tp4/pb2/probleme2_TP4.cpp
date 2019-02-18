/*
 * Nom: Jeu avec la LED
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */

#define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t minuterieExpiree = 0;
volatile uint8_t boutonPoussoir = 0; 

void initialisation() 
{
	cli();
	// cli est une routine qui bloque toutes les interruptions.
	// Il serait bien mauvais d'être interrompu alors que
	// le microcontroleur n'est pas prêt...
	
	DDRB = 0xff; // PORT B est en mode sortie pour envoyer les couleurs a la DEL
	DDRD = 0x00; // PORT D est en mode lecture pour lire le boutton pressoire	
	// Configurer et choisir les ports pour les entrées et les sorties. DDRx... 
	// Initialisez bien vos 

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
	// de niveau du bouton-poussoir en a justant le registre EICRA

	sei();
	// sei permet de recevoir à nouveau des interruptions.
}

ISR ( TIMER1_COMPA_vect ) 
{
	minuterieExpiree = 1;
}


ISR ( INT0_vect ) 
{
	_delay_ms(30);
	// anti-rebond
	boutonPoussoir = 1;
	EIFR |= (1 << INTF0);
}

void partirMinuterie (const uint16_t duree ) 
//documentation de la minuterie 8 bits = page 91 de 'ATmega324 Manual.pdf' 
//(nous on sera sur 16 bits donc c'est quelques pages plus loin page 108)
{
	minuterieExpiree = 0;

	TCNT1 = 0x0 ;
	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée

	OCR1A = duree;
	//"output compare" = le temps qu'on veut que la minuterie dure

	TCCR1A = (1 << WGM12); 
		// Pour configurer OCR1A a CTC, p 128-129 ATMEGA

	TCCR1B = (1 << CS12) | (1 << CS10);
		// Pour divier la clock de 1024, p 130-131 ATMEGA
		
	TCCR1C = 0;
		// Pour forcer une comparaison avec la channel A (ici OCR1A)

	TIMSK1 = (1 << OCIE1A);
}

void attendreDixSecEtClignoter(uint8_t eteint, uint8_t rouge)
// attendre 10 secondes et faire clignoter la LED en rouge durant 1/10s 
// avant de "commencer le jeu de réflexe"
{
	_delay_ms(10000);
	PORTB = rouge;
	_delay_ms(100);
	PORTB = eteint;
}


int main()
{
	initialisation();
	const uint8_t rouge = 0x1; 
	const uint8_t vert = ~0x1; 
	const uint8_t eteint = 0x0; 
		
	PORTB = eteint;
	// initialisation de la LED éteinte
	cli();
	attendreDixSecEtClignoter(eteint, rouge);
	//voir fonction
	sei();
	const uint16_t duree = 7812;
	partirMinuterie( duree );
	
	do {
		// on garde ici vide c'est juste qu'on attend une interreuption
		// attendre qu'une des deux variables soit modifiée
		// par une ou l'autre des interruptions.
	} while ( minuterieExpiree == 0 && boutonPoussoir == 0 );

	cli ();
	// Une interruption s'est produite. Arrêter toute forme d'interruption. 
	// Une seule réponse suffit.

	// Verifier la réponse :
	if (boutonPoussoir == 1)
		PORTB = vert;
		//Si l'interruption venait du bouton, c'est que l'utilisateur a appuye a temps sur le bouton 
		//et donc la LED affiche vert jusqu'a pression du bouton reset
	
	else 
		if ( minuterieExpiree == 1 )
		PORTB = rouge;
		//Si l'interruption venait de la minuterie, c'est que l'utilisateur n'a pas appuye a temps et 
		//donc la LED affichera rouge jusqu'a pression du bouton reset.
	
	sei(); 	//inutile ? normalement pas besoin de re-enable les interruptions, car dans la doc il est dit 
			//que RESET est plus fort que 'cli()'
	
	return 0; 
}
