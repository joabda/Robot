/*
 * Code permettant d'ecrire sur la mémoire externe
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
#include "libCommune.h"

uint16_t getSizeFichier(Uart& uart)
{
    uint16_t octet16 = ( uart.reception()  << 8) | uart.reception();
    return octet16;                 // Concatenantion des deux octet pour trouver la taille du fichiers transmit
}

void test(Memoire24CXXX memoire1, uint8_t adresse, Uart& uart)
// Avec serieViaUSB -l va recevoir le contenu de la memoire qui devrait etre le meme que ce qui a ete transmis
{
    unsigned char donee[] = {0};
    for(uint8_t i = 0; i <= adresse; i++)
    {
        memoire1.lecture(i, &donee[0],1);
        uart.transmission(&donee[0]);
    }
}

int main()
{
    Uart uart1;
    LED led1(A,ledRouge,0,1);

    Memoire24CXXX memoire1; 
    uint16_t sizeFichier = getSizeFichier(uart1);
    if(sizeFichier == 106)
        led1.activer();
    uint16_t adresse = 0;
    for (; adresse != sizeFichier - 2; adresse++)  // -2 car on lit les 2 premiers octets qui donnent la taille (on ne veut pas les ecrire )
    {
        unsigned char doneeRecue[] = {uart1.reception()};
        memoire1.ecriture(adresse, &doneeRecue[0], 1);
    }
    test(memoire1, adresse, uart1); //Decommenter cette ligne pour tester le code
}