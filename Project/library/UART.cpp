#include "UART.h"

/*
    @det    Constructeur par default de la classe qui initialise les attributs=
                pour une transmission/reception a 8bits a 2400 bauds
    @param  void
    @ret    void
*/
Uart::Uart()
{
    UBRR0H = 0;
    UBRR0L = 0xCF;
    UCSR0A = (1 << RXC0 | 1 << TXC0) ;
    UCSR0B = (1 << TXEN0 | 1 << RXEN0) ;
    UCSR0C = (1 << UCSZ01 | 1 << UCSZ00) ;
}

/*
    @det    Methode de tranmission de dnonee
    @param  Donne a transmettre
    @ret    void
*/
void Uart::transmission(unsigned char donnee[])
{
    setMode(send);
    while ( !( UCSR0A & (1 << UDRE0)) );
    UDR0 = donnee[0];
}

/*
    @det    Methode de reception de donnee
    @param  void
    @ret    Donee recue
*/
uint8_t Uart::reception()
{
    setMode(receive);
    while ( !(UCSR0A & (1<<RXC0)) );
    return UDR0;
}

/*
    @det    Methode setter qui indique si on transmet ou on recoit
    @param  void
    @ret    void
*/
void Uart::setMode(const ModeUart& mode)
{
    mode_ = mode;
}

/*
    @det    Methode getter qui indique si on transmet ou on recoit
    @param  void
    @ret    void
*/
ModeUart Uart::getMode()
{
    return mode_;
}