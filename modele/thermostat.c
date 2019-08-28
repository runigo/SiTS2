/*
Copyright novembre 2018, Stephan Runigo
runigo@free.fr
SiGP 2.2  simulateur de gaz parfait
Ce logiciel est un programme informatique servant à simuler un gaz et à
en donner une représentation graphique. Il permet d'observer une détente
de Joule ainsi que des transferts thermiques avec des thermostats.
Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".
En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme, le
titulaire des droits patrimoniaux et les concédants successifs.
A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies. Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

#include "thermostat.h"

void thermostatInverseEtatGauche(thermostatT * thermostat);
void thermostatInverseEtatDroite(thermostatT * thermostat);

void thermostatAfficheTemperature(thermostatT * thermostat);

void thermostatInitialise(thermostatT * thermostat)

			//	Initialisation du thermostat
	{
	
	(*thermostat).uniforme = sqrt(TEMPERATURE_MAX * TEMPERATURE_MIN);//	Température moyenne du thermostat
	(*thermostat).gauche = TEMPERATURE_MIN;	//	Température gauche
	(*thermostat).droite = TEMPERATURE_MAX;	//	Température droite
	(*thermostat).actif = 0;	//	0 : Système isolé, 1:température uniforme, 2:active gauche-droite
	(*thermostat).etatDroite = 0;		//	0: isolé à droite, 1:températures droite
	(*thermostat).etatGauche = 0;		//	0: isolé à gauche, 1:température gauche
	return;
	}

void thermostatChangeEtat(thermostatT * thermostat, int mode)
	{
	//		Change l'activité des thermostats

	(*thermostat).actif = mode;

	switch( (*thermostat).actif )
		{
		case 0:
			printf("(*thermostat).actif = %d : système isolé\n", (*thermostat).actif);break;
		case 1:
			printf("(*thermostat).actif = %d : thermostat uniforme\n", (*thermostat).actif);break;
		case 2:
			printf("(*thermostat).actif = %d : thermostats gauche-droite\n", (*thermostat).actif);break;
		default:
			;
		}
	return;
	}

void thermostatChangeEtatGauche(thermostatT * thermostat, int etat)
		// Change l'état du thermostat gauche
	{
	switch(etat)
		{
		case 0:
			(*thermostat).etatGauche = 0;
			fprintf(stderr, "Thermostat gauche éteint\n");
			break;
		case 1:
			(*thermostat).etatGauche = 1;
			fprintf(stderr, "Thermostat gauche allumé\n");
			break;
		case -1:
			if((*thermostat).etatGauche==1)
			{
			(*thermostat).etatGauche = 0;
			fprintf(stderr, "Thermostat gauche éteint\n");
			}
			else
			{
			(*thermostat).etatGauche = 1;
			fprintf(stderr, "Thermostat gauche allumé\n");
			}
		default:
			;
		}
	return;
	}

void thermostatChangeEtatDroite(thermostatT * thermostat, int etat)
		// Change l'état du thermostat droit
	{
	switch(etat)
		{
		case 0:
			(*thermostat).etatDroite = 0;
			fprintf(stderr, "Thermostat droit éteint\n");
			break;
		case 1:
			(*thermostat).etatDroite = 1;
			fprintf(stderr, "Thermostat droit allumé\n");
			break;
		case -1:
			if((*thermostat).etatDroite==1)
			{
			(*thermostat).etatDroite = 0;
			fprintf(stderr, "Thermostat droit éteint\n");
			}
			else
			{
			(*thermostat).etatDroite = 1;
			fprintf(stderr, "Thermostat droit allumé\n");
			}
		default:
			(*thermostat).etatDroite = 0;
			fprintf(stderr, "ERREUR : Thermostat gauche éteint\n");
			;
		}
	return;
	}

void thermostatChangeTemperature(thermostatT * thermostat, float facteur)

			//	Change température moyenne
	{
	float uniforme = (*thermostat).uniforme * facteur;
	if (uniforme < TEMPERATURE_MAX && uniforme > TEMPERATURE_MIN)
		{
		(*thermostat).uniforme = (*thermostat).uniforme * facteur;
		printf("Température = %f\n", (*thermostat).uniforme);
		}
	else
		{
		if (uniforme > TEMPERATURE_MAX)
			{
			(*thermostat).uniforme = TEMPERATURE_MAX ;
			printf("Température maximale atteinte\n");
			printf("Température = %f\n", (*thermostat).uniforme);
			}
		if (uniforme < TEMPERATURE_MIN)
			{
			(*thermostat).uniforme = TEMPERATURE_MIN ;
			printf("Température minimale atteinte\n");
			printf("Température = %f\n", (*thermostat).uniforme);
			}
		}
	return;
	}
void thermostatChangeTemperatureDroite(thermostatT * thermostat, float facteur)

			//	Change température droite
	{
	float droite = (*thermostat).droite * facteur;
	if (droite < TEMPERATURE_MAX && droite > TEMPERATURE_MIN)
		{
		(*thermostat).droite = droite ;
		printf("Température droite = %f\n", (*thermostat).droite);
		}
	else
		{
		if (droite > TEMPERATURE_MAX)
			{
			(*thermostat).droite = TEMPERATURE_MAX ;
			printf("Température maximale atteinte\n");
			printf("Température droite = %f\n", (*thermostat).droite);
			}
		if (droite < TEMPERATURE_MIN)
			{
			(*thermostat).droite = TEMPERATURE_MIN ;
			printf("Température minimale atteinte\n");
			printf("Température droite = %f\n", (*thermostat).droite);
			}
		}
	return;
	}
void thermostatChangeTemperatureGauche(thermostatT * thermostat, float facteur)

			//	Change température gauche
	{
	float gauche = (*thermostat).gauche * facteur;
	if (gauche < TEMPERATURE_MAX && gauche > TEMPERATURE_MIN)
		{
		(*thermostat).gauche = gauche;
		printf("Température gauche = %f\n", (*thermostat).gauche);
		}
	else
		{
		if (gauche > TEMPERATURE_MAX)
			{
			(*thermostat).gauche = TEMPERATURE_MAX ;
			printf("Température maximale atteinte\n");
			printf("Température gauche = %f\n", (*thermostat).gauche);
			}
		if (gauche < TEMPERATURE_MIN)
			{
			(*thermostat).gauche = TEMPERATURE_MIN ;
			printf("Température minimale atteinte\n");
			printf("Température gauche = %f\n", (*thermostat).gauche);
			}
		}
	return;
	}

void thermostatAfficheTemperature(thermostatT * thermostat)

			// 	Affiche la valeur des paramètres du thermostat
	{
	printf("(*thermostat).uniforme = %f\n", (*thermostat).uniforme);
	printf("(*thermostat).gauche = %f\n", (*thermostat).gauche);
	printf("(*thermostat).droite = %f\n", (*thermostat).droite);
	return;
	}

void thermostatAfficheThermostat(thermostatT * thermostat)

			// 	Affiche la valeur des paramètres du thermostat
	{
	switch( (*thermostat).actif )
		{
		case 0:
			printf("\nSYSTEME ISOLÉ\n");
			break;
		case 1:
			printf("\nTHERMOSTAT UNIFORME\n");
			break;
		case 2:
			printf("\nTHERMOSTAT GAUCHE-DROITE\n");
			break;
		default:
			break;
		}
	printf("	Thermostat uniforme = %f\n", (*thermostat).uniforme);
	printf("	Thermostat gauche = %f\n", (*thermostat).gauche);
	printf("	Thermostat droite = %f\n", (*thermostat).droite);
	return;
	}

//////////////////////////////////////////////////////////////////////////
