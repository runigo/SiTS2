/*
Copyright septembre 2019, Stephan Runigo
runigo@free.fr
SiTS 2.3  simulateur de thermodynamique statistique
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
offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme, le
titulaire des droits patrimoniaux et les concédants successifs.
A cet égard l'attention de l'utilisateur est attirée sur les risques
associés au chargement, à l'utilisation, à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant des connaissances informatiques approfondies. Les
utilisateurs sont donc invités à charger et tester l'adéquation du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

#include "montage.h"

int montageInitialiseTrou(montageT * montage, int trou)
		// Initialise
	{
	if(trou > -1 && trou < (HAUTEUR-MARGE)/2+1)
		{
		(*montage).trou = trou;
		printf("(*montage).trou = %d\n", (*montage).trou);
		return 0;
		}
	else
		{
		(*montage).trou = RAYON_TROU;
		printf("ERREUR montageInitialiseTrou(%d) (*montage).trou = %d\n", trou, (*montage).trou);
		}
	return 1;
	}

int montageInitialiseParoiCentrale(montageT * montage, int paroiCentrale)
		// Initialise
	{
	if(paroiCentrale > -3 && paroiCentrale <3)
		{
		(*montage).paroiCentrale = paroiCentrale;
		printf("(*montage).paroiCentrale = %d\n", (*montage).paroiCentrale);
		return 0;
		}
	else
		{
		(*montage).paroiCentrale = 0;
		printf("(*montage).paroiCentrale = %d\n", (*montage).paroiCentrale);
		}
	return 1;
	}

int montageInitialiseDemonMaxwell(montageT * montage, int demonMaxwell)
		// Initialise
	{
	(*montage).demonMaxwell = demonMaxwell;
	fprintf(stderr, "(*montage).demonMaxwell = %d\n", (*montage).demonMaxwell);
	return 0;
	}

void montageChangeParoiCentrale(montageT * montage, int mode)
		// 0 : Pas de cloison, 1 : Cloison fermée, 2 : Cloison percée
		// 0 : pas de paroi centrale, 1 : Cloison fermée, 2 : Trou.
	{
	(*montage).paroiCentrale = mode;

	switch((*montage).paroiCentrale)	//	Température gauche
		{
		case 1:
			(*montage).trou = 0;break; //	Marche
		case 2:
			(*montage).trou = RAYON_TROU;break; //	Marche
		default:
			;
		}

	fprintf(stderr, "Paroi centrale = %d, ", (*montage).paroiCentrale);
	fprintf(stderr, "trou = %d\n", (*montage).trou);

	return;
	}

void montageSupprimeDemon(montageT * montage)
		// Supprime démon de Maxwell
	{
	(*montage).demonMaxwell = 0;
	fprintf(stderr, "Démon de Maxwell désactivé\n");
	return;
	}

void montageChangeDemonMaxwell(montageT * montage)
		// Change l'état du démon de Maxwell
	{
	if((*montage).demonMaxwell==1)
		{
		(*montage).demonMaxwell = 0;
		fprintf(stderr, "Démon de Maxwell désactivé\n");
		}
	else
		{
		(*montage).demonMaxwell = 1;
		fprintf(stderr, "Démon de Maxwell activé\n");
		}
	return;
	}

void montageChangeTrou(montageT * montage, float facteur)
	{
	if(facteur<0)
		{
		if(facteur == -1)
			(*montage).trou = RAYON_TROU;
		else
			(*montage).trou = (*montage).demiHauteur;
		}
	else
		{
		if( (*montage).trou > (*montage).demiHauteur && facteur > 1 )
			{
			fprintf(stderr, "Maximum atteint, ");
			}
		else
			{
			if( (*montage).trou > 11 ){
				(*montage).trou = (*montage).trou * facteur;}
			else{
				if( facteur > 1){
					(*montage).trou ++;}
				else{
					if( (*montage).trou >= 1 ){
						(*montage).trou --;}
					else{
						fprintf(stderr, "Minimum atteint, ");}
					}
				}
			}
		}

	if( (*montage).trou > (*montage).demiHauteur) (*montage).trou = (*montage).demiHauteur;

	fprintf(stderr, "trou = %d\n", (*montage).trou);

	return;
	}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
