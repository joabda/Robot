#include "led.h"

/*
    @det    Constructeur de la classe qui initialise les attributs aux parametress, par default
                initialise au port B broche 0 et 1 a etein
    @param  Port de branchement, pin de branchement et etat de la LED 
    @ret    void
*/
LED::LED(const Port& port, const Etat& etat, const uint8_t& brochePetite, const uint8_t& brocheGrande)
{
    port_ = port;
    etat_ = etat;

    // Dans les deux cas on fait des masks sur les deux broche pour que si 
    //  l'utilisateur veut brancher sur pin 2 et 3 pas 0 et 1 ...
    broches_[0] = 1 << brocheGrande;
    broches_[1] = 1 << brochePetite;
}

/*
    @det    Methode qui active la LED a l'etat deja definit
    @param  void
    @ret    void
*/
void LED::activer() const
{
    switch (etat_)
    {
        case ledRouge: couleur(broches_[0], broches_[1]);
            break;

        case ledVerte:couleur(broches_[1], broches_[0]); 
        // Si on veut la led a vert on fait que inverser les deux broche pour 
        //  ne pas changer le comportement de la fonction
            break;
    
        default: eteindre();
            break;
    }
}

/*
    @det    Methode qui allume la LED
    @param  Broches de branchement de la LED
    @ret    void
*/
void LED::couleur(const uint8_t& broche0, const uint8_t& broche1) const
{
    switch (port_)
    {
        case A: 
        {
            DDRA = 0xff;
            // Cela met le bit a la position brohe_0 a 1 sans changer l'etat des autres bits
            PORTA |= broche0; 
            PORTA &= ~broche1; 
            // ici on a broche[1] en 0 avec un 1 a la position broche 1, on inverse 
                //et on a tout des 0 sauf a la position broche 1
            // par la suite on fait un & et donc on ne changera l'etat que du bit 
                //a la position broche_[1]
        }
            break;
        
        case B:  
        {
            DDRB = 0xff;
            PORTB |= broche0; 
            PORTB &= ~broche1; 
        }
            break;

        case C:
        {
            DDRC = 0xff;
            PORTC |= broche0; 
            PORTC &= ~broche1; 
        }
            break;

        case D:
        {
            DDRD = 0xff;
            PORTD |= broche0; 
            PORTD &= ~broche1; 
        }
            break;
    }
}


/*
    @det    Methode qui desactive la LED
    @param  void
    @ret    void
*/
void LED::eteindre() const
{
    switch (port_)
    {
        case A: PORTA = ledEteinte;
            break;
        
        case B: PORTB = ledEteinte;
            break;

        case C: PORTC = ledEteinte;
            break;

        case D: PORTD = ledEteinte;
            break;
    }
}

/*
    @det    Methode getter du port de branchement
    @param  void
    @ret    Port de branchement de la LED
*/
Port LED::getPort() const
{
    return port_;
}

/*
    @det    Methode getter du port de branchement
    @param  void
    @ret    Etat de la LED
*/
Etat LED::getEtat() const
{
    return etat_;
}

/*
    @det    Methode setter qui met sur quel Port la LED devrait etre branchee
    @param  Port a mettre la LED 
    @ret    void
*/
void LED::setPort(const Port& port)
{
    port_ = port;
}

/*
    @det    Methode setter qui met l'etat de la LED
    @param  Port a mettre la LED 
    @ret    void
*/
void LED::setEtat(const Etat& etat)
{
    etat_ = etat;
    activer();
}