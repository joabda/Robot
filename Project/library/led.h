/*
 * Classe permettant differents reglages de la LED
 * du microcontroleur ATMega16 de Atmel.
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

#ifndef LED_H
#define LED_H

#include <avr/io.h>

enum Port {A = 0, B = 1, C = 2, D = 3};
enum Etat{ledEteinte = 0, ledRouge = 1, ledVerte = 2};

class LED {
    public:
        LED(const Port& port = B, const Etat& etat = ledRouge, const uint8_t& broche1 = 0, const uint8_t& broche2 = 1);
        void activer() const;
        void eteindre() const;

        //getters
        Port getPort() const;
        Etat getEtat() const;
        
        //setters
        void setEtat(const Etat& etat);
        void setPort(const Port& port);
   
    private:
        Port port_;
        Etat etat_;
        uint8_t broches_[2];
        void couleur(const uint8_t& broche0, const uint8_t& broche1) const;
};
#endif