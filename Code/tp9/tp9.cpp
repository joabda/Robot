#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h> 
#include <util/delay.h>
#include <stdio.h>
#include <libCommune.h>


/*
    @det    Fonction qui lit l'instruction du code mneumonique compile (en binaire)
    @param  Memoire24CXXX	Memoire de laquelle lire les bits
	@param	uint8 			Adresse a partir de laquelle lire	 
    @ret    uint8			Donnees lues 
*/
uint8_t lireInstruction(Memoire24CXXX& memoireExterne, uint8_t& adresse)
{
	uint8_t* instruction  = 0; // Preferablement nullptr mais ca donne une erreur (c'est une ancienne version de C++)
	//adresse += 8;
	memoireExterne.lecture(adresse, instruction);
	_delay_ms(5);
	adresse++;
	return *instruction;
}

/*
    @det    Fonction qui lit l'operande du code mneumonique compile (en binaire)
    @param  Memoire24CXXX	Memoire de laquelle lire les bits
	@param	uint8 			Adresse a partir de laquelle lire	 
    @ret    uint8			Donnees lues 
*/
uint8_t lireOperande(Memoire24CXXX& memoireExterne, uint8_t& adresse)
{
	uint8_t* operande = 0;	// Preferablement nullptr mais ca donne une erreur (c'est une ancienne version de C++)
	memoireExterne.lecture(adresse, operande);
	_delay_ms(5);
	adresse++;
	return *operande;
}

/*
    @det    Fonction qui attend un nombre de ms (Block le circuit)
    @param  uint8	nombre de fois a attendre 25ms 
    @ret    void
*/
void ATT(const uint8_t operande)
{
	for (uint8_t i = 0; i < operande; i++)
		_delay_ms(25);
}

int main()
{
	enum Instruction {dbt = 0x01, att = 0x02, dal = 0x44, det = 0x45, sgo = 0x48, sar = 0x09, mar1 = 0x60, mar2 = 0x61, mav = 0x62, mre = 0x63, trd = 0x64, trg = 0x65, dbc = 0xc0, fbc = 0xc1, fin = 0xff};
	Memoire24CXXX memoireExterne;
	uint8_t donnee = 0, operande = 0, adresse = 0, debutBool = 0;
	uint8_t adresseDebutBoucle = 0, compteurBoucle = 0, maxBoucle = 0;
	DDRA = 0xff;
	DDRB = 0xff;

	donnee  = lireInstruction(memoireExterne, adresse);

	Uart debug;
	
	Moteur moteur;
	Buzzer unBuzzer;
	while(donnee != 0xff) {
		operande = lireOperande(memoireExterne, adresse);
		debug.transmission(&donnee);
		if(donnee == 0x01)
		{
			debutBool = 1;

			// Mélodie de début de parcours
			unBuzzer.buzzerInit();
		}
		if (debutBool == 1) {
			switch (donnee) {
				case att:
				{
					ATT(operande);
					moteur.arreter();
				}
					break;
				
				case dal:
					PORTA = operande;
					break; 
				
				case det: 
					PORTA = 0;
					break;

				case sgo: 
					unBuzzer.faireBuzzer(operande);				
					break;
				
				case sar: 
					unBuzzer.arreterBuzzer();  
					break;
				
				case mar1 : 
					moteur.arreter();
					break;

				case mar2 : 
					moteur.arreter();
					break;

				case mav:           
					moteur.avancer(255-operande, 255-operande); // 255-operande car dans notre classe a 255 le robot est imobile et a 0 il avance a 100%
				 	break;

				case mre:           
					moteur.reculer(255-operande, 255-operande);
					break;
				
				case trd:           
					moteur.droite90();
					break;
				
				case trg:           
					moteur.gauche90();
					break;
				
				case dbc:
					adresseDebutBoucle = adresse; 
					compteurBoucle = 0; 
					maxBoucle = operande;
					break;
				
				case fbc:
					if (compteurBoucle < maxBoucle) 
					{
						compteurBoucle++;
						adresse = adresseDebutBoucle;
					}
					break;
				
				case fin:
					break;
			}
		}
		donnee = lireInstruction(memoireExterne, adresse);
	}
	// Mélodie de fin de parcours
	unBuzzer.buzzerFF(); 
}