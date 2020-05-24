/*
 * Code du projet final pour
 *  la realisation de differents parcours.
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Theo Quiquempoix, Philippe Babin, Joe Abdo et Mathurin Chritin
 * 2019
 *
 * Code qui n'est sous aucune license.
 *
 */

#include <libCommune.h>

    /**************** Initialisation *****************/
    /************ des variables globale **************/
    /*************************************************/
enum Sections{sct1, sct2, sct3, sct4, notSet};
#define LED_1 64
#define LED_12 96
#define LED_123 112
#define LED_1234 120
#define LED_12345 124
#define LED_2 32
#define LED_23 48
#define LED_234 56
#define LED_2345 60 
#define LED_3 16
#define LED_34 24
#define LED_345 28
#define LED_4 8
#define LED_45 12
#define LED_5 4
volatile uint16_t compteurMinuterie2 = 0;
volatile bool boutonPoussoir = 0;
volatile bool backupIR = 0;
volatile uint8_t nbPression = 0;
Moteur moteurRobot;
Buzzer buzzerRobot;
CapteurLigne capteurLignes;
Minuterie minuterie2(CTC, div1024, deux);

/*
	@det    ISR du boutton poussoir qui debounce le boutton poussoir
	@param  void
	@ret    void      
*/
ISR( INT0_vect ) 
{
    // anti-rebond
    _delay_ms(25);
    if ( PIND & 0x04 && !boutonPoussoir) {
        boutonPoussoir = true;
		if (backupIR) {
			nbPression++;
			if (nbPression == 10)
				nbPression = 1;
			compteurMinuterie2 = 0;
			minuterie2.partirMinuterie(252);
		}
	}
	else if (!(PIND & 0x04)) {
		boutonPoussoir = false;
	}
}

/*
	@det    ISR de la minuterie 2, increment compteurMinuterie2
                a chaque foi qu'un cycle de la minuterie est fini
	@param  void
	@ret    void      
*/
ISR(TIMER2_COMPA_vect)
{
	compteurMinuterie2++;
}

/*
	@det    Fonction qui permet de recevoir et interpreter un signal IR
	@param  void
	@ret    uint16 c'est la commande sur 7 bits recue      
*/
uint16_t getSignal()
{
	DDRA = 0x00; // Initialisation du portA en entree
 	uint8_t dureeBitRecu = 0; 
	uint8_t nbBitsRecus = 0;
	uint8_t duree = 0;
 	uint8_t temp[12] = {0};
	bool sequenceInit = false;

	while(!sequenceInit && !boutonPoussoir) 
	{
		while( (PINA & 0x01) == 0 && !boutonPoussoir) // Tant que le port A a 0 (CAD capte 1)
		{
			duree++;
			_delay_us(100);
		}	
		// Quand le portA change d'etat
		if(duree > 13) // Si il est reste a 0 plus de 1.3ms c'etait le signal d'initialisation
		{
			sequenceInit = true;
			while((PINA & 0x01) == 0x01 && !boutonPoussoir); // sert à se réaligner avec le prochain 1
		}
		else  // Sinon ce n'etait pas le signal d'initialisation alors on attend a nouveau
		{
			duree = 0;			
			while((PINA & 0x01) == 0x01 && !boutonPoussoir); // sert à se réaligner avec le prochain 1
		}
	}// Quand on sort de cette loop alors le signal d'initialisation a ete recu et traite

 	while(nbBitsRecus < 12 && !boutonPoussoir) 
 	{		
 		/*
		PORTA == 0x00 -> (PORTA & 0x01) == 0
		PORTA == 0x01 -> (PORTA & 0x01) == 1
 		*/
		while((PINA & 0x01) == 0x00 && !boutonPoussoir) 
		{
			_delay_us(100);
			dureeBitRecu++;
		}
		temp[nbBitsRecus] = dureeBitRecu;
		dureeBitRecu = 0;
		nbBitsRecus++;
		while((PINA & 0x01) == 0x01 && !boutonPoussoir); // sert à se réaligner avec le prochain 1
 	}

	if (boutonPoussoir)
		return 0;

	// Interpretation des donnees
	uint16_t nombre = 0;
	for(uint8_t i = 0; i < 12; i++)
	{
		if(temp[i] >= 11) // Si on a captee 1 pour plus de 1.1ms donc c'est un 1 logique
			nombre |= 1 << i;
		else if(temp[i] > 5) // Implique < 11 car dans le else, si on capte 1 pour plus de  0.5ms c'est donc un 0 logique
			nombre |= 0 << i;
	}
	return (nombre - 128); // -128 car c'est la valeur de l'adresse qu'on transmet qui est toujours la meme
							// dans le contexte de ce proejt
}

/*
	@det    Fonction pour etre sur qu'une ligne blanche est percue
	@param  uint8	valeur des pins du capteur	
	@ret    bool	vrai si une ligne blanche est captee a 20ms d'intervalle, faux sinon
*/
bool debounceLigneBlanche(uint8_t& valeurCapteur)
{
	if((valeurCapteur ^ 124) == 124)
	{
		_delay_ms(20);
		valeurCapteur = capteurLignes.getValeurIR();
		if((valeurCapteur ^ 124) == 124)
			return true;
		else 
			return false;
	}
	else 
		return false;
}

/*
	@det    Cette fonction avance le robot sur une ligne
				avec ajustements si les capteurs de droite ou gauche touchent la ligne
				pour avancer le plus droit possible
	@param  void
	@ret    void      
*/
void avancerAvecAjustement(uint8_t& valeurCapteur)
{                    
	// Rectification a droite
	if ((valeurCapteur & 32) == 32 || (valeurCapteur & 48) == 48) 
		moteurRobot.avancer(130, 100);
	// Rectification a gauche
	else if ((valeurCapteur & 24) == 24 || (valeurCapteur & 8) == 8) 
			moteurRobot.avancer(100, 130);
	else if ((valeurCapteur & 16) == 16)               
			moteurRobot.avancer(100, 110);
		// Rectification a gauche forte
	else if((valeurCapteur & 12) == 12 || (valeurCapteur & 4) == 4)
			moteurRobot.avancer(100,150);
	// Rectification a droite forte
	else if((valeurCapteur & 64) == 64 || (valeurCapteur & 96) == 96 )
			moteurRobot.avancer(150,100);
	valeurCapteur = capteurLignes.getValeurIR();
}

/*
	@det    Cette fonction renvoi si la detection de fin de section a bien etait detectee
				avec des intervalles de 50ms 
	@param  uint8	la valeur du capteur a regarder
	@ret    bool	vrai si la fin de section et detectee faux sinon
*/
bool debounceDetectionFinSection(uint8_t& valeurCapteur)
{
	if((valeurCapteur & 124) == 112)
	{
		_delay_ms(50);
		if( (valeurCapteur & 124) == 112)
			return true;
		else 
			return false;
	}
	else
		return false;
}

/*
	@det    Cette fonction est appelle quand il est temps de finir 
				la section et se replacer sur la prochaine
	@param  uint8	la valeur du capteur au moment d'appel
	@ret    void      
*/
void virageFinSection(uint8_t& valeurCapteur)
{
	while(!debounceDetectionFinSection(valeurCapteur)) 
		avancerAvecAjustement(valeurCapteur);
	moteurRobot.arreter();
	_delay_ms(500);
}

/*
	@det    Cette fonction est appelle quand le robot rencontre une ligne noir
				si cette ligne est appelle elle finit la ligne noir
	@param  void
	@ret    void      
*/
void finirLigneNoir(uint8_t& valeurCapteur)
{
	while( (valeurCapteur & 124) == 124)
	{
		moteurRobot.avancer(120,130);
		valeurCapteur = capteurLignes.getValeurIR();
	}
}

/*
	@det    Cette fonction est appelle quand le robot rencontre un segment
				si cette ligne est appelle elle depasse le segment
	@param  void
	@ret    void      
*/
void finirSegment(uint8_t& valeurCapteur,  const uint8_t& valeurAFinir)
{
	while( (valeurCapteur & valeurAFinir) == valeurAFinir)
	{
		avancerAvecAjustement(valeurCapteur);
		valeurCapteur = capteurLignes.getValeurIR();
	}
}

/*
	@det    Cette fonction est appelle quand le robot doit avancer 
				dans un reectangle sans sortir de ce dernier
	@param  void
	@ret    void      
*/
void avancerDansRectangle(uint8_t& valeurCapteur)
{
	// Rectification a droite
	if ((valeurCapteur & 48) == 48) 
		moteurRobot.avancer(100, 130);
	// Rectification a gauche
	else if ((valeurCapteur & 24) == 24) 
			moteurRobot.avancer(130, 100);
	else if ((valeurCapteur & 16) == 16)                
			moteurRobot.avancer(100, 110);
	// Rectification a gauche forte
	else if((valeurCapteur & 4) == 4)
			moteurRobot.avancer(150,100);
	// Rectification a droite forte
	else if((valeurCapteur & 64) == 64)
			moteurRobot.avancer(100,150);
	else if((valeurCapteur & 96) == 96 || (valeurCapteur & 32) == 32)
			moteurRobot.avancer(100,250);
	else if((valeurCapteur & 8) == 8 || (valeurCapteur & 12) == 12)
			moteurRobot.avancer(250,100);
	valeurCapteur = capteurLignes.getValeurIR();
}


/*
	@det    Cette fonction doit etre appelle a chaque fin de section
				pour que le robot se repositionne
	@param  void
	@ret    void      
*/
void repositionner(uint8_t& valeurCapteur)
{
	moteurRobot.tourneLuiMeme(antiHoraire);
	while( (valeurCapteur & 16) != 16)
	{
		// Si les differentes combinaisons de IR 3 4 5 sont allumees
		if ( (valeurCapteur & 12) == 12 || (valeurCapteur & 8) == 8 ||
			 (valeurCapteur & 24) == 24 || (valeurCapteur & 4) == 4)
			moteurRobot.tourneLuiMeme(horaire);
		// Si les differentes combinaisons de IR 1 2 3 sont allumees
		if ( (valeurCapteur & 64) == 64 || (valeurCapteur & 32) == 32 ||
			 (valeurCapteur & 96) == 96 || (valeurCapteur & 48) == 48) 
			moteurRobot.tourneLuiMeme(antiHoraire);
		valeurCapteur = capteurLignes.getValeurIR();
	}
	moteurRobot.arreter();
}

/* Fonction qui buzz grave 3 secondes avant et apres rotation */
/* Rotation de 90 degres */
void rotationFinSection(uint8_t& valeurCapteur)
{
	moteurRobot.avancer(0,20);
	_delay_ms(500);
	moteurRobot.arreter();
	_delay_ms(1000);
	moteurRobot.gauche90();
	moteurRobot.arreter();
	_delay_ms(300);
	valeurCapteur = capteurLignes.getValeurIR();
	repositionner(valeurCapteur);
}

/*
	@det    Cette fonction sert de backup si le IR ne fonctionne pas
				et indique au robot la section de debut
	@param  void
	@ret    uint8	numero de la section de debut      
*/
uint8_t getNumeroPremiereSection()
{
	uint8_t numeroPremiereSection = 0;

	while (numeroPremiereSection < 1 ||
		   numeroPremiereSection > 4) {

		numeroPremiereSection = 0;
		nbPression = 0;
		PORTC |= 0x01;
		while( numeroPremiereSection == 0 && !boutonPoussoir)
			numeroPremiereSection = getSignal();
		PORTC &= ~(0x01);

		if (boutonPoussoir) {
			// Le 62 correspond a 2 secondes
			compteurMinuterie2 = 0;
			while(compteurMinuterie2 < 62);
			// On ne fait rien jusqu'a une pause de 2 secondes
			numeroPremiereSection = nbPression;
			if (numeroPremiereSection == 0)
				PORTC |= 0x02;
		}
	}
	return numeroPremiereSection;
}

/*
	@det    Fonction qui permet d'executer la section 1
	@param  void
	@ret    void      
*/
void section1()
{ // Fonction pour la section 1 du parcours

	// initialisation des variables de la fonction
	DDRD = 0x00;
	uint16_t numeroDuPoint = 0;
	nbPression = 0;
	uint8_t valeurCapteur = capteurLignes.getValeurIR();
	boutonPoussoir = false;
	backupIR = true;
    /*************************************************/
    /******************* Avancer *********************/
    /************* Arret a la ligne noir *************/
    /*************************************************/

	while ( (valeurCapteur & 124) != 124) 
		avancerAvecAjustement(valeurCapteur);
	
	moteurRobot.arreter();
	_delay_ms(1000);

    /*************************************************/
    /*** Reception (infrarouge) du numero du point ***/
    /*************************************************/     // Possibilite d'utiliser le bouton si infrarouge marche pas
    
    // Reception du signal ou on passe au bouton
    PORTC |= 0x01;
	while(numeroDuPoint == 0 && !boutonPoussoir) 
		numeroDuPoint = getSignal();
	PORTC &= ~(0x01);

	if (boutonPoussoir) {
		// Le 62 correspond a 2 secondes
		compteurMinuterie2 = 0;
		minuterie2.partirMinuterie(255);
		while(compteurMinuterie2 < 62);
		// On ne fait rien jusqu'a une pause de 2 secondes
		numeroDuPoint = nbPression;
		if (numeroDuPoint == 0)
			PORTC |= 0x02;
	}
	
	// Affichage de la nombre de pression pour verifier le bon fonctionnement
	for (uint8_t i = 0; i < numeroDuPoint; i++) {
		PORTC |= 0x02;
		_delay_ms(200);
		PORTC &= ~(0x02);
		_delay_ms(200);
	}

    /*************************************************/
    /******* Deplacement jusqu'au point voulu ********/
    /*************************************************/     // Paralelle au segment S3
    
    // Virage a gauche de 90 degres
	moteurRobot.gauche90();
	_delay_ms(1000);

	/*
	* 	Bouger en fct du message recu
	*/
	moteurRobot.avancer(100, 100);
	valeurCapteur = capteurLignes.getValeurIR();

	switch(numeroDuPoint) {
		// Avancer jusque la colonne 1
		case 1:
		case 4:
		case 7:
			_delay_ms(2500);
			break;
		
		// Avancer jusque la colonne 2
		case 2:
		case 5:
		case 8:
			_delay_ms(2000);
			break;
		
		// Avancer jusque la colonne 3
		case 3:
		case 6:
		case 9:
			_delay_ms(1400);
			break;
	}

	moteurRobot.arreter();
	_delay_ms(1000);
	moteurRobot.droite90();
	_delay_ms(1000);
	moteurRobot.avancer(100, 100);

	switch(numeroDuPoint) {
		// Avancer jusque la ligne 1
		case 1:
		case 2:
		case 3:
			_delay_ms(2600);
			break;

		// Avancer jusque la ligne 2
		case 4:
		case 5:
		case 6:
			_delay_ms(2050);
			break;

		// Avancer jusque la ligne 3
		case 7:
		case 8:
		case 9:
			_delay_ms(1500);
			break;
	}

	moteurRobot.arreter();
	_delay_ms(500);

    /*************************************************/
    /******* Immobile pdt 3s avec un son grave *******/
    /**** Rotation 90 degres droite (sens hotaire) ***/
    /******* Immobile pdt 3s avec un son grave *******/
    /*************************************************/

    // Debounce de la premiere note
	DDRD = 0Xff;
	buzzerRobot.faireBuzzer(50);
	_delay_ms(10);
	buzzerRobot.arreterBuzzer();
	_delay_ms(10);

	// Buzzer, rotation, buzzer
	buzzerRobot.buzzerRotation();
	moteurRobot.droite90();
	buzzerRobot.buzzerRotation();
	DDRD = 0X00;

    /*************************************************/
    /********** Diriger vers la section S3 ***********/
    /*************************************************/

    moteurRobot.avancer(100, 100);

	switch(numeroDuPoint) {
		// Avancer depuis la colonne 1
		case 1:
		case 4:
		case 7:
			_delay_ms(2500);
			break;
		
		// Avancer depuis la colonne 2
		case 2:
		case 5:
		case 8:
			_delay_ms(2000);
			break;
		
		// Avancer depuis la colonne 3
		case 3:
		case 6:
		case 9:
			_delay_ms(1400);
			break;
	}

	moteurRobot.arreter();
	_delay_ms(1000);
	moteurRobot.gauche90();
	_delay_ms(1000);

    /*************************************************/
    /************** Finir le parcours ****************/
    /*********** Virage 90 degres gauche *************/
    /*************************************************/

	uint16_t compteurAjustement = 0;

	for (compteurAjustement = 0; compteurAjustement < 2000; compteurAjustement++)
		moteurRobot.avancer(100, 255);

	while( (valeurCapteur & 124) == 0) {
		if (compteurAjustement < 1000)
			moteurRobot.avancer(255, 100);
		else
			moteurRobot.avancer(100, 255);
		
		valeurCapteur = capteurLignes.getValeurIR();

		if (compteurAjustement == 2000)
			compteurAjustement = 0;

		compteurAjustement++;
	}
	moteurRobot.arreter();
	_delay_ms(100);
	valeurCapteur = capteurLignes.getValeurIR();
	repositionner(valeurCapteur);
	avancerAvecAjustement(valeurCapteur);
    virageFinSection(valeurCapteur);

	/*************************************************/
    /********** Rotation de fin de section ***********/
    /*************************************************/
	rotationFinSection(valeurCapteur);
}

/*
	@det    Fonction qui permet d'executer la section 2
	@param  void
	@ret    void      
*/
void section2()
{
	uint8_t valeurCapteur = capteurLignes.getValeurIR();
	uint8_t vitesse = 20;
	uint8_t nbFoisReajustement = 0;
	uint8_t nbFoisLoop = 0;

	// Avance jusqu'a detection d'un virage a droitie (fin de section)
    while(!debounceDetectionFinSection(valeurCapteur)) 
    {    
		nbFoisLoop++;
        if( (valeurCapteur & 12)    == 12 || 
            (valeurCapteur & 28)    == 28 || 
            (valeurCapteur & 60)    == 60 ||
            (valeurCapteur & 124)   == 124)
			{
				_delay_ms(10);
				moteurRobot.arreter();
				repositionner(valeurCapteur);
			}
		// Reactification a droite
		if ((valeurCapteur & 124) == 32 || (valeurCapteur & 124) == 48) 
		{
			nbFoisReajustement++;
			moteurRobot.avancer(vitesse+50, vitesse);
		}
		// Rectification a gauche
		else if ((valeurCapteur & 124) == 24 || (valeurCapteur & 124) == 8)
		{
			nbFoisReajustement++;
			moteurRobot.avancer(vitesse, vitesse+30);
		}
		// Pas d'ajustements
		else if ((valeurCapteur & 124) == 16)   
		{         
			nbFoisReajustement = 0;
			moteurRobot.avancer(vitesse, vitesse);
		}
		// Rectification a gauche forte
		else if( (valeurCapteur & 124) == 12 || (valeurCapteur & 124) == 4)
		{	
			nbFoisReajustement++;
			moteurRobot.avancer(vitesse,vitesse+50);
		}
		// Rectification a droite forte
		else if((valeurCapteur & 124) == 64 || (valeurCapteur & 124) == 96)
		{
			nbFoisReajustement++;
			moteurRobot.avancer(vitesse+130,vitesse);
		}
		if(nbFoisReajustement > 2 && nbFoisLoop < 10)
			vitesse = 120;
		else if(nbFoisReajustement < 2 && nbFoisLoop < 10)
			vitesse = 20;
		valeurCapteur = capteurLignes.getValeurIR();
	}
	moteurRobot.arreter();

	/*************************************************/
    /********** Rotation de fin de section ***********/
    /*************************************************/
	rotationFinSection(valeurCapteur);
}

/*
	@det    Fonction qui permet d'executer la section 3
	@param  void
	@ret    void      
*/
void section3()
{
    uint8_t valeurCapteur = capteurLignes.getValeurIR();
    /*************************************************/
    /******************* Avancer *********************/
    /*************************************************/

	while( (valeurCapteur & 124) != 124 ) // On arrive a la ligne noir
		avancerAvecAjustement(valeurCapteur);

    /*************************************************/
    /************* Arret a la ligne noir *************/
    /*************************************************/
    moteurRobot.arreter();

    /*************************************************/
    /*Appuie sur le bouton pour commencer le segment */
    /********** Debut de lecture du segment **********/
    /*************************************************/
    boutonPoussoir = false;
	while(!boutonPoussoir) // On attend que la personne appuie sur le boutton
	    valeurCapteur = capteurLignes.getValeurIR();

    /*************************************************/
    /************ Arret a la fin de ligne ************/
    /*************************************************/

    while( (valeurCapteur & 124) != 124 ) // On arrive a la ligne noir
		avancerAvecAjustement(valeurCapteur);
	finirLigneNoir(valeurCapteur);// On finit la ligne noir
	_delay_ms(50);
	valeurCapteur = capteurLignes.getValeurIR();
	
	// On initialise et part la minuterie
	bool premierDroite    = false;
	bool premierGauche    = false;
	uint16_t compteurSeg1 = 0;
	uint16_t compteurSeg2 = 0;
	compteurMinuterie2 	  = 0;
	minuterie2.partirMinuterie(255);
	while( !debounceLigneBlanche(valeurCapteur) )
	{
		while(!premierDroite && !premierGauche) // Tant qu'on a pas passe le premier segment
		{
			avancerAvecAjustement(valeurCapteur);
			if( ((valeurCapteur & 124) == LED_345 || (valeurCapteur & 124) == 12) || (valeurCapteur & 124) == LED_2345) 
			{
				compteurSeg1 = compteurMinuterie2;
				finirSegment(valeurCapteur, 12);
				compteurMinuterie2 = 0;
				premierDroite = true;
			}
			else if(( (valeurCapteur & 124) == 112 || (valeurCapteur & 124) == 96) || (valeurCapteur & 124) == 120)
			{
				compteurSeg1 = compteurMinuterie2;
				finirSegment(valeurCapteur, 96);
				compteurMinuterie2 = 0;
				premierGauche = true;
			}
		}
		avancerAvecAjustement(valeurCapteur);
		// Detection du second segment
		if( ((valeurCapteur & 124) == 28 || (valeurCapteur & 124) == 12))
		{
			compteurSeg2 = compteurMinuterie2;
			compteurMinuterie2 = 0;
			finirSegment(valeurCapteur, 12);
		}
		else if(((valeurCapteur & 124) == 112 || (valeurCapteur & 124) == 96))
		{
			compteurSeg2 = compteurMinuterie2;
			compteurMinuterie2 = 0;
			finirSegment(valeurCapteur, 96);
		}
		valeurCapteur = capteurLignes.getValeurIR();
	}
	moteurRobot.arreter();

    /*************************************************/
    /*********** Affichage de quelle ligne ***********/
    /*************************************************/
    uint16_t ratio = (compteurSeg2*100) / compteurSeg1;	
	if(premierDroite) // Premier a droite
	{
		if(ratio > 180)
			PORTC = 0x10;
		else
			PORTC = 0x8;
	}
	else // Premier a gauche
	{
		if(ratio > 180)
			PORTC = 0x40;
		else
			PORTC = 0x20;
	}

    /*************************************************/
    /******** Attente du bouton pour repartir ********/
    /*************************************************/

	boutonPoussoir = false;
	while(!boutonPoussoir);
	    valeurCapteur = capteurLignes.getValeurIR();

    /*************************************************/
    /************** Finir le parcours ****************/
    /*************************************************/
	// Avance jusqua detection de segment a gauche
    virageFinSection(valeurCapteur);


	/*************************************************/
    /********** Rotation de fin de section ***********/
    /*************************************************/
	rotationFinSection(valeurCapteur);
	boutonPoussoir = false;
}

/*
	@det    Fonction qui permet d'executer la section 4
	@param  void
	@ret    void      
*/
static void section4()
{
    uint8_t valeurCapteur = capteurLignes.getValeurIR();
	DDRD = 0xff;
    /******************* Avancer *********************/
    for(uint8_t nbBoucle = 0; nbBoucle < 6 ; nbBoucle ++) {
        // Pas de default car un modulo 2 ne peut donner que 0 ou 1
		switch (nbBoucle % 2) 
		{
			case 0:
				// On sort de la boucle a la detection de ligne noir
                while ((valeurCapteur & 124) != 124) 
					avancerAvecAjustement(valeurCapteur);
				break;
			case 1:
			{
                /************* ENTRER DANS RECTANGLE *************/
                /*** 2 sons aigus de 50ms avec silence de 20ms ***/
                finirLigneNoir(valeurCapteur);
				moteurRobot.arreter();
				buzzerRobot.buzzerRectangle();
                // On sort de la boucle a la detection de ligne noir
				while ((valeurCapteur & 124) != 124)
					avancerDansRectangle(valeurCapteur);
				/************** SORTIR DE RECTANGLE **************/
                /*** 2 sons aigus de 50ms avec silence de 20ms ***/
                finirLigneNoir(valeurCapteur);
				moteurRobot.arreter();
				_delay_ms(100);
				repositionner(valeurCapteur);
				buzzerRobot.buzzerRectangle();
				repositionner(valeurCapteur);
			}
			break;
		} 
    }
	avancerAvecAjustement(valeurCapteur);
	_delay_ms(100);
	repositionner(valeurCapteur);
    valeurCapteur = capteurLignes.getValeurIR();

    /*********** Virage 90 degres gauche *************/
    // Avance jusqu'a detection d'un angle a gauche (fin de section)
	virageFinSection(valeurCapteur);

	/*************************************************/
    /********** Rotation de fin de section ***********/
    /*************************************************/
	rotationFinSection(valeurCapteur);
}

int main()
{
	/*************************************************/ 
    /*********** Initialisation des ports ************/
    /*************************************************/
	DDRA = 0x00;	// En entree pour les capteurs de lignes et IR
	DDRB = 0xff;	// En sortie pour les moteurs
	DDRC = 0xff;	// En sortie pour les LED du breadboard
	DDRD = 0Xff;	// En sortie pour le buzzer
	initialisationBouttonPoussoir();
	boutonPoussoir = 0;

    /*************************************************/ 
    /*** Creation des objets et variable du robot ****/
    /*************************************************/
    uint8_t compteurSections = 0;
	backupIR = true;
	Sections currentSection = notSet;
	uint8_t numeroPremiereSection = getNumeroPremiereSection();
    
    /*************************************************/
    /*************** Depart du robot *****************/
    /******* Reception du numero de depart ***********/
    /*************************************************/

	// Plus besoin apres la surchage de l'operateur
	if (numeroPremiereSection == 1) {
		currentSection = sct1;
		PORTC = 0x40;
	}
	else if (numeroPremiereSection == 2){
		currentSection = sct2;
		PORTC = 0x20;
	}
	else if (numeroPremiereSection == 3){
		currentSection = sct3;
		PORTC = 0x10;
	}
	else if (numeroPremiereSection == 4){
		currentSection = sct4;
		PORTC = 0x08;
	}

	_delay_ms(3000);
	backupIR = false;
    /*************************************************/
    /******** Enchainement des 4 sections ************/
    /*************************************************/
    while (compteurSections < 4) {
        switch (currentSection)
        {
            case sct1:
				section1();
                currentSection = sct2;
                compteurSections++;
                break;

            case sct2:
                section2();
                currentSection = sct3;
                compteurSections++;
                break;

            case sct3:
                section3();
                currentSection =sct4;
                compteurSections++;
                break;

            case sct4:
                section4();
                currentSection = sct1;
                compteurSections++;
                break;
			default:
				break;
        }
	}
    /*************** FIN D'EPREUVE *******************/
    /************** sons aigu de 2s ******************/
	DDRD = 0Xff;
    buzzerRobot.buzzerFini();
}
