#include "CapteurLigne.h"

/*
    @det    Constructeur de la classe 
    @param  void 
    @ret    void
*/
CapteurLigne::CapteurLigne()
{
    
}

/*
    @det    Methode qui allume les LEDs en fonctions du suiveur de ligne
    @param  void 
    @ret    void
*/
void CapteurLigne::setLED()
{
    can convertisseur;
    uint8_t valeur = 0;
    port_ = 0b00000000;
    uint8_t pinALire = 0;
    for(uint8_t i = 0; i < 5; i++)
    {
        pinALire = 6 - i;
        valeur = convertisseur.lecture(pinALire) >> 2;

        if(valeur > 127) // A Changer dependement de la hauteur fixee
            valeur = 0;
        else 
            valeur = 1;
        port_ |= valeur << pinALire;
    }
    PORTC = port_;
}

/*
    @det    Methode qui retourne la valeur en uint8 des pins 
                du capteur
                Les bits a 1 indiquent que le capteur correspondant est sur du Noir
                Les bits a 0 indiquent que le capteur correspondant est sur du Blanc
    @param  void 
    @ret    uint8      valeur du port
*/
uint8_t CapteurLigne::getValeurIR()
{
    setLED();
    return port_;
}