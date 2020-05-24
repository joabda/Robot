/*
 * Classe permettant de faire fonctionner le
 * buzzer avec le microcontroleur ATMega16 de Atmel.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1900
 *
 * Joe Abdo, Mathurin Critin, Teo Quiquempoix et Philippe Babin, 
 * 2019
 *
 * Code qui n'est sous aucune license.
 *
 */

#ifndef BUZZER_H
#define BUZZER_H

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include "minuterie.h"
#include <util/delay.h>

/*
*   Lorsqu'on crée une classe buzzer, le signal sortira sur les broches 5 et 6 du PORTD.
*   La broche 5 est la mise à la Terre et doit donc rester à 0
*   La broche 6 recoit le signal PWM pour générer le son de la fréquence voulue
*/
class Buzzer 
{
    public:
        Buzzer();
        ~Buzzer();
        void faireBuzzer(const uint16_t& frequence) const;
        void arreterBuzzer();
        void buzzerFF();
        void buzzerInit();
        void buzzerMegalo();
        void buzzerRectangle();
        void buzzerFini();
        void buzzerRotation();
};

#endif