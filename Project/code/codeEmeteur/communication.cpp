#include "communication.h"

volatile bool boutonPresse = false;
volatile bool minuterieExpiree = false;
volatile uint8_t nbPressions = 0;
Minuterie minuterie(CTC, div1024, un);

/*
	@det	ISR du boutton poussoir
	@param  void
	@ret    void
*/
ISR(INT0_vect)
{
	_delay_ms(25);
	if (PIND & 0x04 && !boutonPresse) {
		boutonPresse = true;
		nbPressions++;
		minuterie.partirMinuterie(15624);
	}
	else if (!(PIND & 0x04)) {
		boutonPresse = false;
	}
	EIFR |= (1 << INTF0);
}

/*
	@det    Fonction qui permet les interruption avec le bouton poussoir
	@param  void
	@ret    void
*/
void initialisationBouttonPoussoir()
{
	cli();
	EIMSK |= (1 << INT0) ;
	EICRA |= (1 << ISC00) ;
	sei();
} 

/*
	@det	ISR de la minuterie 1 on compare match de OCR1A
	@param  void
	@ret    void
*/
ISR(TIMER1_COMPA_vect)
{
	minuterieExpiree = true;
	PORTB = ~0x01;
	_delay_ms(500);
	PORTB = 0x0;
	_delay_ms(500);
}

/*
	@det    Constructeur d'une commmunication, soit en mode émission soit en mode réception
	@param  ER type : type de la communication, emission ou reception (voir type enum ER dans communication.h)
	@ret    void
*/
Communication::Communication(const ER& type)
{
	adresse_[0] = 1;
	adresse_[1] = 0; 
	adresse_[2] = 0;
	adresse_[3] = 0;
	adresse_[4] = 0;
	for (uint8_t i = 0; i < 7; i++)
		commande_[i] = 0;
	for (uint8_t i = 0; i < 12; i++)
		reception_[i] = 0;
	type_ = type;
	initialisationBouttonPoussoir();
	DDRB = 0xff;
}

/*
	@det    Methode utilisee pour des fins de tests
	@param  uint8 commande a emmeter
	@ret    void
*/
void Communication::setCommande(uint8_t nbPressionTest)
{
	for(uint8_t i = 0; nbPressionTest > 0; i++)
	{
		commande_[i] = nbPressionTest%2;
		nbPressionTest = nbPressionTest/2;
	}
}

/*
	@det   	Méthode qui calcule le nombre d'appui sur le bouton dès son appel et qui
			 	convertit cette valeur en sa représentation binaire dans le tableau attribut commande_[]
	@param  void
	@ret    void
*/
void Communication::getPressionsBouton()
{
	minuterie.partirMinuterie(15624);
	
	// attendre que la minuterie expire pour bien enregistrer les appuis sur le bouton avant de passer à la transmission
	while(!minuterieExpiree) { }
	minuterie.~Minuterie();
	DDRC =0xff;
	//DEBUG--------------------------------	// // affichage du nombre de pressions
	for(uint8_t i = 0; i < nbPressions; i++) {
		PORTC = 0x01;
		_delay_ms(200);
		PORTC = 0x00;
		_delay_ms(200);
	}

	//convertir le nombre nbPressions en binaire
	for(uint8_t i = 0; nbPressions > 0; i++)
	{
		commande_[i] = nbPressions%2;
		nbPressions = nbPressions/2;
	}
}

/*
	@det    Méthode qui émet un 0 selon la norme SIRC
	@param  void
	@ret    void
*/
void Communication::emettre_0() const
{
	for(uint8_t j = 0; j < 23; j++) { // PWM à 38 kHz
		PORTB = 0x01;
		_delay_us(8);
		PORTB = 0x00;
		_delay_us(18);
	}
}

/*
	@det    Méthode qui émet un 1 selon la norme SIRC
	@param  void
	@ret    void
*/
void Communication::emettre_1() const
{
	for(uint8_t j = 0; j < 46; j++) { // PWM à 38 kHz
		PORTB = 0x01;
		_delay_us(8);
		PORTB = 0x00;
		_delay_us(18);
	}
}

/*
	@det    Méthode qui émet la séquence d'initialisation selon la norme SIRC : 2400ms de 1, puis 
	@param  void
	@ret    void
*/
void Communication::sequenceInit() const
{
	for(uint8_t j = 0; j < 92; j++){ // PWM à 38 kHz
		PORTB = 0x01;
		_delay_us(8);
		PORTB = 0x00;
		_delay_us(18);
	}
	PORTB = 0x00;
	_delay_us(600);
}

/*
	@det    Méthode qui émet le tableau attribut commande_[] selon la norme SIRC
	@param  void
	@ret    void
*/
void Communication::emettreCommande() 
{
	for (uint8_t i = 0; i < 7; i++)
	{
		if (commande_[i] == 0)
			emettre_0();
		else if (commande_[i] == 1)
			emettre_1();
		PORTB = 0x00;
		_delay_us(600);
	}
}

/*
	@det    Méthode qui émet le tableau attribut adresse_[] selon la norme SIRC
	@param  void
	@ret    void
*/
void Communication::emettreAdresse()
{
	for (uint8_t i = 0; i < 5; i++)
	{
		if (adresse_[i] == 0)
			emettre_0();
		else if (adresse_[i] == 1)
			emettre_1();
		PORTB = 0x00;
		_delay_us(600);
	}
}

/*
	@det    Méthode à appeler pour émettre le signal voul, à savoir le nombre de pressions sur le bouton
	@param  void
	@ret    void
*/
void Communication::emettreSignal()
{
	sequenceInit();
	emettreCommande();
	emettreAdresse();
	PORTB = 0x00;
	_delay_ms(25); // norme SIRC
}

void resetVaribles()
{
	boutonPresse = false;
	minuterieExpiree = false;
}