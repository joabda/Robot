/*
 * Classe permettant de faire fonctionner le
 * capteur de lignes en convertissant le signal 
 * par le biais d'un CAN et ce pour l'utilisation
 * avec le microcontroleur ATMega16 de Atmel
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

#ifndef CAPTEURLIGNE_H
#define CAPTEURLIGNE_H

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include "can.h"
#include "UART.h"

class CapteurLigne
{
    public:
        CapteurLigne();
        void setLED();
        uint8_t getValeurIR();

    private:
        uint8_t port_;
};
#endif