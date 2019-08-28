/*
Copyright octobre 2018, Stephan Runigo
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

#include "capteurs.h"

int capteursInitialise(capteursT * capteurs)
	{
	int i, j;
	for(j=0;j<CAPTEURS;j++)
		{
		(*capteurs).capteur[j].yZero=0; // Positon de l'origine capteur[CAPTEURS]
		(*capteurs).capteur[j].xZero=0; // Positon de l'origine
		for(i=0;i<DUREE_CAPTEURS;i++)
			{
			(*capteurs).capteur[j].gauche[i].x=0;
			(*capteurs).capteur[j].gauche[i].y=0;
			(*capteurs).capteur[j].droite[i].x=0;
			(*capteurs).capteur[j].droite[i].y=0;
			}
		}
	for(j=0;j<CAPTEURS;j++)
		{
		(*capteurs).capteur[j].largeur = 1; // axe x
		(*capteurs).capteur[j].hauteur = 1; // axe y
		}

	return 0;
	}

int capteursMiseAJourLongueur(capteursT * capteurs, int largeur, int hauteur)
	{

	(*capteurs).capteur[0].xZero=(int)(0.08744395*largeur); // 	x
	(*capteurs).capteur[0].yZero=(int)(0.755*hauteur); // Température

	(*capteurs).capteur[1].xZero=(int)(0.33295964*largeur); // x
	(*capteurs).capteur[1].yZero=(int)(0.755*hauteur); // Nombre

	(*capteurs).capteur[2].xZero=(int)(0.58632287*largeur); // x
	(*capteurs).capteur[2].yZero=(int)(0.755*hauteur); // l. p. m.

	(*capteurs).capteur[3].xZero=(int)(0.08744395*largeur); // 	x
	(*capteurs).capteur[3].yZero=(int)(0.972*hauteur); // Température

	(*capteurs).capteur[4].xZero=(int)(0.33295964*largeur); // x
	(*capteurs).capteur[4].yZero=(int)(0.972*hauteur); // Nombre

	(*capteurs).capteur[5].xZero=(int)(0.58632287*largeur); // x
	(*capteurs).capteur[5].yZero=(int)(0.972*hauteur); // l. p. m.

	int j;
	for(j=0;j<CAPTEURS;j++)
		{
		(*capteurs).capteur[j].largeur=(int)(0.17713*largeur); // longueur de l'axe x 164 165 165 / 892
		(*capteurs).capteur[j].hauteur=(int)(0.123*hauteur); // longueur de l'axe y 91 / 653
		}


	float a;
	int i, x0;
	for(j=0;j<CAPTEURS;j++)
		{
		if(j!=5)
			{
			x0=(*capteurs).capteur[j].xZero;
			a=(float)((*capteurs).capteur[j].largeur) / (float)(DUREE_CAPTEURS);
			for(i=0;i<DUREE_CAPTEURS;i++)
				{
				(*capteurs).capteur[j].gauche[i].x=(int)(a*i)+x0;
				(*capteurs).capteur[j].droite[i].x=(int)(a*i)+x0;
				}
			}
		else
			{
			x0=(*capteurs).capteur[j].xZero;
			a=(float)((*capteurs).capteur[j].largeur) / (float)(DY_ENERGIE);
			for(i=0;i<DY_ENERGIE;i++)
				{
				(*capteurs).capteur[j].gauche[i].x=(int)(a*i)+x0;
				(*capteurs).capteur[j].droite[i].x=(int)(a*i)+x0;
				}
			}
		}

	return 0;
	}

