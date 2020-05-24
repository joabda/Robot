/*
 * Classe permettant differents reglages des minuteries 0, 1 et 2
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

#ifndef MINUTERIE_H
#define MINUTERIE_H
#include <avr/io.h>
#include <avr/interrupt.h>


enum Mode {CTC, PWM};
enum Clock {div1, div8, div64, div256, div1024};
enum NumeroTimer {zero, un, deux};

class Minuterie {
    public:
        Minuterie(Mode mode = PWM, Clock clock = div1024, NumeroTimer minuterie = un);
        ~Minuterie();

        void partirMinuterie(uint16_t duree);
        void arreterMinuterie();

        void activerInterruption();
        void arreterInterruption();
        
		//getter
		bool getMinuteriePartie() const;
        Mode getMode() const;
        Clock getClock() const;
        NumeroTimer getNumeroMinuterie() const;
        bool getPeutInterrupt() const;

    private:
        bool estPartie_;
       	Mode mode_;
        Clock clock_;
        NumeroTimer minuterie_;
        bool peutInterupt_;
        volatile uint8_t* registres_[5];
        // Cette array va contenir des pointeurs vers les registres TCCRnA, TCCRnB, TCNTn, OCRnA, TIMSKn par ordre!
        // On met volatile car les registres sont de type volatile uint8_t (si on ne met pas on ne peut pas faire l'affectation)
/*
        registres:
            0: TCCRnA
            1: TCCRnB
            2: TCNTn
            3: OCRnA
            4: TIMSKn
*/
        volatile uint16_t* registresTimer16bits_[3]; // ICI on met uniquement TCNT1, OCR1A et TCCR1C de timer 1 car ils sont a 16 bits
        void setRegistres();
        void setClock();
        void setMode();
};
#endif