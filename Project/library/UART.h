/*
 * Classe permettant differents reglages des transmission et receptions UART
 * du microcontroleur ATMega16 de Atmel.
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

#ifndef UART_H
#define UART_H
#include <avr/io.h>

enum ModeUart{send = 0, receive = 1};

class Uart
{
    public:
        Uart();
        void transmission(unsigned char donnee[]);
        uint8_t reception();

        void setMode(const ModeUart& mode);
        ModeUart getMode();

    private:
        ModeUart mode_;
};
#endif