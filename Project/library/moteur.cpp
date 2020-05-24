#include "moteur.h"
/*
    @det    Constructeur de la classe construit un moteur
                avec division horloge de 8, en phase correct.
    @param  void
    @ret    void
*/
Moteur::Moteur()
{
    TCCR0A |= (1 << COM0A1)| (1 << COM0B1) | (1 << COM0A0) | (1 << COM0B0) | (1 << WGM00);
    // Phase correct 
    TCCR0B |= 1 << CS01;
    // Division horloge par 8
    OCR0A = 0xFF;
    OCR0B = 0xFF;
    // Pour ne pas avancer

    PORTB &= ~( (1 << 2) | (1 << 5) );
    // Mettre la direction a avancer
}

/*
    @det    Methode pour faire avancer le robot a des paliers
                cette methode ajuste automatiquement l'avancement du robot en lisant la valeur
                du capteur de lignes
    @param  uint8 Valeurs entre 0 et 255 des paliers (0 +rapide) (255 +lent)
    @ret    void
*/
void Moteur::avancer(const uint8_t& palierGauche, const uint8_t& palierDroite)
{
    PORTB &= ~( (1 << 2) | (1 << 5) );
    OCR0A = palierDroite;
    OCR0B = palierGauche;
}


/*
    @det    Methode pour faire avancer le robot a des paliers
    @param  PourcentageVitesse des pourcentage de vitesse v0,v25,v50,v75,v100
    @ret    void
*/
void Moteur::avancer(const PourcentageVitesse& roueGauche, const PourcentageVitesse& roueDroite)
{
    uint8_t vitesseDroite = (roueDroite * 255) / 100 ;
    uint8_t vitesseGauche = roueGauche * 255 / 100 ;
    avancer(vitesseDroite, vitesseGauche);
}

/*
    @det    Methode pour faire reculer le robot a des paliers
    @param  uint8 Valeurs entre 0 et 255 des paliers (0 +rapide) (255 +lent)
    @ret    void
*/
void Moteur::reculer(const uint8_t& palierGauche, const uint8_t& palierDroite)
{
    PORTB |= (1 << 2) | (1 << 5);
    OCR0A = palierDroite;
    OCR0B = palierGauche;
}

/*
    @det    Methode pour faire reculer le robot a des paliers
    @param  PourcentageVitesse des pourcentage de vitesse v0,v25,v50,v75,v100
    @ret    void
*/
void Moteur::reculer(const PourcentageVitesse& roueGauche, const PourcentageVitesse& roueDroite)
{
    uint8_t vitesseDroite = (roueDroite * 255) / 100 ;
    uint8_t vitesseGauche = roueGauche * 255 / 100 ;
    reculer(vitesseDroite, vitesseGauche);
}

/*
    @det    Methode pour faire tourner le robot a 90 degree a droite
    @param  void
    @ret    void
*/
void Moteur::droite90()
{
    PORTB &= ~  (1 << 2);
    PORTB |=    (1 << 5);
    OCR0A = 100;
    OCR0B = 100;
    _delay_ms(850);
    OCR0A = 255;
    OCR0B = 255;
}

/*
    @det    Methode pour faire tourner le robot a 90 degree a gauche
    @param  voidd
    @ret    void
*/
void Moteur::gauche90()
{
    PORTB &= ~  (1 << 5);
    PORTB |=    (1 << 2);
    OCR0A = 100;
    OCR0B = 100;
    _delay_ms(920);
    OCR0A = 255;
    OCR0B = 255;
}

/*
    @det    Methode pour faire tourner le robot sur lui meme 
                en sens anti-horaire
    @param  void
    @ret    void
*/
void Moteur::tourneLuiMeme(const SensRotation& sens)
{
    if(sens == antiHoraire)
    {
        PORTB &= ~  (1 << 5);
        PORTB |=    (1 << 2);
    }
    else
    {
        PORTB &= ~  (1 << 2);
        PORTB |=    (1 << 5);
    }     
    OCR0A = 150;
    OCR0B = 150;
}

/*
    @det    Methode pour faire arreter les roues
                met les paliers a 255
    @param  void
    @ret    void
*/
void Moteur::arreter()
{
    OCR0A = 255;
    OCR0B = 255;
}

/*
    @det    Destructeur de la classe Moteur, remet les registres utilises a 0
    @param  void
    @ret    void
*/
Moteur::~Moteur()
{
    TCCR0A = 0;
    TCCR0B = 0;
}