/*
 * Classe permettant de controler les moteurs avec
 * le microcontroleur ATMega16 de Atmel.
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
#ifndef MOTEUR_H
#define MOTEUR_H

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>

enum PourcentageVitesse{v0 = 100, v25 = 75, v50 = 50, v75 = 25, v100 = 0};
enum SensRotation{horaire = 0, antiHoraire = 1};
class Moteur
{
    public:
        Moteur();
        void avancer(const uint8_t& palierGauche, const uint8_t& palierDroite);
        void avancer(const PourcentageVitesse& roueGauche, const PourcentageVitesse& roueDroite);
        void reculer(const uint8_t& palierGauche, const uint8_t& palierDroite);
        void reculer(const PourcentageVitesse& roueGauche, const PourcentageVitesse& roueDroite);
        void arreter();
        void gauche90();
        void droite90();
        void tourneLuiMeme(const SensRotation& sens);
        ~Moteur();
    
    private:

};
#endif