#include "minuterie.h"


/*
    @det    Constructeur de la classe qui initialise les attributs aux parametress, par default
                initialisation a une minuterie 1 PWM a horloge divisee par 1024 
    @param  Mode de minuterie, division horloge et numero de la minuterie 
    @ret    void
*/
Minuterie::Minuterie(Mode mode, Clock clock, NumeroTimer minuterie)
{
	minuterie_ = minuterie;
	setRegistres();

	mode_ = mode;
	setMode();

	clock_ = clock;
	setClock();

	estPartie_ = false;
	peutInterupt_ = false;
}

/*
    @det    Destructeur de la classe qui remet les registres a 0
    @param  void
    @ret    void
*/
Minuterie::~Minuterie()
{
	arreterInterruption();
	arreterMinuterie();
	*(registres_[0]) &= ~( (1 << COM1A1) | (1 << COM0A0) ); 
	*(registres_[1]) &= ~( (1 << CS12) | (1 << CS11) | (1 << CS10) );
}

/*
    @det    Methode qui en fonction de l'attribut mode configure le mode de la minuterie
    @param  void
    @ret    void
*/
void Minuterie::setMode()
{
	switch (mode_)
	{
		case PWM:
			*(registres_[0]) = (1 << WGM00);
			break;

		case CTC:
		{
			if (getNumeroMinuterie() == un) 
				*(registres_[1]) = (1 << WGM12);
			else 
				*(registres_[0]) = (1 << WGM01);
		}
	}
}

/*
    @det    Methode qui en fonction de l'attribut clock configure la division horloge
    @param  void
    @ret    void
*/
void Minuterie::setClock()
{
	switch (clock_) 
	{
		case div1:
			*(registres_[1]) |= (1 << CS00);
			break;

		case div8:
			*(registres_[1]) |= (1 << CS01);
			break;

		case div64:
			if (minuterie_ == deux)
				*(registres_[1]) |= (1 << CS22);
			else 
				*(registres_[1]) |= (1 << CS00) | (1 << CS01);
			break;

		case div256:
			*(registres_[1]) |= (1 << CS02);
			if (minuterie_ == deux)
				*(registres_[1]) |= (1 << CS21);
			break;

		case div1024:
			*(registres_[1]) |= (1 << CS02) | (1 << CS00);
			if (minuterie_ == deux)
				*(registres_[1]) |= (1 << CS21);
			break;
		}
}

/*
    @det    Methode qui en fonction de du numero de la minuterie 
				configure les pointeurs vers les registres
    @param  void
    @ret    void
*/
void Minuterie::setRegistres()
{
	switch (minuterie_)
	{
		case zero:
		{
			registres_[0] = &TCCR0A;
			registres_[1] = &TCCR0B; 
			registres_[2] = &TCNT0;
			registres_[3] = &OCR0A;
			registres_[4] = &TIMSK0;
		}
			break;
		
		case un:
		{
			registres_[0] = &TCCR1A;
			registres_[1] = &TCCR1B; 
			registresTimer16bits_[0] = &TCNT1;
			registresTimer16bits_[1] = &OCR1A;
			registres_[4] = &TIMSK1;
		}
			break;
		
		case deux:
		{
			registres_[0] = &TCCR2A;
			registres_[1] = &TCCR2B; 
			registres_[2] = &TCNT2;
			registres_[3] = &OCR2A;
			registres_[4] = &TIMSK2;
		}
			break;
	}
}

/*
    @det    Methode qui part la minuterie pour une duree 
    @param  Duree de la minuterie
    @ret    void
*/
void Minuterie::partirMinuterie(uint16_t duree)
{
	activerInterruption();
	estPartie_ = true;
	*(registres_[0]) |= (1 << COM1A1) | (1 << COM1A0); // On met le comparateur a on compare match

	if(minuterie_ == un)
	{
		*(registresTimer16bits_[0]) = 0;
		*(registresTimer16bits_[1]) = duree;
		*(registresTimer16bits_[2]) = 0;
	}
	else
	{
		*(registres_[2]) = 0;
		*(registres_[3]) = (duree); 
	}	
}

/*
    @det    Methode remet les registres a 0 quand on arrete la minuterie
    @param  void
    @ret    void
*/
void Minuterie::arreterMinuterie()
{
	*(registres_[0]) &= ~( (1 << COM1A1) | (1 << COM0A0) ); 
	*(registres_[1]) &= ~( (1 << CS12) | (1 << CS11) | (1 << CS10) );
	// On fait des masks sur tout les registres possibles pour 
		// les differentes minuteries et on met les bits responsables seulement a 0  
}

/*
    @det    Methode qui permet les interruption sur la minuterie
    @param  void
    @ret    void
*/
void Minuterie::activerInterruption()
{
	peutInterupt_ = true;
	// on peut mettre nimporte quel registre (a la palce de 0) il faut juste mettre 1 au LSD de Timsk
	*(registres_[4]) |= (1 << OCIE1A ); 
	sei();	 
}

/*
    @det    Methode qui arrete les interruptions
    @param  void
    @ret    void
*/
void Minuterie::arreterInterruption()
{
	peutInterupt_ = false;
	*(registres_[4]) &= ~ (1 << 0 ); // On met 1 au LSB de TIMSK sans changer les autres bits
	cli();
}

/*
    @det    Methode getter qui dit si la minuterie a ete partie
    @param  void
    @ret    bool en fonction de l'etat de la minuterie
*/
bool Minuterie::getMinuteriePartie() const
{
	return estPartie_;
}

/*
    @det    Methode getter qui donne le mode de la minuteri
    @param  void
    @ret    Mode de la minuterie
*/
Mode Minuterie::getMode() const
{
	return mode_;
}

/*
    @det    Methode getter qui donne la division horloge
    @param  void
    @ret    Clock de ela division horloge
*/
Clock Minuterie::getClock() const
{
	return clock_;
}

/*
    @det    Methode getter qui donne la minuterie utilisee
    @param  void
    @ret    NumeroTimer qui donne le numero de la minuterie
*/
NumeroTimer Minuterie::getNumeroMinuterie() const
{
	return minuterie_;
}

/*
    @det    Methode getter qui dit si on peut interrompre
    @param  void
    @ret    bool en fonction de l'etat des interruptions sur la minuterie
*/
bool Minuterie::getPeutInterrupt() const
{
	return peutInterupt_;
}