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

#ifndef _THERMOSTAT_
#define _THERMOSTAT_

#include "../donnees/constantes.h"

typedef struct ThermostatT thermostatT;
	struct ThermostatT
		{
		float uniforme;	//	Température gauche et droite
		float gauche;		//	Température gauche
		float droite;		//	Température droite

		int actif;		//	0:système isolé, 1:température uniforme, 2:températures gauche-droite
		int etatDroite;		//	0: isolé à droite, 1:températures droite
		int etatGauche;		//	0: isolé à gauche, 1:température gauche
		};

void thermostatInitialise(thermostatT * thermostat);

void thermostatChangeEtat(thermostatT * thermostat, int uniforme);

void thermostatChangeTemperature(thermostatT * thermostat, float facteur);

void thermostatChangeEtatGauche(thermostatT * thermostat, int etat);
void thermostatChangeEtatDroite(thermostatT * thermostat, int etat);
void thermostatChangeTemperatureGauche(thermostatT * thermostat, float facteur);
void thermostatChangeTemperatureDroite(thermostatT * thermostat, float facteur);

void thermostatInverseTemperature(thermostatT * thermostat);

void thermostatAfficheTemperature(thermostatT * thermostat);
void thermostatAfficheThermostat(thermostatT * thermostat);


#endif
