#include "buzzer.h"

uint8_t lesDurees_[37] = { 142, 134, 126, 119, 112, 106, 100, 94, 89, 
			84, 79, 75, 71, 67, 63, 59, 56, 53, 50, 47, 44, 42, 39, 37, 35, 33, 31, 
			29, 28, 26, 25, 23, 22, 21, 19, 18, 17 };
/*
	@det    Constructeur par default de la classe qui initialise les _delay_ms(i)buts
	@param  void
	@ret    void
*/
Buzzer::Buzzer()
{
	DDRD = 0xff; //on met le PORTD 8 en sortie
}

/*
	@det    Destructeur de la classe Buzzer
	@param  void
	@ret    void
*/
Buzzer::~Buzzer()
{
	arreterBuzzer();
}

/*
	@det    Methode qui fait emet un son sur une frequence precise
	@param  uint16  frequence de son a emmetre
	@ret    void
*/
void Buzzer::faireBuzzer(const uint16_t& frequence) const
{
	uint16_t valeur = lesDurees_[frequence-45];
	TCNT1 = 0;
	TCCR1A |= (1 << COM1B0) | (1 << WGM12) | (1 << COM1A0) | (0 << COM1B1) | (0 << COM1A1);
	TCCR1B |= (1 << CS12) | (1 << WGM12);
	TCCR1C = 0;
	OCR1A = valeur;
	OCR1B = valeur;
}

/*
	@det    Methode pour arreter le buzzer
	@param  void
	@ret    void
*/
void Buzzer::arreterBuzzer()
{
	TCNT1 = 0;
	TCCR1A = 0;
	TCCR1B = 0;
}

/*
	@det Fonction qui joue le thème de Final Fantasy pour terminer la tâche
	@param  void
	@ret    void
*/
void Buzzer::buzzerFF()
{
	faireBuzzer(57);
	_delay_ms(31);
	arreterBuzzer();
	_delay_ms(130);

	faireBuzzer(57);
	_delay_ms(31);
	arreterBuzzer();
	_delay_ms(130);

	faireBuzzer(57);
	_delay_ms(31);
	arreterBuzzer();
	_delay_ms(130);

	faireBuzzer(57);
	_delay_ms(420);
	arreterBuzzer();
	_delay_ms(20);

	faireBuzzer(49);
	_delay_ms(420);
	arreterBuzzer();
	_delay_ms(20);

	faireBuzzer(54);
	_delay_ms(420);
	arreterBuzzer();
	_delay_ms(20);

	faireBuzzer(57);
	_delay_ms(114);
	arreterBuzzer();
	_delay_ms(177);

	faireBuzzer(54);
	_delay_ms(130);
	arreterBuzzer();
	_delay_ms(20);

	faireBuzzer(57);
	_delay_ms(1340);
	arreterBuzzer();
}

/*
	Fonction qui joue le thème de Megalovania avec le buzzer
	@param  void
	@ret    void
*/
void Buzzer::buzzerMegalo()
{
	_delay_ms(5); //attente 125 ms
	faireBuzzer(50) ;// émettre un son à 146.83 Hz (D)
	_delay_ms(3); //attente de 75 ms
	arreterBuzzer();
	_delay_ms(2); //attente de 50 ms
	faireBuzzer(50) ;// émettre un son à 146.83 Hz (D)
	_delay_ms(3);
	arreterBuzzer();
	_delay_ms(2);
	faireBuzzer(62) ;// émettre un son à 293.66 Hz (D octet higher)
	_delay_ms(5);
	arreterBuzzer();
	_delay_ms(5);
	faireBuzzer(57) ;// émettre un son à 220 Hz (A)
	_delay_ms(5);
	arreterBuzzer();
	_delay_ms(5);
	faireBuzzer(56) ;// émettre un son à 207.65 Hz (G sharp)
	_delay_ms(8);
	arreterBuzzer();
	_delay_ms(7);
	faireBuzzer(56) ;// émettre un son à 207.65 Hz (G sharp)
	_delay_ms(5);
	arreterBuzzer();
	_delay_ms(4);
	faireBuzzer(55) ;// émettre un son à 196 Hz (G)
	_delay_ms(5);
	arreterBuzzer();
	_delay_ms(5);
	faireBuzzer(53) ;// émettre un son à 174.61 Hz (F)
	_delay_ms(3);
	arreterBuzzer();
	_delay_ms(2);
	faireBuzzer(50) ; // émettre un son à 146.83 Hz (D)
	_delay_ms(3);
	arreterBuzzer();
	_delay_ms(2);
	faireBuzzer(53) ;// émettre un son à 174.61 Hz (F)
	_delay_ms(3);
	arreterBuzzer();
	_delay_ms(2);
	faireBuzzer(55) ; // émettre un son à 196 Hz (G)
	_delay_ms(3);
	arreterBuzzer();
}
/*
	@det Fonction qui joue toutes les tonalités de 45 à 81 à jouer lors de la séquence de démarrage (par exemple quand on lis dbt)
	@param  void
	@ret    void
*/
void Buzzer::buzzerInit()
{
	for (uint8_t i = 45; i < 82; i+= 2) {
		faireBuzzer(i);
		_delay_ms(40);
	}
	arreterBuzzer();
}

/*
	@det Fonction qui joue deux sons aigus pour indiquer l'entrée ou la sortie d'un rectangle
	@param  void
	@ret    void
*/
void Buzzer::buzzerRectangle()
{
	faireBuzzer(80);
	_delay_ms(50);
	arreterBuzzer();
	_delay_ms(20);
	faireBuzzer(80);
	_delay_ms(50);
	arreterBuzzer();
}

/*
	@det Fonction qui joue un son aigu de deux seconde pour indiquer la fin du parcours
	@param  void
	@ret    void
*/
void Buzzer::buzzerFini()
{
	faireBuzzer(70);
	_delay_ms(2000);
	arreterBuzzer();
}

/*
	@det Fonction qui joue un son grave de trois seconde pour indiquer une rotation de 9 deg
	@param  void
	@ret    void
*/
void Buzzer::buzzerRotation()
{
	faireBuzzer(50);
	_delay_ms(3000);
	arreterBuzzer();
}
