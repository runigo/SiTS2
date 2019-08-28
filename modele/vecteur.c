
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

#include "vecteur.h"

void vecteurUnitaireAleatoire(vecteurT * vect)
	{
	double angle;
	angle=aleatoireRadian();
	(*vect).x=cos(angle);
	(*vect).y=sin(angle);
	;
	}

void vecteurDifference(vecteurT * v1, vecteurT * v2, vecteurT * v)
	/*
		 v = v1 - v2
	*/
	{
	(*v).x = (*v1).x - (*v2).x;
	(*v).y = (*v1).y - (*v2).y;

	return ;
	}

void vecteurSomme(vecteurT * v1, vecteurT * v2, vecteurT * v)
	/*
		 v = v1 + v2
	*/
	{
	(*v).x = (*v1).x + (*v2).x;
	(*v).y = (*v1).y + (*v2).y;

	return ;
	}

void vecteurMultiplie(vecteurT * v, float lambda)
	/*
		 v = lambda * v
	*/
	{
	(*v).x = lambda * (*v).x;
	(*v).y = lambda * (*v).y;

	return ;
	}


double vecteurScalaire(vecteurT * v1, vecteurT * v2)
	/*
		Renvoie le produit scalaire de v1 par v2
	*/
	{
	return (((*v1).x)*((*v2).x)+((*v1).y)*((*v2).y));
	}

double vecteurVectoriel(vecteurT * v1, vecteurT * v2)
	/*
		Renvoie le produit vectoriel de v1 par v2
	*/
	{
	return (((*v1).x)*((*v2).y)-((*v1).y)*((*v2).x));
	}

void vecteurRotation(vecteurT * initial, vecteurT * tourner, double angle)
	/*
		Le vecteurT "tourner" est le produit de la rotation
		de l'angle "angle" du vecteurT "initial"
	*/
	{
	double cosin, sinus;
	cosin=cos(angle);
	sinus=sin(angle);
	(*tourner).x=((*initial).x)*cosin-((*initial).y)*sinus;
	(*tourner).y=((*initial).x)*sinus+((*initial).y)*cosin;
	}

void vecteurRotationAleatoire(vecteurT * initial, vecteurT * tourner)
	{
	double angle;
	angle=aleatoireRadian();
	vecteurRotation(initial, tourner, angle);
	}
