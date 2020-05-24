/* Ce programme a pour but de programmer la DEL du robot comme demande dans le sujet.
	par interruption du bouton sur le breadboard ou par interruption de fin de temps
	le programme execute une sequence de changements de couleurs sur la LED
 * Date: 2019-02-22
 * Auteurs: ABDO	Joe			1939689
	  CHRITIN Mathurin 			1883619
 * probleme1_1939689_1883619.cpp
 * */

/*

	EP			B	Min	ES			Sortie
	--------------------------------------------
	init		0	0	INIT		00		
	init		X	1	vert1		00	
	init		1	X	vert1		00	
	--------------------------------------------
	vert1		X	X	eteint		10
	--------------------------------------------
	eteint		X	X	clignoter	00
	--------------------------------------------
	clignoter	X	X	vert2		01/10
	--------------------------------------------
	vert2		X	X	init		10
	--------------------------------------------

*EB 	= ETAT PRESENT 
*B 		= BUTTON (entree)
*Min 	= Minuterie (entree) 
*ES 	= ETAT SUIVANT
*Sortie (DEL) 

	Brancher la LED au PORTB sur les pins 0 et 1 
	Brancher le cables rouge et noir (pour l'alimentation du breadboard) sur nimporte quel port tant que rougue sur VCC et le noir sur GND
	Branche le fil bleu pour les signaux d'interruption sur la PORTD pin 2 (3 sur la carte mere)
*/

#define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

volatile bool boutonPoussoir = false; 
	//True quand il est presse
	//False quand il est relache
volatile bool boutonPoissoirEtatAncien = true;

volatile uint8_t compteur = 0;
volatile uint8_t compteurEtats = 0;
const uint8_t eteinteLED 	= 0x0; 
const uint8_t rougeLED 		= 0x1;
const uint8_t vertLED 		= 0x2;


void initialisation() 
//Fonction d'initialisation pour les ports et les interruptions
{
	cli();
	
	DDRB = 0xff; 		// PORT B est en mode sortie pour envoyer les couleurs a la DEL
	DDRD = 0x00; 		// PORT D est en mode lecture pour lire le boutton pressoire	

	EIMSK |= (1 << INT0) ; 
						// Decale vers la gauche de 1 bit.
						// Pour permettre les interruptions externes


	EICRA |= (1 << ISC00) ; 
	// Int de ISC01 a 1
	// Apres cete operation EICRA est egale a 3  
	// EICRA Gere les ISC
	// il faut sensibiliser les interruptions externes aux changements 
	// de niveau du bouton-poussoir en a justant le registre EICRA

	sei();
						// sei permet de recevoir à nouveau des interruptions.
}


void partirMinuterie (const uint16_t duree ) 
/* Fonction de minuterie cette fonction prend en parametre une duree
	cette variable duree est la valeur jusqu'a la quelle il faut compter 
	notre horloge est divisee par 1024, donc il faut faire F_CPU / 1024 cycle horloge (valeur de duree)
	pour que UNE seconde s'ecoule 
*/
{
	TCNT1 = 0x0 ;
							// mode CTC du timer 1 avec horloge divisée par 1024
							// interruption après la durée spécifiée

	OCR1A = duree;
							//"output compare" = le temps qu'on veut que la minuterie dure

	TCCR1A = (1 << WGM12); 
							// Pour configurer OCR1A a CTC, p 128-129 ATMEGA

	TCCR1B |=(1 << CS12) | (1 << CS10);
							// Pour divier la clock de 1024, p 130-131 ATMEGA
		
	TCCR1C = 0;
							// Pour forcer une comparaison avec la channel A (ici OCR1A)

	TIMSK1 = (1 << OCIE1A);
}

ISR ( TIMER1_COMPA_vect ) 
/* Fonction d'interruption sur le compteur (Timer1)
	cette fonction active d'autre minuterie tant quee 12 secondes ne sont pas ecoulee 
	12 secondes CAD que la variable compteur n'est pas arrive a 120 (on ajout 10 a chaque seconde)
*/
{
	compteur += 10;
	if(compteur == 120)
		compteurEtats++;
	else 
		partirMinuterie(7812);
}


ISR ( INT0_vect ) 
/* Fonction d'interruption sur le boutton pressoire
	elle part la minuterie des que le bouton est presse 
	elle met ensuite l'etat du boutton dans le variable bouttonPoussoir ( 0 si presse et 1 si relache)
*/
{
	_delay_ms(30);	
							// anti-rebond

	partirMinuterie(7812); 
							// On part la minuterie des que le bouton est pressee
							// 7812  = F_CPU/1024 (on a divise la clock par 2014) c'est le nombre de cycles pour passer une seconde

	boutonPoissoirEtatAncien = boutonPoussoir; 
	boutonPoussoir = !boutonPoussoir; 
							// A chaque foi qu'une interruption a lieu sur le boutton poussoir
							// On echange sont etat de presse a relache et vice-versa

	if(boutonPoissoirEtatAncien && !boutonPoussoir) 
		compteurEtats++;
							// La condition dit que si on arrive a un etat ou le boutonAncien est presse et le boutton est ensuite relache alors passer a l'etat suivant

	EIFR |= (1 << INTF0);
}

void clignoterRouge()
// Fonction qui permet a la LED de clignotter a un rythme de 2 fois par secondes pour compteur /2 fois
{
	for(uint16_t i = 0; i < compteur/2 ; i++)
	{
		PORTB = 0x1;
		_delay_ms(250);
		PORTB = 0x0;
		_delay_ms(250);
		PORTB = 0x1;
	}
}

void arreterInterruptions()
/*
	Cette fonction arrete les interruptions le temps d'executer la sequence
	pour les changements de couleurs de la LED 
*/
{
	cli(); 					//Quand la premiere interruption a lieux (du au bouton ou a la minuterie) on ne veux plus etre interrompus
	OCR1A = 0;
	TCCR1A = 0;
	TCCR1B |= 0;
	TIMSK1 =  0;
}

int main()
{
	initialisation();					//Initialisation
	enum etats{init = 0, vert1 = 1, eteint = 2, clignoter = 3, vert2 = 4};

	/*
	Comme le charge de laboratoire l'a bien explique on a le droite de faire des delays
	a condition de bien savoir que la sequence ne PEUT pas etre interompuee
	*/

	while(true)
		switch (compteurEtats)
		{
			case init: PORTB = eteinteLED;
				break;
			case vert1:
			{ 
				arreterInterruptions();
				PORTB = vertLED;
				_delay_ms(500);				
				compteurEtats++;
			}
				break;
			case eteint:
			{	
				PORTB = eteinteLED;
				_delay_ms(2000); 		//Enonce dit que la carte mere ne fait rien pour 2s on peut donc faire un delays }
				compteurEtats++;
			}
				break;
			case clignoter:
			{
				clignoterRouge();
				compteurEtats++;
			}
				break;
			case vert2:
			{
				PORTB = vertLED;
				_delay_ms(1000);
				sei(); 					//Active de nouveau les interruptions
				compteurEtats = 0;
			}
		}
}