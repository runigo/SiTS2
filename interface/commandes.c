/*
Copyright octobre 2018, Stephan Runigo
runigo@free.fr
SiGP 2.2.1  simulateur de gaz parfait
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

#include "commandes.h"

int commandesInitialiseBoutons(commandesT * commandes, int largeur, int hauteur)
	{

	//	PANNEAU DROIT

		 // Zone des boutons rotatifs
	(*commandes).rotatifs=(int)(0.839378*largeur); // 972 / 1158
		// BOUTONS ROTATIFS SUIVANT X
	(*commandes).rotatifX=(int)(0.02*largeur); //    D-G /2 = 23 / 1158 // Rayon suivant X
	(*commandes).rotatifsCentre=(int)(0.8929188*largeur); // 1034 // Positon X des boutons rotatifs
		// BOUTONS ROTATIFS SUIVANT Y
	(*commandes).rotatifY=(int)(0.025*hauteur); // 21 / 691	Rayon suivant Y

	(*commandes).rotatifCentre[0]=(int)(0.098009*hauteur); //	Trou
	(*commandes).rotatifCentre[1]=(int)(0.2098009*hauteur); //	Particules

	(*commandes).rotatifCentre[2]=(int)(0.362940*hauteur); // Température
	(*commandes).rotatifCentre[3]=(int)(0.503828*hauteur); // Gauche
	(*commandes).rotatifCentre[4]=(int)(0.646248*hauteur); // Droite

	(*commandes).rotatifCentre[5]=(int)(0.7963246*hauteur); // Simulation
	(*commandes).rotatifCentre[6]=(int)(0.923430*hauteur); // Nombre

		 // Zone des petits boutons
	(*commandes).boutons=(int)(0.943005*largeur); // 1092 / 1158
		// PETITS BOUTONS SUIVANT X
	(*commandes).boutonX=(int)(0.013*largeur); // Rayon suivant X
	(*commandes).boutonsCentre=(int)(0.96373*largeur); // Positon X des petits boutons
		// PETITS BOUTONS SUIVANT Y
	(*commandes).boutonY=(int)(0.015*largeur); // Rayon suivant Y // 10 / 667

	(*commandes).boutonCentre[0]=(int)(0.06738*hauteur); // CLoison
	(*commandes).boutonCentre[1]=(int)(0.11485*hauteur); // Trou
	(*commandes).boutonCentre[2]=(int)(0.164*hauteur); // Démon
	(*commandes).boutonCentre[3]=(int)(0.212*hauteur); // Supprim.

	(*commandes).boutonCentre[4]=(int)(0.324*hauteur); // 224 Arrêt
	(*commandes).boutonCentre[5]=(int)(0.362*hauteur); // 259 1 T.
	(*commandes).boutonCentre[6]=(int)(0.403*hauteur); // 294 2 T.

	(*commandes).boutonCentre[7]=(int)(0.483*hauteur); // 359	Marche
	(*commandes).boutonCentre[8]=(int)(0.527*hauteur); // 395	Arrêt

	(*commandes).boutonCentre[9]=(int)(0.626339969*hauteur); // 481	Marche
	(*commandes).boutonCentre[10]=(int)(0.670*hauteur); // 516	Arrêt

	(*commandes).boutonCentre[11]=(int)(0.763*hauteur); //		Pause
	(*commandes).boutonCentre[12]=(int)(0.803*hauteur); //	Min
	(*commandes).boutonCentre[13]=(int)(0.844*hauteur); //	Max

	(*commandes).boutonCentre[14]=(int)(0.901*hauteur); //	Initialise
	(*commandes).boutonCentre[15]=(int)(0.947*hauteur); //	Paramètre

	//	PANNEAU BAS

		 // Zone du panneau
	(*commandes).bas=(int)(0.8538*hauteur); // 590/691
/*
		// COMPTEUR LINEAIRES SUIVANT Y
	(*commandes).lineaireY=(int)(0.0392156862745*hauteur); // 693-636/2 =  / 714 // Rayon suivant Y
	(*commandes).lineairesCentre=(int)(0.958181818182*hauteur); // 693+636/2 =  / 714 // Position Y des boutons linéaires
		// COMPTEUR LINEAIRES SUIVANT X
	(*commandes).lineaireX=(int)(0.0306563039724*largeur); // 332-261/2 / 1158	Rayon suivant X
	(*commandes).lineaireCentre[0]=(int)(0.20067264574*largeur); // 261	Hauteur
	(*commandes).lineaireCentre[1]=(int)(0.20067264574*largeur); // 332	Distance
	(*commandes).lineaireCentre[2]=(int)(0.257847533632*largeur); // 414+596/2	Simulation < temps réèl
	(*commandes).lineaireCentre[3]=(int)(0.406950672646*largeur); // 414+596/2	Simulation > temps réèl	
*/
		// BOUTONS TRIANGULAIRES SUIVANT Y
	(*commandes).triangleY=(int)(0.017*hauteur); // Rayon suivant Y 
	(*commandes).trianglesCentre=(int)(0.92185*hauteur);  // Position Y des petits triangles
	(*commandes).trianglesLumiere=(int)(0.96093*hauteur);  // Position Y des lumière
		// BOUTONS TRIANGULAIRES SUIVANT X
	(*commandes).triangleX=(int)(0.012*largeur); // Rayon suivant X
	(*commandes).triangleCentre[0]=(int)(0.037996546*largeur); // 44	1
	(*commandes).triangleCentre[1]=(int)(0.066493955*largeur); // 78	2
	(*commandes).triangleCentre[2]=(int)(0.094991364*largeur); // 110 	3

	(*commandes).triangleCentre[3]=(int)(0.150259067*largeur); // 174	1
	(*commandes).triangleCentre[4]=(int)(0.177892919*largeur); // 207	2
	(*commandes).triangleCentre[5]=(int)(0.207253886*largeur); // 240	3
	(*commandes).triangleCentre[6]=(int)(0.236614853*largeur); // 274	4

	(*commandes).triangleCentre[7]=(int)(0.315198618*largeur); // 364	pause
	(*commandes).triangleCentre[8]=(int)(0.355785838*largeur); // 412	Min
	(*commandes).triangleCentre[9]=(int)(0.387737478*largeur); // 450	Moins
	(*commandes).triangleCentre[10]=(int)(0.417962003*largeur); // 483	Plus
	(*commandes).triangleCentre[11]=(int)(0.450777202*largeur); // 	Max


		//	Vitesse de la simulation

	(*commandes).a=(0.450777202-0.355785838)*largeur/(DUREE_MAX-1);
	(*commandes).b=0.355785838*largeur-(*commandes).a;

	return 0;
	}

int commandesInitialiseSouris(commandesT * commandes, int sourisX, int sourisY)
	{
		// Position de la souris
	(*commandes).sourisX = sourisX; // position X de la souris
	(*commandes).sourisY = sourisY; // position Y de la souris

		 // Rayon d'action de la souris
	int rayonX=(*commandes).boutonX;
	int rayonY=(*commandes).boutonY;
	if(sourisX>(*commandes).rotatifs)
		{
		if(sourisX<(*commandes).boutons)// Zone des boutons rotatifs
			{
			rayonX=(*commandes).rotatifX;
			rayonY=(*commandes).rotatifY;
			}
		}
	else 
		{
		if(sourisY>(*commandes).bas) // Zone du bas
			{
			rayonX=(*commandes).triangleX;
			rayonY=(*commandes).triangleY;
			}
		}

		// Zone d'action de la souris
	(*commandes).sourisGauche = sourisX-rayonX; // position X de la souris - RayonBoutonX
	(*commandes).sourisDroite = sourisX+rayonX; // position X de la souris + RayonBoutonX
	(*commandes).sourisHaut = sourisY-rayonY; // position Y de la souris - RayonBoutonY
	(*commandes).sourisBas = sourisY+rayonY; // position Y de la souris + RayonBoutonY

	return 0;
	}

int commandeBoutons(commandesT * commandes)
	{
	int i;
	if((*commandes).boutonsCentre>(*commandes).sourisGauche && (*commandes).boutonsCentre<(*commandes).sourisDroite)
		{
		for(i=0;i<BOUTON_COMMANDES;i++)
			{
			if((*commandes).boutonCentre[i]>(*commandes).sourisHaut && (*commandes).boutonCentre[i]<(*commandes).sourisBas)
				return i;
			}
		}
	return -1;
	}

int commandeRotatifs(commandesT * commandes)
	{
	int i;
	if((*commandes).rotatifsCentre>(*commandes).sourisGauche && (*commandes).rotatifsCentre<(*commandes).sourisDroite)
		{
		for(i=0;i<ROTATIF_COMMANDES;i++)
			{
			if((*commandes).rotatifCentre[i]>(*commandes).sourisHaut && (*commandes).rotatifCentre[i]<(*commandes).sourisBas)
				return i;
			}
		}
	return -1;
	}

int commandeTriangles(commandesT * commandes)
	{
	int i;
	if((*commandes).trianglesCentre>(*commandes).sourisHaut && (*commandes).trianglesCentre<(*commandes).sourisBas)
		{
		for(i=0;i<TRIANGLE_COMMANDES;i++)
			{
			if((*commandes).triangleCentre[i]>(*commandes).sourisGauche && (*commandes).triangleCentre[i]<(*commandes).sourisDroite)
				return i;
			}
		}
	return -1;
	}

int commandeLineaires(commandesT * commandes)
	{
	int i;
	if((*commandes).lineairesCentre>(*commandes).sourisHaut && (*commandes).lineairesCentre<(*commandes).sourisBas)
		{
		for(i=0;i<LINEAIRE_COMMANDES;i++)
			{
			if((*commandes).lineaireCentre[i]>(*commandes).sourisGauche && (*commandes).lineaireCentre[i]<(*commandes).sourisDroite)
				return i;
			}
		}
	return -1;
	}

//////////////////////////////////////////////////////////////////
