/*
Copyright novembre 2018, Stephan Runigo
runigo@free.fr
SiGP 2.2.2  simulateur de gaz parfait
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

#include "systeme.h"

void systemeIncremente(systemeT * systeme); // L'ancien état reçoit l'état actuel qui reçoit le nouvel état.
void systemeMoteur(systemeT * systeme); // Calcule le nouvel état du système en fonction de l'ancien et de l'actuel

void sytemeAjouteParticule(systemeT * systeme); // Ajoute une particule au systeme

void systemeParoi(systemeT * systeme);
void systemeInertie(systemeT * systeme);
void systemeChocsParticules(systemeT * systeme);
void systemeIncrementeLibreParcoursMoyen(systemeT * systeme);

void systemeInitialisePosition(systemeT * systeme, int forme)
	{
	(void)forme;
		// Initialisation des mobiles
	int i;
	for(i=0;i<(*systeme).nombre;i++)
		{
		if(i%2==0)
			{
			mobileInitialise(&(*systeme).mobile[i], &(*systeme).montage, i, (*systeme).diametre, (*systeme).vitesseInitiale);
			}
		else		// Possibilité vitesse = 0 pour la moitié des particules
			{
			mobileInitialise(&(*systeme).mobile[i], &(*systeme).montage, i, (*systeme).diametre, (*systeme).vitesseInitiale);
			//mobileInitialise(&(*systeme).mobile[i], &(*systeme).montage, 0, i);
			}
		}
	return;
	}

void systemeInitialise(systemeT * systeme, int nombre, int taille, float vitesse)
	{
		// Nombre de particules
	(*systeme).nombre = nombre;

		// Taille des particules
	(*systeme).diametre = taille;

		// Conditions initiales
	(*systeme).vitesseInitiale = vitesse;

		// Initialisation des mobiles
	systemeInitialisePosition(systeme, 1);

	return;
	}

void systemeEvolue(systemeT * systeme, int duree)
	{		//  Évolution du système
	int i;
		//fprintf(stderr, "Evolution temporelle du systeme \n");
	for (i=0;i<duree;i++)
		{
			//fprintf(stderr, "systemeMoteur, calcul nouvel état\n");
		systemeMoteur(systeme);
		}
	return;
	}

void systemeMoteur(systemeT * systeme)
/*
	Calcule le nouvel état du système en fonction de l'actuel et du nouveau
*/
	{
		//fprintf(stderr, "systemeIncremente, évolution ancien état\n");
	systemeIncremente(systeme); // Fait disparaitre l'ancien

		//fprintf(stderr, "systemeInertie, Propagation inertielle\n");
	systemeInertie(systeme); // Calcule la nouvelle position

		//fprintf(stderr, "systemeChocsParticules, Chocs entre particules\n");
	systemeChocsParticules(systeme); // Corrige les coordonnées

		//fprintf(stderr, "systemeParoi, Chocs avec les parois\n");
	systemeParoi(systeme); // Corrige les coordonnées

		// Incrémentation du libre parcours moyen
	systemeIncrementeLibreParcoursMoyen(systeme);

	return;
	}

void systemeInertie(systemeT * systeme)
/*
		Propagation inertielle
*/
	{
	int i;

	for(i=0;i<(*systeme).nombre;i++)			//	Application du principe d'inertie
		{
		mobileInertie(&(*systeme).mobile[i]);
		}
	return;
	}

void systemeParoi(systemeT * systeme)
/*
		Chocs avec les parois
*/
	{
	int i;

	for(i=0;i<(*systeme).nombre;i++)			//	Chocs avec les parois
		{
		mobileParoi(&(*systeme).mobile[i], &(*systeme).montage);
		}
	return;
	}

void systemeChocsParticules(systemeT * systeme)
/*
		Chocs entre particules
*/
	{
	int i, j;

	for(i=0;i<(*systeme).nombre;i++)		//	Réinitialisation
		{
		(*systeme).mobile[i].collision = 0;
		}

	for(i=0;i<(*systeme).nombre;i++)			//	Chocs entre particules
		{
		for(j=i+1;j<(*systeme).nombre;j++)
			{
				{
				//fprintf(stderr, "mobileCollision entre i et j %d %d\n", i, j);
				mobileCollision(&(*systeme).mobile[i], &(*systeme).mobile[j]);
				}
			}
		}

	return;
	}

void systemeIncrementeLibreParcoursMoyen(systemeT * systeme)
	{
	int i;
	for(i=0;i<(*systeme).nombre;i++)
		{
		mobileIncrementeLibreParcoursMoyen(&(*systeme).mobile[i]);
		}
	return;
	}

void systemeIncremente(systemeT * systeme)
	//	L'ancien état reçoit l'état actuel,
	//	l'état actuel reçoit le nouvel état.
	{
	int i;
	for(i=0;i<(*systeme).nombre;i++)
		{
		mobileIncremente(&(*systeme).mobile[i]);
		}
	return;
	}

void systemeChangeNombre(systemeT * systeme, int delta)
/*
		Change le nombre de particules
*/
	{
	int nombre = (*systeme).nombre + delta;

	if(delta==1 && nombre < NOMBRE_MAX)
		{
		(*systeme).nombre = nombre;
		sytemeAjouteParticule(systeme);
		}
	else
		{
		if(nombre > NOMBRE_MIN && delta==-1)
			{
			(*systeme).nombre = nombre;
			}
		else
			{
			if(nombre < NOMBRE_MIN)
				{
				printf("Nombre minimal atteint. ");
				}
			if(nombre > NOMBRE_MAX)
				{
				printf("Nombre maximal atteint. ");
				}
			}
		}

	printf("Nombre de particules = %d\n", (*systeme).nombre);

	return;
	}

void sytemeAjouteParticule(systemeT * systeme)
/*
		Changement de la surface efficace des particules
*/
	{
	mobileInitialise(&(*systeme).mobile[(*systeme).nombre], &(*systeme).montage, (*systeme).nombre,(*systeme).diametre, (*systeme).vitesseInitiale);
	return;
	}

void systemeChangeDiametre(systemeT * systeme, float facteur)
/*
		Changement de la surface efficace des particules
*/
	{
	int i;
	int diametre = 1;
	float diamCarre = 0.71;

	//(*mobile).diametre=TAILLE;
	//(*mobile).diamCarre=TAILLE*TAILLE*0.71;

	if( (*systeme).diametre > TAILLE_MAX && facteur > 1 )
		{
		fprintf(stderr, "Diamètre maximal atteint. ");
		}
	else
		{
		if( facteur > 1)
			{
			(*systeme).diametre ++;
			}
		else
			{
			if( (*systeme).diametre > TAILLE_MIN )
				{
				(*systeme).diametre --;
				}
			else
				{
				fprintf(stderr, "Diamètre minimal atteint. ");
				}
			}
		}

	diametre = (*systeme).diametre;
	diamCarre = diametre*diametre*0.71;

		//	Réinitialisation surface efficace
	for(i=0;i<(*systeme).nombre;i++)
		{
		(*systeme).mobile[i].diametre = diametre;
		(*systeme).mobile[i].diamCarre = diamCarre;
		}

	printf("Diamètre des particules = %d\n", diametre);
	return;
	}

////////////////////////////////////////////////////////////////////////////////

