
/*
Copyright septembre 2017, Stephan Runigo
runigo@free.fr
SiGP 1.3.3  simulateur de gaz parfait
Ce logiciel est un programme informatique servant à simuler un gaz parfait
et à en donner une représentation graphique. Il permet d'observer une détente
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

#include "aleatoire.h"

int aleatoireRandMax(void)
	{
	/*
		Produit un entier aléatoire entre 0 et RAND_MAX

		Lors du premier appel, un tableau de nombre aléatoire est construit.

		Lors d'un appel, une case du tableau est choisie aléatoirement,
		son contenu est renvoyée par la fonction puis remplacé par un 
		nouveau nombre aléatoire.
	*/

	static int tableauAleatoire[ALEA];// Tableau d'entier aléatoire
	static int first = 0;// Marque le premier appel
	int index, entier;
	if(first == 0)
		{// Initialisation du tableau lors du premier appel
		int i;
		srand(time(NULL));// initialisation de la graine
		for(i=0;i<ALEA;i++)
			tableauAleatoire[i] = rand();
		first = 1;
		}
	index=(int)((rand())*(1.0/RAND_MAX)*(ALEA-1));//  Choix aléatoire d'une case du tableau
	entier=tableauAleatoire[index];// qui donne l'entier
	tableauAleatoire[index] = rand();// Changement de la case utilisée.
	return(entier);
	}

double aleatoireRadian(void)
	{
	/*
		Produit un angle aléatoire entre 0 et 2 PI
	*/

	double angle;
	angle=(PI*(aleatoireRandMax()))*(2.0/RAND_MAX);
	return angle;
	}

