/*
EN-TETE DE PROGRAMME
Nom : code test du TP7 pour verifier le bon fonctionnement de la librairie 
Auteurs : Theo Quiquempoix 1882798, 
          Philippe Babin 1884183,
          Joe Abdo 1939689,
          Mathurin Chritin 1883619.
Derniere modification : 21 fevrier 2019
*/

#include <libCommune.h>
#define ROUGE 0x01
#define VERT 0x02
#define BLANC 0x00

volatile uint8_t minuterieExpiree = 0;
volatile uint8_t compteur = 0;

void testLED()
{
    LED laLED(B, ledRouge, 0,1);
    laLED.activer();

    _delay_ms(1000);
    laLED.eteindre();

    // _delay_ms(1000);
    // for(;;)
    //     laLED.ambree();

    _delay_ms(1000);
    laLED.activer();
    _delay_ms(1000);
    laLED.setEtat(ledVerte);
    _delay_ms(1000);
    laLED.eteindre();
}

ISR (TIMER0_COMPA_vect) 
{
    compteur++;

    if (compteur == 100)
        minuterieExpiree = 1;
}

void testMinuterie()
{
    Minuterie minuterieTest(CTC, div1024, zero);
    
    uint16_t duree = 200;
    minuterieTest.partirMinuterie(duree);

    while (minuterieExpiree == 0) {}
    minuterieTest.arreterInterruption();
    testLED();
}

void testPWMoteur()
{
    Moteur unmoteur(phaseCorrect, by8);
    while(true)
    {
        unmoteur.avancer(100,100);
        _delay_ms(1000);
        unmoteur.droite(avant);
        _delay_ms(250);
        unmoteur.droite(arriere);
        _delay_ms(250);

        unmoteur.gauche(avant);
        _delay_ms(250);
        unmoteur.gauche(arriere);
        _delay_ms(250);
        unmoteur.reculer(100,100);
        _delay_ms(1000);
    }
}

int main() 
{
    testLED();
    testMinuterie();
    //testPWMoteur();

    /*Moteur unmoteur(phaseCorrect, by8);
    while(true)
        unmoteur.avancer(0,0);*/
}