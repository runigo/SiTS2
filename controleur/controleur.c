/*
Copyright décembre 2019, Stephan Runigo
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

#include "controleur.h"
#include "controleurClavier.h"
#include "controleurSouris.h"

void controleurEvolution(controleurT * controleur);
int controleurProjection(controleurT * controleur);
int controleurEvolutionSysteme(controleurT * controleur);
int controleurConstructionGraphique(controleurT * controleur);
int controleurActionClavier(controleurT * controleur);

int controleurTraiteEvenement(controleurT * controleur);

int controleurKEYDOWN(controleurT * controleur);

int controleurInitialiseParametresCloison(controleurT * controleur);
int controleurInitialiseParametresTemperature(controleurT * controleur);

void controleurChangeMode(controleurT * controleur);
void controleurChangeVitesse(controleurT * controleur, float facteur);

int controleurInitialise(controleurT * controleur)
	{

	(*controleur).duree = (*controleur).options.duree;	// nombre d'évolution du système par affichage

	(*controleur).modePause = -1;	// Évolution du système si < 0
	(*controleur).sortie = 0;	// Sortie de SiCP si > 0
	(*controleur).appui = 0;	// Appuie sur la souris

		fprintf(stderr, " Initialisation de la projection\n");
	projectionInitialise(&(*controleur).projection);

		fprintf(stderr, " Initialisation du système\n");
		// Initialisation géométrique de l'enceinte
	(*controleur).systeme.montage.largeur = LARGEUR; // Largeur
	(*controleur).systeme.montage.hauteur = HAUTEUR;// Hauteur
	(*controleur).systeme.montage.demiLargeur = LARGEUR/2; // Demi largeur
	(*controleur).systeme.montage.demiHauteur = HAUTEUR/2;// Demi hauteur
	(*controleur).systeme.montage.paroiCentrale = (*controleur).options.cloison; 
	(*controleur).systeme.montage.demonMaxwell = 0;// 0 : pas de démon de Maxwell
	(*controleur).systeme.montage.trou = (*controleur).options.trou;// Rayon du trou, sur 2

		// Initialisation du thermostat
	(*controleur).systeme.montage.thermostat.uniforme = (*controleur).options.temperature;//	Température initiale
	(*controleur).systeme.montage.thermostat.gauche = (*controleur).options.gauche;	//	Température thermostat gauche
	(*controleur).systeme.montage.thermostat.droite = (*controleur).options.droite;	//	Température thermostat droite
	(*controleur).systeme.montage.thermostat.actif = (*controleur).options.thermostat;	//	0 : Système isolé, 1:température uniforme, 2:active gauche-droite

		// Initialisation du système
	systemeInitialise(&(*controleur).systeme, (*controleur).options.nombre, (*controleur).options.diametre, sqrt((*controleur).options.temperature)/2);

		// Initialisation des observables
	observablesInitialise(&(*controleur).observables);

		// Initialisation du graphe
	grapheInitialise(&(*controleur).graphe);

		// Initialisation des capteurs
	capteursInitialise(&(*controleur).capteurs);

		//fprintf(stderr, " Initialisation SDL\n");
	interfaceInitialisationSDL();
		//fprintf(stderr, " Création de l'interface SDL\n");
	interfaceInitialisation(&(*controleur).interface);
		//fprintf(stderr, " Création du rendu\n");
	fprintf(stderr, " graphiqueCreation = %d\n",graphiqueCreation(&(*controleur).graphique, &(*controleur).interface));

		//fprintf(stderr, " Initialisation des longueurs\n");
	int largeur;
	int hauteur;
	SDL_GetWindowSize((*controleur).interface.fenetre, &largeur, &hauteur);
	(*controleur).graphique.largeur=largeur;
	(*controleur).graphique.hauteur=hauteur;

	capteursMiseAJourLongueur(&(*controleur).capteurs, largeur, hauteur);

	commandesInitialiseBoutons(&(*controleur).commandes, largeur, hauteur);

	grapheMiseAJourLongueur(&(*controleur).graphe, largeur, hauteur);

	//projectionInitialiseLongueurs(&(*controleur).projection, largeur, hauteur);

		// Initialisation des commandes de la souris
	int x, y;
	SDL_PumpEvents();
	SDL_GetMouseState(&x,&y);
	commandesInitialiseSouris(&(*controleur).commandes, x, y);

		//fprintf(stderr, " Initialisation horloge SDL\n");
	horlogeCreation(&(*controleur).horloge);

	return 0;
	}

int controleurDestruction(controleurT * control)
	{
	fprintf(stderr, "Suppression de l'horloge\n");
	horlogeSuppression(&(*control).horloge);

	fprintf(stderr, "Suppression du rendu\n");
	graphiqueDestruction(&(*control).graphique);

	fprintf(stderr, "Suppression de la fenêtre\n");
	interfaceDestruction(&(*control).interface);

	fprintf(stderr, "Sortie de la SDL\n");
	interfaceQuitteSDL();

	return 0;
	}


int controleurSimulationGraphique(controleurT * controleur)
	{
		//fprintf(stderr, "Entrée dans la boucle SDL\n");
	do	{
		controleurActionClavier(controleur);
		}
	while((*controleur).sortie == 0);

	return 0;
	}

int controleurActionClavier(controleurT * controleur)
	{
		//fprintf(stderr, "Prise en compte des actions clavier\n");
	if (SDL_WaitEvent(&(*controleur).interface.evenement))
		{
		controleurTraiteEvenement(controleur);
		}
	return (*controleur).sortie;
	}

void controleurEvolution(controleurT * controleur)
	{

	//horlogeChrono(&(*controleur).horloge, 0);

	controleurProjection(controleur);

	//horlogeChrono(&(*controleur).horloge, 1);

	if((*controleur).modePause < 0)
		{
			controleurEvolutionSysteme(controleur);
		}

	//horlogeChrono(&(*controleur).horloge, 2);

	controleurConstructionGraphique(controleur);

	//horlogeChrono(&(*controleur).horloge, 3);

	return;
	}

int controleurProjection(controleurT * controleur)
	{
		//fprintf(stderr, "Projection du système sur la représentation graphique\n");


		// Réinitialisation des longueurs
	int largeur;
	int hauteur;
	SDL_GetWindowSize((*controleur).interface.fenetre, &largeur, &hauteur);
	if((*controleur).graphique.largeur!=largeur || (*controleur).graphique.hauteur!=hauteur)
		{	// Réinitialisation des longueurs si la fenêtre change de taille
		(*controleur).graphique.largeur=largeur;
		(*controleur).graphique.hauteur=hauteur;
		commandesInitialiseBoutons(&(*controleur).commandes, largeur, hauteur);
		capteursMiseAJourLongueur(&(*controleur).capteurs, largeur, hauteur);
		}
	grapheMiseAJourLongueur(&(*controleur).graphe, (*controleur).graphique.largeur, (*controleur).graphique.hauteur);

		// Réinitialisation des commandes de la souris
	int x, y;
	SDL_PumpEvents();
	SDL_GetMouseState(&x,&y);
	commandesInitialiseSouris(&(*controleur).commandes, x, y);


		// Projection du système sur le graphique
	projectionSystemeGraphe(&(*controleur).systeme, &(*controleur).projection, &(*controleur).graphe);

		// Projection du système sur les commandes
	projectionSystemeCommandes(&(*controleur).systeme, &(*controleur).projection, &(*controleur).commandes, (*controleur).duree, (*controleur).modePause);

		// Projection des observables sur les capteurs
	projectionObservablesCapteurs(&(*controleur).observables, &(*controleur).projection, &(*controleur).capteurs);

	return (*controleur).sortie;
	}

int controleurEvolutionSysteme(controleurT * controleur)
	{
		// Évolution du système
	systemeEvolue(&(*controleur).systeme, (*controleur).duree);

		// Mise à jour des observables
	observablesMiseAJour(&(*controleur).observables, &(*controleur).systeme);

	return 0;
	}

int controleurConstructionGraphique(controleurT * controleur)
	{

		//fprintf(stderr, "Nettoyage de l'affichage\n");
	//graphiqueNettoyage(&(*controleur).graphique);

		//fprintf(stderr, "Dessin des Commandes\n");
	graphiqueCommandes(&(*controleur).graphique, &(*controleur).commandes);

		//fprintf(stderr, "Dessin des graphes\n");
	graphiqueDessineGraphe(&(*controleur).graphique, &(*controleur).graphe);

		//fprintf(stderr, "Dessin des capteurs\n");
	graphiqueCapteurs(&(*controleur).graphique, &(*controleur).capteurs);

		//fprintf(stderr, "Mise à jour de l'affichage\n");
	graphiqueMiseAJour(&(*controleur).graphique);

	return (*controleur).sortie;
	}

int controleurTraiteEvenement(controleurT * controleur)
	{
	int sortie = 0;
	switch((*controleur).interface.evenement.type)
		{
		case SDL_QUIT:
			sortie = 1;break;
		case SDL_MOUSEWHEEL:
			controleurSourisDefile(controleur);break;
		case SDL_MOUSEMOTION:
			sortie = controleurSourisMouvement(controleur);break;
		case SDL_MOUSEBUTTONDOWN:
			controleurSourisBouton(controleur, 1);break;
		case SDL_MOUSEBUTTONUP:
			controleurSourisBouton(controleur, 0);break;
		case SDL_USEREVENT:
			controleurEvolution(controleur);break;
		case SDL_KEYDOWN:
			controleurKEYDOWN(controleur);break;
		default:
			;
		}

	if(sortie != 0)
		{
		fprintf(stderr, "sortie <> 0\n");
		(*controleur).sortie = 1;
		}
	return sortie;
	}

int controleurKEYDOWN(controleurT * controleur)
	{
	int Maj = 0;
	int ctrl = 0;

	if ((((*controleur).interface.evenement.key.keysym.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
	|| (((*controleur).interface.evenement.key.keysym.mod & KMOD_RSHIFT) == KMOD_RSHIFT))
		{
		Maj = 1;
		}
	if ((((*controleur).interface.evenement.key.keysym.mod & KMOD_LCTRL) == KMOD_LCTRL)
	|| (((*controleur).interface.evenement.key.keysym.mod & KMOD_RCTRL) == KMOD_RCTRL))
		{
		ctrl = 1;
		}
	if(Maj == 0 && ctrl == 0)
		{
		return controleurClavier(controleur);
		}
	else
		{
		if(Maj == 1 && ctrl == 1)
			{
			return controleurClavierCtrlMaj(controleur);
			}
		else
			{
			if(Maj == 1 )
				{
				return controleurClavierMaj(controleur);
				}
			else
				{
				return controleurClavierCtrl(controleur);
				}
			}
		}

	return (*controleur).sortie;
	}

void controleurChangeMode(controleurT * controleur)
	{
	(*controleur).modePause=-(*controleur).modePause;

	return;
	}

void controleurChangeVitesse(controleurT * controleur, float facteur)
	{
	if(facteur > 0.0)
		{
		if( (*controleur).duree >= DUREE_MAX && facteur > 1 )
			{
			printf("Duree maximale atteinte. ");
			}
		else
			{
			if( (*controleur).duree > 11 )
				{
				(*controleur).duree = (*controleur).duree * facteur;
				}
			else
				{
				if( facteur > 1)
					{
					(*controleur).duree ++;
					}
				else
					{
					if( (*controleur).duree > 1 )
						{
						(*controleur).duree --;
						}
					else
						{
						printf("Duree minimale atteinte. ");
						}
					}
				}
			}
		}
	else
		{
		if(facteur > -5.0)
			{
			(*controleur).duree=1;
			}
		else
			{
			(*controleur).duree=DUREE_MAX;
			}
		}
	if( (*controleur).duree > DUREE_MAX)
			{
			(*controleur).duree=DUREE_MAX;
			}
	printf("Duree = %d\n", (*controleur).duree);
	return;
	}

int controleurInitialiseParametresCloison(controleurT * controleur)
	{
	//systemeChangeCloison(&(*controleur).systeme, 0);
		montageChangeParoiCentrale(&(*controleur).systeme.montage, 0);
	montageSupprimeDemon(&(*controleur).systeme.montage);
	(*controleur).systeme.montage.trou = RAYON_TROU; // Rayon implicite du trou dans la cloison

	return 0;
	}

int controleurInitialiseParametresTemperature(controleurT * controleur)
	{

	(*controleur).systeme.montage.thermostat.uniforme = sqrt(TEMPERATURE_MAX * TEMPERATURE_MIN ); //	Température moyenne du thermostat
	(*controleur).systeme.montage.thermostat.gauche = TEMPERATURE_MIN + TEMPERATURE_MAX/5;	//	Température gauche
	(*controleur).systeme.montage.thermostat.droite = sqrt(TEMPERATURE_MAX * TEMPERATURE_MIN );	//	Température droite
	(*controleur).systeme.montage.thermostat.actif = 0;	//	0 : Système isolé, 1:température uniforme, 2:active gauche-droite
	(*controleur).systeme.montage.thermostat.etatDroite = 0;		//	0: isolé à droite, 1:températures droite
	(*controleur).systeme.montage.thermostat.etatGauche = 0;		//	0: isolé à gauche, 1:température gauche

	return 0;
	}

int controleurInitialiseParametres(controleurT * controleur, int forme)
	{
	switch(forme)
		{
		case 0:
			controleurInitialiseParametresCloison(controleur);break;
		case 1:
			controleurInitialiseParametresCloison(controleur);
			//systemeChangeCloison(&(*controleur).systeme, 2);
			montageChangeParoiCentrale(&(*controleur).systeme.montage, 2);
			montageChangeDemonMaxwell(&(*controleur).systeme.montage);break;
		case 2:
			controleurInitialiseParametresTemperature(controleur);
			thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 1);
			break;
		case 3:
			controleurInitialiseParametresTemperature(controleur);
			thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 2);
			thermostatChangeEtatGauche(&(*controleur).systeme.montage.thermostat, 1);
			thermostatChangeEtatDroite(&(*controleur).systeme.montage.thermostat, 1);
			break;
		default:
			;
		}
	return 0;
	}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
