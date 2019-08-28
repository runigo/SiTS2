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

#include "graphique.h"

int graphiqueDestruction(graphiqueT * graphique)
	{
	SDL_DestroyRenderer((*graphique).rendu);
	return 0;
	}


int graphiqueCreation(graphiqueT * graphique, interfaceT * interface)
	{

	int largeur;
	int hauteur;
	SDL_GetWindowSize((*interface).fenetre, &largeur, &hauteur);
	(*graphique).largeur=largeur;
	(*graphique).hauteur=hauteur;

		// Création du rendu
	(*graphique).rendu = SDL_CreateRenderer((*interface).fenetre, -1 , 
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(NULL == (*graphique).rendu)
		{
		(*graphique).rendu = SDL_CreateRenderer((*interface).fenetre, -1 , 
						SDL_RENDERER_SOFTWARE);
		if(NULL == (*graphique).rendu)
			{
			fprintf(stderr, "ERREUR interfaceInitialisation : Erreur SDL_CreateRenderer : %s \n", SDL_GetError());
			return EXIT_FAILURE;
			}
		}

	(*graphique).orange.r = 255; (*graphique).orange.g = 127; (*graphique).orange.b = 40; (*graphique).orange.a = 255;
	(*graphique).orangeF.r = 198; (*graphique).orangeF.g = 8; (*graphique).orangeF.b = 0; (*graphique).orangeF.a = 255;

	(*graphique).gris.r = 127; (*graphique).gris.g = 127; (*graphique).gris.b = 127; (*graphique).gris.a = 255;
	(*graphique).grisF.r = 27; (*graphique).grisF.g = 27; (*graphique).grisF.b = 27; (*graphique).grisF.a = 255;

	(*graphique).cyan.r = 127; (*graphique).cyan.g = 40; (*graphique).cyan.b = 255; (*graphique).cyan.a = 255;

	(*graphique).vert.r = 173; (*graphique).vert.g = 255; (*graphique).vert.b = 47; (*graphique).vert.a = 255;
	(*graphique).vertF.r = 0; (*graphique).vertF.g = 86; (*graphique).vertF.b = 27; (*graphique).vertF.a = 255;
	//(*graphique).vertF.r = 27; (*graphique).vertF.g = 79; (*graphique).vertF.b = 8;

	(*graphique).aubergine.r = 55; (*graphique).aubergine.g = 0; (*graphique).aubergine.b = 40; (*graphique).aubergine.a = 255;

		//(253, 63, 146) Fuschia
		//(255, 0, 255) Magenta
	(*graphique).gauche.r = 253;
	(*graphique).gauche.g = 63;
	(*graphique).gauche.b = 146;
	(*graphique).gauche.a = 255;

		//(52, 201, 36) Pomme
	(*graphique).droite.r = 52;
	(*graphique).droite.g = 201;
	(*graphique).droite.b = 36;
	(*graphique).droite.a = 255;


	int retour = 0;

	SDL_Surface *image = 0;

	image = SDL_LoadBMP("particule.bmp");
	if (!image)
		{
		fprintf(stderr,"Erreur chargement image, particule.bmp : %s\n",SDL_GetError());
		retour = 0;
		}
	(*graphique).particule = SDL_CreateTextureFromSurface((*graphique).rendu, image);
	SDL_FreeSurface(image);
	if ((*graphique).particule == 0)
		{
		fprintf(stderr,"grapheInitialisation : Erreur creation texture : %s\n",SDL_GetError());
		retour = 0;
		}

		// Activation de la transparence
	//SDL_BLENDMODE_NONE || SDL_BLENDMODE_BLEND || SDL_BLENDMODE_ADD || SDL_BLENDMODE_MOD
	if(SDL_SetTextureBlendMode((*graphique).particule, SDL_BLENDMODE_MOD) < 0)
		fprintf(stderr, "grapheInitialisation : Erreur SDL_SetRenderDrawBlendMode : %s.", SDL_GetError());

	SDL_Surface *panneau = 0;

	panneau = SDL_LoadBMP("sigp.bmp");
	if (!panneau)
		{
		fprintf(stderr,"ERREUR chargement image, sigp.bmp : %s\n",SDL_GetError());
		retour = 1;
		}
	(*graphique).imageFond = SDL_CreateTextureFromSurface((*graphique).rendu, panneau);
	SDL_FreeSurface(panneau);
	if ((*graphique).imageFond == 0)
		{
		fprintf(stderr,"ERREUR grapheInitialisation : Erreur creation texture sigp.bmp ; %s\n",SDL_GetError());
		retour = 2;
		}

	SDL_Surface *lumiereVerte = 0;

	lumiereVerte = SDL_LoadBMP("lumiereVerte.bmp");
	if (!lumiereVerte)
		{
		fprintf(stderr,"ERREUR chargement image, lumiereVerte.bmp : %s\n",SDL_GetError());
		retour = 5;
		}
	(*graphique).lumiereVerte = SDL_CreateTextureFromSurface((*graphique).rendu, lumiereVerte);
	SDL_FreeSurface(lumiereVerte);
	if ((*graphique).lumiereVerte == 0)
		{
		fprintf(stderr,"ERREUR grapheInitialisation : Erreur creation texture lumiereVerte.bmp ; %s\n",SDL_GetError());
		retour = 6;
		}

	SDL_Surface *lumiereRouge = 0;

	lumiereRouge = SDL_LoadBMP("lumiereRouge.bmp");
	if (!lumiereRouge)
		{
		fprintf(stderr,"ERREUR chargement image, lumiereRouge.bmp : %s\n",SDL_GetError());
		retour = 7;
		}
	(*graphique).lumiereRouge = SDL_CreateTextureFromSurface((*graphique).rendu, lumiereRouge);
	SDL_FreeSurface(lumiereRouge);
	if ((*graphique).lumiereRouge == 0)
		{
		fprintf(stderr,"ERREUR grapheInitialisation : Erreur creation texture lumiereRouge.bmp ; %s\n",SDL_GetError());
		retour = 8;
		}

	SDL_Surface *lumiereJaune = 0;

	lumiereJaune = SDL_LoadBMP("lumiereJaune.bmp");
	if (!lumiereJaune)
		{
		fprintf(stderr,"ERREUR chargement image, lumiereJaune.bmp : %s\n",SDL_GetError());
		retour = 9;
		}
	(*graphique).lumiereJaune = SDL_CreateTextureFromSurface((*graphique).rendu, lumiereJaune);
	SDL_FreeSurface(lumiereJaune);
	if ((*graphique).lumiereJaune == 0)
		{
		fprintf(stderr,"ERREUR grapheInitialisation : Erreur creation texture lumiereJaune.bmp ; %s\n",SDL_GetError());
		retour = 10;
		}

	SDL_Surface *lumiereOrange = 0;

	lumiereOrange = SDL_LoadBMP("lumiereOrange.bmp");
	if (!lumiereOrange)
		{
		fprintf(stderr,"ERREUR chargement image, lumiereOrange.bmp : %s\n",SDL_GetError());
		retour = 11;
		}
	(*graphique).lumiereOrange = SDL_CreateTextureFromSurface((*graphique).rendu, lumiereOrange);
	SDL_FreeSurface(lumiereOrange);
	if ((*graphique).lumiereOrange == 0)
		{
		fprintf(stderr,"ERREUR grapheInitialisation : Erreur creation texture lumiereOrange.bmp ; %s\n",SDL_GetError());
		retour = 12;
		}


	return retour;
}

int graphiqueNettoyage(graphiqueT * graphique)
	{
	SDL_RenderClear((*graphique).rendu);
	return 0;
	}

int graphiqueMiseAJour(graphiqueT * graphique)
	{
	SDL_RenderPresent((*graphique).rendu);
	return 0;
	}

int graphiqueChangeCouleur(graphiqueT * graphique, SDL_Color couleur)
	{
	if(SDL_SetRenderDrawColor((*graphique).rendu, couleur.r, couleur.g, couleur.b, couleur.a) < 0)
		{return -1;}
	return 0;  
	}

int graphiqueCapteurs(graphiqueT * graphique, capteursT * capteurs)
	{
	int i, j;

		// Grandeurs à gauche
	graphiqueChangeCouleur(graphique, (*graphique).gauche);

	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[0].gauche, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[1].gauche, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[2].gauche, DUREE_CAPTEURS);

	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[3].gauche, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[4].gauche, DUREE_CAPTEURS);
	//SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[5].gauche, DY_ENERGIE);

		// Grandeurs à droite
	graphiqueChangeCouleur(graphique, (*graphique).droite);

	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[0].droite, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[1].droite, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[2].droite, DUREE_CAPTEURS);

	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[3].droite, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[4].droite, DUREE_CAPTEURS);
	//SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[5].droite, DY_ENERGIE);


		// Epaississement du trait
	for(j=0;j<CAPTEURS;j++)
		{
		for(i=0;i<DUREE_CAPTEURS;i++)
			{
			(*capteurs).capteur[j].droite[i].y = (*capteurs).capteur[j].droite[i].y + 1;
			}
		}
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[0].droite, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[1].droite, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[2].droite, DUREE_CAPTEURS);

	graphiqueChangeCouleur(graphique, (*graphique).gauche);
	for(j=0;j<CAPTEURS;j++)
		{
		for(i=0;i<DUREE_CAPTEURS;i++)
			{
			(*capteurs).capteur[j].gauche[i].y = (*capteurs).capteur[j].gauche[i].y + 1;
			}
		}
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[0].gauche, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[1].gauche, DUREE_CAPTEURS);
	SDL_RenderDrawLines((*graphique).rendu, (*capteurs).capteur[2].gauche, DUREE_CAPTEURS);

	return 0;
	}

int graphiqueCommandes(graphiqueT * graphique, commandesT * commandes)
	{
		// Image de fond
	SDL_Rect coordonnee = {0, 0, (*graphique).largeur, (*graphique).hauteur};
	SDL_RenderCopy((*graphique).rendu, (*graphique).imageFond, NULL, &coordonnee);
	
		// Commandes sélectionées
	int centrage = 5;
	coordonnee.w=10;
	coordonnee.h=10;
	int i;
	int X, Y, x, y;

	coordonnee.x = (*commandes).boutonsCentre - centrage;
	for(i=0;i<BOUTON_COMMANDES;i++)
		{
		if((*commandes).boutonEtat[i]==1)
			{
			coordonnee.y = (*commandes).boutonCentre[i] - centrage;
			SDL_RenderCopy((*graphique).rendu, (*graphique).particule, NULL, &coordonnee);
			}
		}

		// Position des boutons rotatifs
	graphiqueChangeCouleur(graphique, (*graphique).orange);

	X=(*commandes).rotatifsCentre;
	for(i=0;i<ROTATIF_COMMANDES;i++)
		{
		Y=(*commandes).rotatifCentre[i];
		x=X+(*commandes).rotatifPositionX[i];
		y=Y+(*commandes).rotatifPositionY[i];
		SDL_RenderDrawLine((*graphique).rendu, X-1, Y, x-1, y);
		SDL_RenderDrawLine((*graphique).rendu, X, Y-1, x, y-1);
		SDL_RenderDrawLine((*graphique).rendu, X+1, Y, x+1, y);
		SDL_RenderDrawLine((*graphique).rendu, X, Y+1, x, y+1);
		}

	centrage = 6;
	coordonnee.w=12;
	coordonnee.h=12;
	coordonnee.y = (*commandes).trianglesLumiere - centrage;	// Positon Y de la zone du bas
	for(i=0;i<TRIANGLE_COMMANDES;i++)
		{
		if((*commandes).triangleEtat[i]==1)			//	1 VERT
			{
			coordonnee.x = (*commandes).triangleCentre[i] - centrage; // Positon X des boutons triangulaire
			SDL_RenderCopy((*graphique).rendu, (*graphique).lumiereVerte, NULL, &coordonnee);
			}
		else
			{
			if((*commandes).triangleEtat[i]==2)	//	2 ROUGE
				{
				coordonnee.x = (*commandes).triangleCentre[i] - centrage; // Positon X des boutons triangulaire
				SDL_RenderCopy((*graphique).rendu, (*graphique).lumiereRouge, NULL, &coordonnee);
				}
			else
				{
				coordonnee.x=(*commandes).lineairePositionX;	//	JAUNE ORANGE
				//coordonnee.x=(*commandes).triangleCentre[i];	//	JAUNE ORANGE
				if((*commandes).triangleEtat[9]==-1)
					{
					SDL_RenderCopy((*graphique).rendu, (*graphique).lumiereOrange, NULL, &coordonnee);
					}
				}
			}
		}

	return 0;
	}

void graphiqueDessineGraphe(graphiqueT * graphique, grapheT * graphe)
	{
	int i, x, y;
	int centrage = (*graphe).taille/2; // rayon des particules
	SDL_Rect coordonnee = {0, 0, (*graphe).taille, (*graphe).taille};

		//fprintf(stderr, "Dessin des particules\n");
	for(i=0;i<(*graphe).nombre;i++)
		{
		x=(*graphe).abscisse[i];
		y=(*graphe).ordonnee[i];
		coordonnee.x = x - centrage;
		coordonnee.y = y - centrage;
		SDL_RenderCopy((*graphique).rendu, (*graphique).particule, NULL, &coordonnee);
		}

		//fprintf(stderr, "Dessin du montage\n");
			// Parois horizontales
	graphiqueChangeCouleur(graphique, (*graphique).gauche);
	if(TRAIT_ENCEINTE>2)
		{
		for(i=0;i<TRAIT_ENCEINTE;i++)
			{
			SDL_RenderDrawLine((*graphique).rendu, (*graphe).ax-i, (*graphe).dy-i, (*graphe).bx, (*graphe).dy-i);
			SDL_RenderDrawLine((*graphique).rendu, (*graphe).ax-i, (*graphe).gy+i, (*graphe).bx, (*graphe).gy+i);
			}
		graphiqueChangeCouleur(graphique, (*graphique).droite);
		for(i=0;i<TRAIT_ENCEINTE;i++)
			{
			SDL_RenderDrawLine((*graphique).rendu, (*graphe).bx, (*graphe).dy-i, (*graphe).cx+i, (*graphe).dy-i);
			SDL_RenderDrawLine((*graphique).rendu, (*graphe).bx, (*graphe).gy+i, (*graphe).cx+i, (*graphe).gy+i);
			}
		}
	graphiqueChangeCouleur(graphique, (*graphique).grisF);
	SDL_RenderDrawLine((*graphique).rendu, (*graphe).ax, (*graphe).dy, (*graphe).cx, (*graphe).dy);
	SDL_RenderDrawLine((*graphique).rendu, (*graphe).ax, (*graphe).gy, (*graphe).cx, (*graphe).gy);
	SDL_RenderDrawLine((*graphique).rendu, (*graphe).ax-TRAIT_ENCEINTE+1, (*graphe).dy-TRAIT_ENCEINTE+1, (*graphe).cx+TRAIT_ENCEINTE-1, (*graphe).dy-TRAIT_ENCEINTE+1);
	SDL_RenderDrawLine((*graphique).rendu, (*graphe).ax-TRAIT_ENCEINTE+1, (*graphe).gy+TRAIT_ENCEINTE-1, (*graphe).cx+TRAIT_ENCEINTE-1, (*graphe).gy+TRAIT_ENCEINTE-1);

			// Parois verticales
	graphiqueChangeCouleur(graphique, (*graphique).grisF);
	for(i=0;i<TRAIT_ENCEINTE;i++)
		{
		SDL_RenderDrawLine((*graphique).rendu, (*graphe).ax-i, (*graphe).dy-i, (*graphe).ax-i, (*graphe).gy+i);
		SDL_RenderDrawLine((*graphique).rendu, (*graphe).cx+i, (*graphe).dy-i, (*graphe).cx+i, (*graphe).gy+i);
		}

			// Paroi centrale
	if((*graphe).cloison != 0)
		{
		for(i=(-TRAIT_CLOISON/2);i<TRAIT_CLOISON;i++)
			{
			SDL_RenderDrawLine((*graphique).rendu, (*graphe).bx+i, (*graphe).dy, (*graphe).bx+i, (*graphe).ey);
			SDL_RenderDrawLine((*graphique).rendu, (*graphe).bx+i, (*graphe).fy, (*graphe).bx+i, (*graphe).gy);
			}
		if((*graphe).demon != 0)
			graphiqueChangeCouleur(graphique, (*graphique).orange);
		else
			graphiqueChangeCouleur(graphique, (*graphique).gris);
		for(i=(-TRAIT_CLOISON/2);i<TRAIT_CLOISON;i++)
			{
			SDL_RenderDrawLine((*graphique).rendu, (*graphe).bx+i, (*graphe).ey, (*graphe).bx+i, (*graphe).fy);
			}
		}
	return;
	}

