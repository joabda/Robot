/*
 * Code du projet final du robot emmeteur
 *          de signaux infrarouge
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Theo Quiquempoix, Philippe Babin, Joe Abdo et Mathurin Chritin
 * 2019
 *
 * Code qui n'est sous aucune license.
 *
 */

#include "communication.h"
#include <avr/io.h>
// #include <util/delay.h>
// #include <avr/interrupt.h>
// #include <stdio.h>

void emissionDeSignal(Communication communicationE)
{
	// Le signal à envoyer est maintenant dans l'attribut commande_[]s
	communicationE.getPressionsBouton(); 
	_delay_ms(1000);
	PORTC = 0x01; // Rouge durant l emission 
	for(uint8_t i  = 0; i < 50; i++) 
		communicationE.emettreSignal();
}

int main()
{
	DDRC = 0xff; 	// En sortie pour la LED 
	DDRB = 0xff;	// En sortie pour l'emeteur IR

	Communication communicationE(emission); // Envoie du numero de la section de debut
	emissionDeSignal(communicationE);

	// Reinitialisation
	PORTC = 0x02; // Vert quand il a finit d emettre
}