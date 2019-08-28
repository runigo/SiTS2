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

#include "mobile.h"

int mobileCollisionElastique(mobileT * m1, mobileT * m2);

float mobileDistanceCarre(mobileT * m1, mobileT * m2);
float mobileDistanceGeom(mobileT * m1, mobileT * m2);

void mobileInverseVy(mobileT * mobile, int hauteur);
void mobileInverseVx(mobileT * mobile, int largeur);

void mobileInverseVxThermique(mobileT * mobile, int largeur, float temperature);

float mobileModuleVitesse(mobileT * mobile)
	{
	return (*mobile).vitesse;
	}
float mobileEnergieCinetique(mobileT * mobile)
	{
		// Mise à jour de l'énergie cinétique et de la vitesse
	double vx, vy;

		// Calcul de la vitesse
	//vx=((*mobile).actuel.x)-((*mobile).nouveau.x);
	//vy=((*mobile).actuel.y)-((*mobile).nouveau.y);
	vx=((*mobile).actuel.x)-((*mobile).ancien.x);
	vy=((*mobile).actuel.y)-((*mobile).ancien.y);

	(*mobile).ec=(vx*vx+vy*vy);
	(*mobile).vitesse=sqrt((*mobile).ec);

	return (*mobile).ec;
	}

void mobileInitialise(mobileT * mobile, montageT * montage, int nom, int diametre, float vitesse)
	{
	(*mobile).actuel.x = ((1.0/RAND_MAX)*((*montage).demiLargeur)*aleatoireRandMax());
	(*mobile).actuel.y = ((1.0/RAND_MAX)*((*montage).hauteur)*aleatoireRandMax() - (*montage).demiHauteur);

	if( (*mobile).actuel.x < (*montage).demiLargeur/2 )
		{
		(*mobile).nouveau.x=(*mobile).actuel.x+vitesse;
		(*mobile).ancien.x=(*mobile).actuel.x-vitesse;
		}
	else
		{
		(*mobile).nouveau.x=(*mobile).actuel.x-vitesse;
		(*mobile).ancien.x=(*mobile).actuel.x+vitesse;
		}

	if((*mobile).actuel.y < 0)
		{
		(*mobile).nouveau.y=(*mobile).actuel.y+vitesse;
		(*mobile).ancien.y=(*mobile).actuel.y-vitesse;
		}
	else
		{
		(*mobile).nouveau.y=(*mobile).actuel.y-vitesse;
		(*mobile).ancien.y=(*mobile).actuel.y+vitesse;
		}

	(*mobile).nom = nom;
	(*mobile).dernier = nom;
	(*mobile).vitesse = vitesse;
	(*mobile).ec = vitesse * vitesse ;
	(*mobile).collision = 0;
	(*mobile).droite = 1;

	(*mobile).diametre=diametre;
	(*mobile).diamCarre=diametre*diametre*SECTION_EFFICACE;
	return;
	}

void mobileInertie(mobileT * mobile)
	{		// Application du principe d'inertie
	((*mobile).nouveau.x)=2.0*((*mobile).actuel.x)-((*mobile).ancien.x);
	((*mobile).nouveau.y)=2.0*((*mobile).actuel.y)-((*mobile).ancien.y);

	return;
	}

void mobileParoi(mobileT * mobile, montageT * montage)
	{
			// Chocs avec les parois horizontales

	if ( (*mobile).nouveau.y > (*montage).demiHauteur )
		{
		mobileInverseVy(mobile, (*montage).hauteur);
		}
	if ( (*mobile).nouveau.y < -(*montage).demiHauteur )
		{
		mobileInverseVy(mobile, - (*montage).hauteur);
		}

			// Chocs avec les thermostats

	if ( (*mobile).nouveau.x > (*montage).demiLargeur )	// Droite
		{
		switch ((*montage).thermostat.actif)
			{
			case 0:
				mobileInverseVx(mobile, (*montage).largeur);break;
			case 1:
				mobileInverseVxThermique(mobile, (*montage).largeur, (*montage).thermostat.uniforme);break;
			case 2:
				if((*montage).thermostat.etatDroite==1)
				mobileInverseVxThermique(mobile, (*montage).largeur, (*montage).thermostat.droite);
				else
				mobileInverseVx(mobile, (*montage).largeur);
				break;
			default:
				;
			}
		}
	if ( (*mobile).nouveau.x < - (*montage).demiLargeur )	// Gauche
		{
		switch ((*montage).thermostat.actif)
			{
			case 0:
				mobileInverseVx(mobile, - (*montage).largeur);break;
			case 1:
				mobileInverseVxThermique(mobile, - (*montage).largeur, (*montage).thermostat.uniforme);break;
			case 2:
				if((*montage).thermostat.etatGauche==1)
				mobileInverseVxThermique(mobile, - (*montage).largeur, (*montage).thermostat.gauche);
				else
				mobileInverseVx(mobile, - (*montage).largeur);
				break;
			default:
				;
			}
		}

			// Chocs avec la paroi centrale

	if((*montage).paroiCentrale == 0) // Pas de paroi
		{
				// Arrivé de gauche
		if( (*mobile).nouveau.x > 0 && (*mobile).droite == 0)
			{
			(*mobile).droite = 1;
			}
			// Arrivé de droite
		if( (*mobile).nouveau.x < 0 && (*mobile).droite == 1)
			{
			(*mobile).droite = 0;
			}
		}
	else	// Paroi
		{			// 	Rebond sur la cloison
		if( (*mobile).nouveau.y >= (*montage).trou || (*mobile).nouveau.y <= - (*montage).trou )
			{
				// Arrivé de gauche
			if( (*mobile).nouveau.x > 0 && (*mobile).droite == 0)
				{
				mobileInverseVx(mobile, 0);
				}
				// Arrivé de droite
			if( (*mobile).nouveau.x < 0 && (*mobile).droite == 1)
				{
				mobileInverseVx(mobile, 0);
				}
			}
		else			// 	Passe par le trou
			{
				// Arrivé de gauche
			if( (*mobile).nouveau.x > 0 && (*mobile).droite == 0)
				{
				if((*montage).demonMaxwell==1)	// Démon de maxwell
					{
					mobileInverseVx(mobile, 0);
					}
				else
					{
					(*mobile).droite = 1;
			//fprintf(stderr, "(*mobile).droite = %d\n", (*mobile).droite);
					}
				}
				// Arrivé de droite
			if( (*mobile).nouveau.x < 0 && (*mobile).droite == 1)
				{
				if((*montage).demonMaxwell==2)	// Démon de maxwell
					{
					mobileInverseVx(mobile, 0);
					}
				else
					{
					(*mobile).droite = 0;
			//fprintf(stderr, "(*mobile).droite = %d\n", (*mobile).droite);
					}
				}
			}
		}
	return;
	}

void mobileCollision(mobileT * m1, mobileT * m2)
	{
	int distance;
	double distCarre;

	if( (*m1).droite == (*m2).droite ) // m1 et m2 sont du même coté.
	if( (*m1).collision==0 && (*m2).collision==0 ) // Supprime les collisions à trois particules.
	if( (*m2).dernier != (*m1).nom && (*m1).dernier != (*m2).nom ) // Intrication des particules.
		{
		//fprintf(stderr, "Distance géométrique < diametre\n");
		distance=(mobileDistanceGeom(m1, m2));
		if(distance<(*m1).diametre)
			{
			//fprintf(stderr, "Distance au carré < diametre au carré\n");
			distCarre=(mobileDistanceCarre(m1, m2));
			if(distCarre<(*m1).diamCarre)
				{
				//fprintf(stderr, "mobileCollision %d , %d\n", (*m1).nom, (*m2).nom);
					// Collision élastique
				mobileCollisionElastique(m1, m2);
					// Deviennent transparents
				mobileTransparent(m1, m2);
				}
			}
		}
	return;
	}

int mobileCollisionElastique(mobileT * m1, mobileT * m2)
	{
	vecteurT p;// vitesse relative au centre de masse, vitesse du centre de masse
	vecteurT v;// nouvelle vitesse relative au centre de masse

			// AVANT LE CHOC

		// vitesse relative au centre de masse
	p.x=( (*m1).nouveau.x-(*m1).ancien.x - (*m2).nouveau.x+(*m2).ancien.x )/4;
	p.y=( (*m1).nouveau.y-(*m1).ancien.y - (*m2).nouveau.y+(*m2).ancien.y )/4;

		// Rotation aléatoire de la vitesse relative au centre de masse
	vecteurRotationAleatoire(&p, &v);

		// Vitesse du centre de masse
	p.x=( (*m1).nouveau.x-(*m1).ancien.x + (*m2).nouveau.x-(*m2).ancien.x )/4;
	p.y=( (*m1).nouveau.y-(*m1).ancien.y + (*m2).nouveau.y-(*m2).ancien.y )/4;

			// APRÉS LE CHOC

		// L'actuel devient le nouveau 
	(*m1).actuel.x=(*m1).nouveau.x;
	(*m1).actuel.y=(*m1).nouveau.y;
	(*m2).actuel.x=(*m2).nouveau.x;
	(*m2).actuel.y=(*m2).nouveau.y;

		// on ajoute p+v et p-v à l'actuel.
	(*m1).ancien.x=(*m1).actuel.x - ( v.x + p.x );
	(*m1).ancien.y=(*m1).actuel.y - ( v.y + p.y );
	(*m2).ancien.x=(*m2).actuel.x + v.x - p.x;
	(*m2).ancien.y=(*m2).actuel.y + v.y - p.y;

	(*m1).nouveau.x=(*m1).actuel.x + v.x + p.x;
	(*m1).nouveau.y=(*m1).actuel.y + v.y + p.y;
	(*m2).nouveau.x=(*m2).actuel.x - v.x + p.x;
	(*m2).nouveau.y=(*m2).actuel.y - v.y + p.y;

		// Remise à zéro du libre parcours moyen
	(*m1).lpm=0.0;
	(*m2).lpm=0.0;

		// Mise à jour de l'énergie cinétique et de la vitesse
	mobileEnergieCinetique(m1);
	mobileEnergieCinetique(m2);

	return 0;
	}

int mobileIncrementeLibreParcoursMoyen(mobileT * mobile)
	{
	(*mobile).lpm=(*mobile).lpm + (*mobile).vitesse;
	return 0;
	}

void mobileTransparent(mobileT * m1, mobileT * m2)
	{
	(*m1).collision=1;
	(*m2).collision=1;
	(*m1).dernier=(*m2).nom;
	(*m2).dernier=(*m1).nom;
	return;
	}

void mobileIncremente(mobileT * mobile)
	//	L'ancien état reçoit l'état actuel,
	//	l'état actuel reçoit le nouvel état.
	{
		// Incrémentation des positions
	((*mobile).ancien.x)=((*mobile).actuel.x);
	((*mobile).ancien.y)=((*mobile).actuel.y);
	((*mobile).actuel.x)=((*mobile).nouveau.x);
	((*mobile).actuel.y)=((*mobile).nouveau.y);

	return;
	}

float mobileDistanceCarre(mobileT * m1, mobileT * m2)
	{
	float x2, y2;
	x2=((*m1).nouveau.x)-((*m2).nouveau.x);
	y2=((*m1).nouveau.y)-((*m2).nouveau.y);
	x2=x2*x2;
	y2=y2*y2;
	return(x2+y2);
	}

float mobileDistanceGeom(mobileT * m1, mobileT * m2)
	{
	float x, y, max;
	x=((*m1).nouveau.x)-((*m2).nouveau.x);
	y=((*m1).nouveau.y)-((*m2).nouveau.y);
	if(x<0) x = -x;
	if(y<0) y = -y;
	max = x;
	if(x<y) max = y;
	return max;
	}

void mobileInverseVy(mobileT * mobile, int hauteur)
	{
	(*mobile).nouveau.y = hauteur - (*mobile).nouveau.y;
	(*mobile).actuel.y = hauteur - (*mobile).actuel.y;
	(*mobile).ancien.y = hauteur - (*mobile).ancien.y;

	return;
	}

void mobileInverseVx(mobileT * mobile, int largeur)
	{
	(*mobile).nouveau.x = largeur - (*mobile).nouveau.x;
	(*mobile).actuel.x = largeur - (*mobile).actuel.x;
	(*mobile).ancien.x = largeur - (*mobile).ancien.x;

	return;
	}

void mobileInverseVxThermique(mobileT * mobile, int largeur, float temperature)
	{
	float lambda;
	vecteurT v;

		// Collision élastique
	mobileInverseVx(mobile, largeur);

		// Calcul de lambda
	lambda = sqrt( (1 + temperature/(*mobile).ec) / 2 );

		// Vitesse et variation
	vecteurDifference(&(*mobile).nouveau, &(*mobile).actuel, &v);
	vecteurMultiplie(&v, lambda);

		// Changement de vitesse
	vecteurSomme(&(*mobile).actuel, &v, &(*mobile).nouveau);
	vecteurDifference(&(*mobile).actuel, &v, &(*mobile).ancien);

		// Mise à jour de l'énergie cinétique
	(*mobile).ec=mobileEnergieCinetique(mobile);

	return;
	}

void mobileAffiche(mobileT * mobile)
	{
			fprintf(stderr, " (*mobile).droite = %d\n", (*mobile).droite);
	return;
	}

/*
*/
////////////////////////////////////////////////////////////////////////////////

