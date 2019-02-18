/* Programme simple pour essaier d'ecrire et de lire de la memoire externe
	Si on ecrit et on lit la meme sequence alors la LED va s'allumer en vert
	Sinon elle s'allume en rouge */


#define F_CPU 8000000//UL // 8 MHz -- Define toujours au debut

#include <avr/io.h> 
#include "memoire_24.h"


int main()
{
	DDRB = 0xff; // B en mode ecriture sur la LED
	
	Memoire24CXXX memoireExterne; // Appelle du constructeur de memoire pour initialiser
	
	unsigned char aEcrire[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
	uint8_t size = 0;
	while(aEcrire[size] != '\0')
		size++;
	memoireExterne.ecriture(0x0, &aEcrire[0], size);
		// le size du string est aussi egale a sizeof(aEcrire)-1 (-1 car en C la chaine se termine par \0 qui est le charactere de fin de chaine)
	
	bool equal = false;
	unsigned char aLire[] = {0};
	memoireExterne.lecture(0x0, &aLire[0], size);
	
	for(uint8_t i = 0; i < size; i++)
	{
		if(aLire[i] != aEcrire[i])
		{
			equal = false;
			break;
		}
		if(size-1 == i && aLire[i] == aEcrire[i])
		{
			equal = true;
			if(equal)
				PORTB = 0x2;
			else
				PORTB = 0x0;
		}
	}
}

