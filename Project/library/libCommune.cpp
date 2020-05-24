#include "libCommune.h"

const uint8_t rougeLED = 0x1;
const uint8_t vertLED = 0x2;

bool lectureBoutonDebounce_logiciel() 
// Fonction de lecture d'un bouton avec debouncing logiciel
{
    bool valeurBouton1 = 0;
    
    if (PIND & 0x04){ // Lecture du bouton
        valeurBouton1 = 1;
        _delay_ms (10);
        if ( PIND & 0x04 && valeurBouton1 == 1){ // Deuxieme lecture du bouton, avec delay de 10 ms
            return 1; // Si les deux lecture sont true, le bouton est true (appuyer)
        }
    }
    return 0;
} // Tp1

/*
	@det    Fonction qui permet les interruption avec le bouton poussoir
	@param  void
	@ret    void
*/
void initialisationBouttonPoussoir()
{
	cli();
	DDRD = 0x00; // PORT D est en mode lecture pour lire le boutton pressoire	

	EIMSK |= (1 << INT0) ;
	//Cette procédure ajuste le registre EIMSK de l’ATmega324PA pour permettre les interruptions externes

	EICRA |= (1 << ISC00) ;
	// Apres cete operation EICRA est egale a 3 et EICRA Gere les ISC
	// il faut sensibiliser les interruptions externes aux changements 
	// de niveau du bouton-poussoir en a justant le registre EICRA

	sei();
} //Tp4 pb2