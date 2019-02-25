/* Ce programme a pour but de programmer la DEL du robot en fonction de la luminosité ambiante : 
 * - Verte lorsque la diode est cachée, 
 * - Ambrée à luminosité ambiante 
 * - Rouge sous une lampe torche
 * Date: 2019-02-16
 * Auteurs: ABDO	Joe			1939689
	  CHRITIN Mathurin 			1883619
 * probleme2_1939689_1883619.cpp
 * */
 
 
#define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>
#include "can.h"


 // On branche notre photorésistance sur la PINA0
 // On branche la LED sur PINB0-1 
	 
	 
void initialisation() 
// Fonction d'initialisation pour les ports et les interruptions
{

	DDRB = 0xff; // PORT B est en mode sortie pour envoyer les couleurs a la DEL
	DDRA = 0x00; // PORT D est en mode lecture pour lire le boutton pressoire	
}

void ambree(const uint8_t& rougeLED, const uint8_t& vertLED)
/* 
 * Fonction qui envoie les valeurs nécessaires de rouge et de vert (passés en paramètre) pour que la LED prenne une couleur ambrée.
*/
{
	PORTB = rougeLED;
	_delay_ms(1);
	PORTB = vertLED;
	_delay_ms(7);
}

int main()
{	
	initialisation();

	const uint8_t eteinteLED 	= 0x0; 
	const uint8_t rougeLED 		= 0x1;
	const uint8_t vertLED 		= 0x2;
	
	PORTB = eteinteLED; //Initialisation de la DEl à éteinte
	
	//La photorésistance renvoie une valeur entre 160 et 260 à luminosité du laboratoire (220 environ)
	const uint8_t luminositeForte =  240;
	const uint8_t luminositeFaible = 180;	
	
	can photoresistance; //Creation d'un objet photoresistance (voir can.h can.cpp)

	
	
	while(true) 
	{
		
		uint16_t lecture = (photoresistance.lecture(0) >> 2); // on décale de 2 comme mentiionné dans l'énoncé : les deux derniers bits ne sont pas significatifs

		// luminosité forte (lampe torche)
		if (lecture > luminositeForte)
			PORTB = rougeLED;
		
		// luminosité ambiante (lampes du labo)
		if (lecture <= luminositeForte && lecture >= luminositeFaible)
			ambree(rougeLED, vertLED);		
		
		// luminosité faible (photoresistance cachée)
		if (lecture < luminositeFaible)
			PORTB = vertLED;

 		_delay_ms(10); //delay parce qu'on n'a pas besoin d'autre chose pendant ce temps, et pas besoin de lire la valeur de la photoresistance à chaque tick d'horloge 
	

	}
}
