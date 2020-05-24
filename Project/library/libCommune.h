/*
 * Librairie qui contient differentes classe qui aiderons
 * a la realisation de differents projets.
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1900
 *
 * Theo Quiquempoix, Philippe Babin, Joe Abdo et Mathurin Chritin
 * 2019
 *
 * Code qui n'est sous aucune license.
 *
 */

#ifndef LIB_COMMUNE
#define LIB_COMMUNE

#ifndef F_CPU
#define F_CPU 8000000
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "led.h"
#include "minuterie.h"
#include "moteur.h"
#include "can.h"
#include "buzzer.h"
#include "UART.h"
#include "CapteurLigne.h"

bool lectureBoutonDebounce_logiciel();
void initialisationBouttonPoussoir();
#endif