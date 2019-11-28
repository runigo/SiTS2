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

#ifndef _CONSTANTES_
#define _CONSTANTES_

//#include "unistd.h"

#include <assert.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define PI 3.14159265359
#define DEUXPI 6.28318530718


	//	PARAMÈTRES GRAPHIQUES


#define TEMPS_AFFICHAGE 110 // Durée entre les affichages en ms

#define MEMOIRE_CHRONO 10 // Nombre de points en mémoire des durées chronométrées

#define FENETRE_X 892 // Largeur de la fenêtre
#define FENETRE_Y 653 // Hauteur de la fenêtre

#define BOUTON_COMMANDES 16 // Nombres de petits boutons
#define ROTATIF_COMMANDES 7 // Nombre de boutons rotatifs

#define LINEAIRE_COMMANDES 3 // Nombres de commandes linéaires
#define TRIANGLE_COMMANDES 12 // Nombres de commandes triangulaires

#define CAPTEURS 6 // Nombres de capteurs
#define DUREE_CAPTEURS 128 // Nombre de points en mémoire
#define DY_ENERGIE (DUREE_CAPTEURS / 16) // Pour la densité d'énergie.

#define MARGE 17 // Marge fenêtre-montage

#define TRAIT_CLOISON 4
#define TRAIT_ENCEINTE 6


	//	PARAMÈTRES DU SYSTÈME


#define ALEA 77 // Taille du tableau de nombre aléatoire

#define LARGEUR 600 // Largeur du système
#define HAUTEUR 300 // Hauteur du système

#define DY_PRESSION (HAUTEUR / 10) // Epaisseur du capteur de pression

#define NOMBRE_MIN 9 // Nombre minimal de particules
#define NOMBRE_MAX 249 // Nombre maximal de particules
#define TAILLE_MIN 7 // Taille minimale des particules
#define TAILLE_MAX 29 // Taille maximale des particules
#define SECTION_EFFICACE 0.66 // Paramètre de section efficace

#define RAYON_TROU 7 // Rayon implicite du trou dans la cloison

#define DUREE_MAX 99 // Nombre maximal d'itération du système par affichage
#define DUREE_MIN 1 // Nombre d'itération initial

#define TEMPERATURE_MAX 33.3 // Température maximale
#define TEMPERATURE_MIN 0.0333 // Température minimale

#endif
/////////////////////////////////////////////////////////////
