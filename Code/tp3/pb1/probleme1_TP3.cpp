/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */


#define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include <util/delay.h>

bool debounce()
// Fonction qui debounce le boutton pressoire
// Cette fonction retourne true si le boutton a ete presse SI il n'est pas presse elle retourne false
{
	if(PIND & 0x04)
	{
		_delay_ms(10);
		if(PIND & 0x04)
			return true;
		else
			return false;	
	}
	else
		return false;
}

void LED(const double duree, const uint8_t& etat)
// Cette fonction place la LED a un etat pour une duree*8 us
{
	PORTB = etat;
	for(int i = 0; i < duree ; i++)
		_delay_us(8);
}

void init()
{
	DDRB = 0xff; // PORT B est en mode sortie
	DDRD = 0x00; // PORT D est en mode lecture	
}

int main()
{
	init();
	const uint8_t rouge = 0x1; 
	const uint8_t eteint = 0x0; 
	
	bool allumee = false;
	int periode = 100;
	int a =0;

	for(;;)  // boucle sans fin
		if(debounce())
		{
			if(allumee == false)
			{
				for(uint8_t i = 0; i < 50; i++)
					for(uint8_t j = 0; j < 30; j++)
					{
						LED(i ,rouge);
						LED((periode-i), eteint);
						// Les deux fonctions LED durent ensembles 1ms car 1/b est de 1kHz donc b=10^(-3)s 
					}
				PORTB = rouge;
				allumee = true;
			}
			else
			{
				for(uint8_t i = 50; i > 0; i--)
					for(uint8_t j = 0; j < 30; j++)
					{
						LED(i ,rouge);
						LED((periode-i), eteint);
						// Les deux fonctions LED durent ensembles 1ms car 1/b est de 1kHz donc b=10^(-3)s 
					}
				allumee = false;
			}
		}	
	return 0; 
}
