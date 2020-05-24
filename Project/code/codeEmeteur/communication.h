/*
 * Classe permettant de faire communiquer nos 2 robots, soit en mode émission soit en mode réception.
 * 
 * En mode emission : appeler getPressionsBouton(), puis emettreSignal().
 * 1) Détection du nombre de pressions du bouton
 * 2) Emission selon SIRC : 
 *  - signal d'initialisation : une émission de 2.4ms, puis un blanc de 0.6 ms
 *  - emission de la commande : 7 bits, un 1 correspond à une émission de 1.2ms, un 0 correspond à une émission de 0.6ms - si on a appuyé 1 fois sur le bouton, on envoie 1000 0000, 2 on envoie 010 0000...
 *  - emission de l'adresse : 5 bits, un 1 correspond à une émission de 1.2ms, un 0 correspond à une émission de 0.6ms - note : l'adresse est toujours la même à savoir 1, donc on transmettra toujours 1 0000
 * 
 * 
 * Exemple de 5 appuis sur le bouton :
 * Après sequenceInit, on enverra 1010 0001 0000 (12 bits)
 *  * Note : ici on utilise le transmetteur uniquement en SIRC 12 bits car pas besoin de +, mais il existe aussi des versions 15 et 20 bits
 * Reception sur PORTA 1 (1 sur carte)
 * Se référer au pdf "SIRC.pdf"
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <util/delay.h>
#include "minuterie.h"

enum ER {emission = 0, reception = 1};

class Communication 
{
    public:
        Communication(const ER& type);
        void setCommande(uint8_t nbPressionsTest);
        void getPressionsBouton();
        void emettreSignal();
        uint16_t getSignal();

    private:
        ER type_;
        uint8_t commande_[7];
        uint8_t adresse_[5]; // l'adresse est toujours la même
        uint8_t reception_[12];
        void sequenceInit() const;
        void emettre_0() const;
        void emettre_1() const;
        void emettreCommande();
        void emettreAdresse();
};

void resetVaribles();

#endif