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

#include "options.h"

void optionsPause(optionsT * options, char *opt);
void optionsDuree(optionsT * options, char *opt);

void optionsTemperature(optionsT * options, char *opt);
void optionsGauche(optionsT * options, char *opt);
void optionsDroite(optionsT * options, char *opt);

void optionsThermostat(optionsT * options, char *opt);
void optionsCloison(optionsT * options, char *opt);
void optionsTrou(optionsT * options, char *opt);
void optionsDiametre(optionsT * options, char *opt);
void optionsNombre(optionsT * options, char *opt);

void optionsAide(void);

int optionsInitialise(optionsT * options)
	{
		// Préréglage des valeurs optionnelles

	(*options).duree=DUREE_MIN;		// nombre d'incrémentation du système par affichage

	(*options).temperature = sqrt(TEMPERATURE_MAX * (TEMPERATURE_MIN + TEMPERATURE_MAX)/2);	// Température initiale
	(*options).gauche = TEMPERATURE_MIN;	// Température thermostat gauche
	(*options).droite = TEMPERATURE_MAX;	// Température thermostat droit

	(*options).thermostat=0;	// 0 : système isolé, 1 : système thermostaté.
	(*options).cloison = 2;		// 0 : pas de paroi centrale. 1 : cloison fermé, 2 : détente de joule,
					//	 -1, -2 : démon de maxwell.
	(*options).trou = RAYON_TROU;
	(*options).diametre = TAILLE_MAX/3;
	(*options).nombre = NOMBRE_MAX/2;

	return 0;
	}

int optionsTraitement(optionsT * options, int nb, char *opt[])
	{
	int i=0;	//fprintf(stderr, "\nNombre d'options : %d\n", nb);
	do
		{
		if(strcmp(opt[i], "duree")==0 && opt[i+1]!=NULL)
			optionsDuree(options, opt[i+1]);	// Nombre d'évolution du système entre les affichages.

		if(strcmp(opt[i], "thermostat")==0 && opt[i+1]!=NULL)
			optionsThermostat(options, opt[i+1]);	// Activation du thermostat, 0 : système isolé.
		if(strcmp(opt[i], "temperature")==0 && opt[i+1]!=NULL)
			optionsTemperature(options, opt[i+1]);	// Température initiale.
		if(strcmp(opt[i], "gauche")==0 && opt[i+1]!=NULL)
			optionsGauche(options, opt[i+1]);	// Température thermostat gauche.
		if(strcmp(opt[i], "droite")==0 && opt[i+1]!=NULL)
			optionsDroite(options, opt[i+1]);	// Température thermostat droit.

		if(strcmp(opt[i], "cloison")==0 && opt[i+1]!=NULL)
			optionsCloison(options, opt[i+1]);	// Activation cloison centrale.
		if(strcmp(opt[i], "trou")==0 && opt[i+1]!=NULL)
			optionsTrou(options, opt[i+1]);	// Trou dans la cloison centrale.
		if(strcmp(opt[i], "diametre")==0 && opt[i+1]!=NULL)
			optionsDiametre(options, opt[i+1]);	// Diamètre des particules.
		if(strcmp(opt[i], "nombre")==0 && opt[i+1]!=NULL)
			optionsNombre(options, opt[i+1]);	// Nombre de particules.

		if(strcmp(opt[i], "aide")==0)
			optionsAide();	// Affiche l'aide.
		if(strcmp(opt[i], "help")==0)
			optionsAide();	// Affiche l'aide.

  		i++;
  		}
		while(i<nb);
	return 0;
	}

void optionsDuree(optionsT * options, char *opt)
	{    	// Nombre d'évolution du système entre les affichages
	int duree = atoi(opt);
	if ( duree >= DUREE_MIN && duree < DUREE_MAX)
		{
		(*options).duree = duree;
		printf("Option duree valide, duree = %d\n", (*options).duree);
		}
	else
		{
		printf("Option duree non valide, duree = %d\n", (*options).duree);
		printf("Option duree : %d < duree < %d\n", DUREE_MIN, DUREE_MAX);
		}
	return;
	}

void optionsTemperature(optionsT * options, char *opt)
	{    	// Température initiale
	float temperature = atof(opt);
	if(temperature>TEMPERATURE_MIN && temperature<TEMPERATURE_MAX)
		{
		(*options).temperature = temperature;
		printf("Option temperature valide, température initiale = %6.3f\n", (*options).temperature);
		}
	else
		{
		printf("Option temperature non valide, température = %6.3f\n", (*options).temperature);
		printf("Option temperature : %6.3f < temperature < %6.3f\n", TEMPERATURE_MIN, TEMPERATURE_MAX);
		}
	return;
	}

void optionsGauche(optionsT * options, char *opt)
	{    	// Température initiale à gauche
	float gauche = atof(opt);
	if(gauche>TEMPERATURE_MIN && gauche<TEMPERATURE_MAX)
		{
		(*options).gauche = gauche;
		printf("Option gauche valide, température thermostat gauche = %6.3f\n", (*options).gauche);
		}
	else
		{
		printf("Option gauche non valide, gauche = %6.3f\n", (*options).gauche);
		printf("Option gauche : %6.3f < gauche < %6.3f\n", TEMPERATURE_MIN, TEMPERATURE_MAX);
		}
	return;
	}

void optionsDroite(optionsT * options, char *opt)
	{    	// Température initiale à droite
	float droite = atof(opt);
	if(droite>TEMPERATURE_MIN && droite<TEMPERATURE_MAX)
		{
		(*options).droite = droite;
		printf("Option droite valide, température thermostat droite = %6.3f\n", (*options).droite);
		}
	else
		{
		printf("Option droite non valide, droite = %6.3f\n", (*options).droite);
		printf("Option droite : %6.3f < droite < %6.3f\n", TEMPERATURE_MIN, TEMPERATURE_MAX);
		}
	return;
	}

void optionsThermostat(optionsT * options, char *opt)
	{		// choix du thermostat
	int thermostat = atoi(opt);
	if(thermostat > -3 && thermostat < 3)
		{
		(*options).thermostat = thermostat;
		printf("Option thermostat valide, thermostat = %d\n", (*options).thermostat);
		}
	else
		{
		printf("Option thermostat non valide, thermostat = %d\n", (*options).thermostat);
		printf("Option thermostat : -3 < thermostat < 3\n");
		}
	return;
	}

void optionsCloison(optionsT * options, char *opt)
	{
	int cloison = atoi(opt);
	if(cloison > -3 && cloison <3)
		{
		(*options).cloison = cloison;
		printf("Option cloison valide, cloison = %d\n", (*options).cloison);
		}
	else
		{
		printf("Option cloison non valide, cloison = %d\n", (*options).cloison);
		printf("Option cloison : -3 < cloison < 3\n");
		}
	return;
	}

void optionsTrou(optionsT * options, char *opt)
	{
	int trou = atoi(opt);
	if(trou > -1 && trou < (HAUTEUR-MARGE)/2+1)
		{
		(*options).trou = trou;
		printf("Option trou valide, trou = %d\n", (*options).trou);
		}
	else
		{
		printf("Option trou non valide, trou = %d\n", (*options).trou);
		printf("Option trou : 0 <= trou < %d\n", RAYON_TROU);
		}
	return;
	}


void optionsDiametre(optionsT * options, char *opt)
	{
	int diametre = atoi(opt);
	if(diametre > -1 && diametre < TAILLE_MAX+1)
		{
		(*options).diametre = diametre;
		printf("Option diametre valide, diametre = %d\n", (*options).diametre);
		}
	else
		{
		printf("Option diametre non valide, diametre = %d\n", (*options).diametre);
		printf("Option diametre : 0 <= diametre < %d\n", RAYON_TROU);
		}
	return;
	}


void optionsNombre(optionsT * options, char *opt)
	{
	int nombre = atoi(opt);
	if(nombre > -1 && nombre < NOMBRE_MAX+1)
		{
		(*options).nombre = nombre;
		printf("Option nombre valide, nombre = %d\n", (*options).nombre);
		}
	else
		{
		printf("Option nombre non valide, nombre = %d\n", (*options).nombre);
		printf("Option nombre : 0 <= nombre < %d\n", NOMBRE_MAX);
		}
	return;
	}

void optionsAide(void)
	{
	printf("\n\nAide de SiGP\n\n");

	printf("Options et commande du simulateur de thermodynamique\n\n");

	//printf("Option fond	0 < fond < 255	:	couleur du fond de l'affichage\n");
	//printf("	Fond noir : 0, fond blanc : 255\n\n");

	printf("OPTIONS DE LA LIGNE DE COMMANDE\n");
	printf("	\n");
	printf("pause	5 < pause < 555	:	pause entre les affichages en ms\n");
	printf("	\n");

	printf("duree	1 < duree < %d	:	nombre d'évolution du système entre les affichages\n", DUREE_MAX);
	printf("	\n");

	printf("temperature	%6.3e < temperature < %6.3f	Température du thermostat\n", TEMPERATURE_MIN, TEMPERATURE_MAX);
	printf("	\n");

	printf("gauche	%6.3f < gauche < %6.3f	Température à gauche \n", TEMPERATURE_MIN, TEMPERATURE_MAX);
	printf("	\n");

	printf("droite	%6.3f < droite < %6.3f	Température à droite \n", TEMPERATURE_MIN, TEMPERATURE_MAX);
	printf("	\n");

	printf("thermostat	-1 < thermostat < 3	Système isolé, système thermostaté, \n");
	printf("		0 : système isolé, 1 : thermostat symétrique \n");
	printf("		2 : thermostats droite-gauche. \n");

	printf("trou	-1 < trou < %d, taille du trou	\n", (HAUTEUR-MARGE)/2+1);

	printf("diametre	-1 < diametre < %d, diametre des particules	\n", TAILLE_MAX+1);

	printf("nombre	-1 < nombre < %d, nombre de particules	\n", NOMBRE_MAX+1);

	printf("cloison	-3 < cloison < 3	Cloison si <> 0\n");
	printf("		0 : pas de cloison, 1 : cloison percée,\n");
	printf("		2 : cloison fermé, -1 : démon de maxwell.\n");
	printf("\n");

	printf("COMMANDE DU CLAVIER\n\n");

	printf("duree	F9, F10, F11 et F12	nombre d'évolution du système entre les affichages\n");
	printf("	\n");

	printf("température	p, m : augmenter, diminuer la température du thermostat\n");
	printf("	\n");

	printf("gauche	y, h : augmenter, diminuer la température à gauche \n");
	printf("	\n");

	printf("droite	u, j : augmenter, diminuer la température \n");
	printf("	\n");

	printf("thermostat	o : Désactive le thermostat, système isolé, \n");
	printf("		i : Active le thermostat, système thermostaté,\n");
	printf("		k : Active les thermostats droite-gauche. \n");
	printf("	 \n");

	printf("diamètre	e, d : augmenter, diminuer la surface efficace\n");
	printf("	 \n");

	printf("trou	a, q : augmenter, diminuer la taille du trou\n");
	printf("	z, s : Trou max, valeur implicite\n");

	printf("cloison		w : pas de cloison, x : cloison fermée,\n");
	printf("		c : cloison percée, v : démon de maxwell.\n");
	printf("		b : démon de maxwell, n : démon de maxwell.\n");
	printf("\n");
	fprintf(stderr, "\nSortie de SiGP\n");
	exit(EXIT_FAILURE);
	return;
	}

////////////////////////////////////////////////////////////////////////////
