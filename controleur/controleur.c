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

#include "controleur.h"

void controleurChangeMode(controleurT * controleur);
void controleurChangeVitesse(controleurT * controleur, float facteur);

void controleurEvolution(controleurT * controleur);
int controleurProjection(controleurT * controleur);
int controleurEvolutionSysteme(controleurT * controleur);
int controleurConstructionGraphique(controleurT * controleur);
int controleurActionClavier(controleurT * controleur);

int controleurTraiteEvenement(controleurT * controleur);


int controleurActionClavier(controleurT * controleur);
int controleurClavierMaj(controleurT * controleur);
int controleurClavierCtrl(controleurT * controleur);

int controleurCommandes(controleurT * controleur, int zone);
int controleurInitialiseParametres(controleurT * controleur, int forme);
int controleurInitialiseParametresCloison(controleurT * controleur);
int controleurInitialiseParametresTemperature(controleurT * controleur);

int controleurSouris(controleurT * controleur);
int controleurDefile(controleurT * controleur);
int controleurDefilePointDeVue(controleurT * controleur);
int controleurDefileCommandes(controleurT * controleur, int zone);
void controleurBoutonSouris(controleurT * controleur, int appui);
void controleurAfficheSouris(controleurT * controleur);

int controleurClavier(controleurT * controleur);
int controleurClavierMaj(controleurT * controleur);
int controleurClavierCtrl(controleurT * controleur);
int controleurSouris(controleurT * controleur);
void controleurBoutonSouris(controleurT * controleur, int appui);

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
	//printf("Entrée dans controleurEvolution, SDL_GetTicks() = %d\n",(int)(SDL_GetTicks()));

		//fprintf(stderr, "    Durée entre affichage = %d\n",horlogeChronoDuree(&(*controleur).horloge));
	//horlogeChronoDepart(&(*controleur).horloge);

		//fprintf(stderr, "\nProjection du système sur la représentation graphique");
	controleurProjection(controleur);
		//fprintf(stderr, "    Durée = %d\n",horlogeChronoDuree(&(*controleur).horloge));

	if((*controleur).modePause < 0)
		{
			//horlogeChronoDepart(&(*controleur).horloge);
			//fprintf(stderr, "Evolution temporelle du systeme");
			controleurEvolutionSysteme(controleur);
			//fprintf(stderr, "    Durée = %d\n",horlogeChronoDuree(&(*controleur).horloge));
		}

	//horlogeChronoDepart(&(*controleur).horloge);

		//fprintf(stderr, "Mise à jour de la fenêtre graphique");
	controleurConstructionGraphique(controleur);
		//fprintf(stderr, "    Durée = %d\n",horlogeChronoDuree(&(*controleur).horloge));

	//horlogeChronoDepart(&(*controleur).horloge);
	//fprintf(stderr, "    Durée des évolutions = %d\n",horlogeChronoDuree(&(*controleur).horloge));
	//printf("Sortie de controleurEvolution, SDL_GetTicks() = %d\n",(int)(SDL_GetTicks()));
		//fprintf(stderr, "    Durée entre affichage = %d\n",horlogeChronoDuree(&(*controleur).horloge));

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
			controleurDefile(controleur);break;
		case SDL_MOUSEMOTION:
			sortie = controleurSouris(controleur);break;
		case SDL_MOUSEBUTTONDOWN:
			controleurBoutonSouris(controleur, 1);break;
		case SDL_MOUSEBUTTONUP:
			controleurBoutonSouris(controleur, 0);break;
		case SDL_USEREVENT:
			controleurEvolution(controleur);break;
		case SDL_KEYDOWN:
			{
			if ((((*controleur).interface.evenement.key.keysym.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
			|| (((*controleur).interface.evenement.key.keysym.mod & KMOD_RSHIFT) == KMOD_RSHIFT))
				{
				sortie = controleurClavierMaj(controleur);break;
				}
			else
				{
				if ((((*controleur).interface.evenement.key.keysym.mod & KMOD_LCTRL) == KMOD_LCTRL)
				|| (((*controleur).interface.evenement.key.keysym.mod & KMOD_RCTRL) == KMOD_RCTRL))
					{
					sortie = controleurClavierCtrl(controleur);break;
					}
				else
					{
					sortie = controleurClavier(controleur);break;
					}
				}
			}
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

int controleurClavier(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{

	// Sortie

		case SDLK_ESCAPE:
			(*controleur).sortie = 1;break;

    // Mode : attente d'un evenement / pas d'attente

		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		case SDLK_BACKSPACE:
			controleurChangeMode(controleur);break;

	// Vitesse de la simulation
		case SDLK_KP_PLUS:
			controleurChangeVitesse(controleur, 1.1);break;
		case SDLK_KP_MINUS:
			controleurChangeVitesse(controleur, 0.91);break;
		case SDLK_F9:
			controleurChangeVitesse(controleur, -1.0);break;
			//(*controleur).duree=1;break;
		case SDLK_F10:
			controleurChangeVitesse(controleur, 0.91);break;
		case SDLK_F11:
			controleurChangeVitesse(controleur, 1.1);break;
		case SDLK_F12:
			controleurChangeVitesse(controleur, -10.0);break;
			//(*controleur).duree=DUREE_MAX;break;

	// Diametre des particules

		case SDLK_e:
			systemeChangeDiametre(&(*controleur).systeme, 1.1);break;
		case SDLK_d:
			systemeChangeDiametre(&(*controleur).systeme, 0.9);break;

	// Taille du trou

		case SDLK_a:
			montageChangeTrou(&(*controleur).systeme.montage, 1.1);break;
		case SDLK_q:
			montageChangeTrou(&(*controleur).systeme.montage, 0.9);break;
		case SDLK_z:
			montageChangeTrou(&(*controleur).systeme.montage, -2);break;
		case SDLK_s:
			montageChangeTrou(&(*controleur).systeme.montage, -1);break;

		case SDLK_w:	//	Pas de cloison central
			//systemeChangeCloison(&(*controleur).systeme, 0);
	montageChangeParoiCentrale(&(*controleur).systeme.montage, 0);break;
		case SDLK_x:	//	Cloison fermée
			//systemeChangeCloison(&(*controleur).systeme, 1);
	montageChangeParoiCentrale(&(*controleur).systeme.montage, 0);break;
		case SDLK_c:	//	Cloison percée
			//systemeChangeCloison(&(*controleur).systeme, 2);
	montageChangeParoiCentrale(&(*controleur).systeme.montage, 0);break;
		case SDLK_v:	//	cloison percée et démon de Maxwell
			montageChangeDemonMaxwell(&(*controleur).systeme.montage);break;
		case SDLK_b:	//	Démon de Maxwell
			//systemeChangeCloison(&(*controleur).systeme, -1);
	montageChangeDemonMaxwell(&(*controleur).systeme.montage); break;
		case SDLK_n:	//	Trou max et démon de Maxwell
			//systemeChangeCloison(&(*controleur).systeme, -2);
			break;

	// Thermostat

		case SDLK_o:	//	Système isolé
			thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 0);break;
		case SDLK_i:	//	Système thermostaté symétrique
			thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 1);break;
		case SDLK_k:	//	Système  thermostaté gauche-droite
			thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 2);break;
		case SDLK_l:	//	Erreur
			thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 3);break;

		case SDLK_p:	//	Valeur de la température
			thermostatChangeTemperature(&(*controleur).systeme.montage.thermostat, 1.7);break;
		case SDLK_m:	//	
			thermostatChangeTemperature(&(*controleur).systeme.montage.thermostat, 0.6);break;

		case SDLK_u:	//	Température à droite
			thermostatChangeTemperatureDroite(&(*controleur).systeme.montage.thermostat,1.9);break;
		case SDLK_j:	//	Température à droite
			thermostatChangeTemperatureDroite(&(*controleur).systeme.montage.thermostat,0.51);break;
		case SDLK_y:	//	Température à gauche
			thermostatChangeTemperatureGauche(&(*controleur).systeme.montage.thermostat,1.9);break;
		case SDLK_h:	//	Température à gauche
			thermostatChangeTemperatureGauche(&(*controleur).systeme.montage.thermostat,0.51);break;

		case SDLK_t:	//	Thermostat à droite
			thermostatChangeEtatDroite(&(*controleur).systeme.montage.thermostat, -1);break;
		case SDLK_g:	//	Thermostat à gauche
			thermostatChangeEtatGauche(&(*controleur).systeme.montage.thermostat, -1);break;


  // Initialiser le système

		case SDLK_F1:
			systemeInitialisePosition(&(*controleur).systeme, 1);
			break;

  // Afficher les informations

		case SDLK_F4: // Corde asymétrique
			controleurAfficheSouris(controleur);
			break;
		case SDLK_F5:	//	
			observablesAffiche(&(*controleur).observables);
			break;
		case SDLK_F6:	//	
			thermostatAfficheThermostat(&(*controleur).systeme.montage.thermostat);
			break;
		case SDLK_F7:	//	
			projectionAffiche(&(*controleur).projection);
			break;

		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurClavierMaj(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{

	// Sortie

		case SDLK_ESCAPE:
			(*controleur).sortie = 1;break;

    // Mode : attente d'un evenement / pas d'attente

		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		case SDLK_BACKSPACE:
			controleurChangeMode(controleur);break;


	// Réinitialisation du système
		// Lecture des fichier
		case SDLK_a:
			fprintf(stderr, "Réinitialisation du système\n");
			systemeInitialisePosition(&(*controleur).systeme, 1);break;
	/*	case SDLK_z:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 1);break;
		case SDLK_e:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 2);break;
			fichierFonction(&(*controleur).systeme, 2);break;//Triangle
		case SDLK_r:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 3);break;
			fichierFonction(&(*controleur).systeme, 3);break;//Triangle
		case SDLK_t:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 4);break;
			fichierFonction(&(*controleur).systeme, 4);break;//Triangle
		case SDLK_y:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 5);break;
			fichierFonction(&(*controleur).systeme, 5);break;//Triangle
		case SDLK_u:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 6);break;
			fichierFonction(&(*controleur).systeme, 6);break;//Triangle
		case SDLK_i:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 7);break;
			fichierFonction(&(*controleur).systeme, 7);break;//Triangle


		case SDLK_q:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 10);break;
		case SDLK_s:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 11);break;
		case SDLK_d:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 12);break;
		case SDLK_f:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 13);break;
		case SDLK_g:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 14);break;
		case SDLK_h:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 15);break;
		case SDLK_j:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 16);break;
		case SDLK_k:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*controleur).systeme, 17);break;

		case SDLK_o:
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 8);break;
		case SDLK_p:
			//fprintf(stderr, "Réinitialisation du système\n");
			controleurChangeSensibilite(&(*controleur), 1.1);break;
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 9);break;
		case SDLK_l:
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 18);break;
		case SDLK_m:
			controleurChangeSensibilite(&(*controleur), 0.91);break;
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*controleur).systeme, 19);break;


		// Ecriture des fichiers
		// Ecriture des fichiers
		case SDLK_w:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 10);break;
		case SDLK_x:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 11);break;
		case SDLK_c:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 12);break;
		case SDLK_v:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 13);break;
		case SDLK_b:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 14);break;
		case SDLK_n:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 15);break;
		case SDLK_d:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*controleur).systeme, );break;
		case SDLK_f:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*controleur).systeme, );break;
		case SDLK_g:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*controleur).systeme, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*controleur).systeme, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*controleur).systeme, );break;*/

		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurClavierCtrl(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{
	// Sortie
		case SDLK_ESCAPE:
			(*controleur).sortie = 1;break;
	// Mode : attente d'un evenement / pas d'attente
		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		case SDLK_BACKSPACE:
			controleurChangeMode(controleur);break;

	// Enregistrement
		// Sauvegarde du système fichierEcriture sans effet
	/*	case SDLK_a:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 0);break;
		case SDLK_z:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 1);break;
		case SDLK_e:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 2);break;
		case SDLK_r:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 3);break;
		case SDLK_t:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 4);break;
		case SDLK_y:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 5);break;
		case SDLK_u:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 6);break;
		case SDLK_i:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 7);break;
		case SDLK_o:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 8);break;
		case SDLK_p:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 9);break;
		case SDLK_q:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 10);break;
		case SDLK_s:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 11);break;
		case SDLK_d:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 12);break;
		case SDLK_f:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 13);break;
		case SDLK_g:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 14);break;
		case SDLK_h:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 15);break;


		// Ecriture des fichiers
		case SDLK_w:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 10);break;
		case SDLK_x:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 11);break;
		case SDLK_c:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 12);break;
		case SDLK_v:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 13);break;
		case SDLK_b:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 14);break;
		case SDLK_n:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, 15);break;
		case SDLK_d:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, );break;
		case SDLK_f:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, );break;
		case SDLK_g:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*controleur).systeme, );break;*/

		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurSouris(controleurT * controleur)
	{
	float x, y;
	if((*controleur).appui==1)
		{
		x=-0.01*(float)((*controleur).interface.evenement.motion.xrel);
		y=0.1*HAUTEUR*(float)((*controleur).interface.evenement.motion.yrel);
		//fprintf(stderr, "controleurSouris yrel = %d , x = %f\n", (*controleur).evenement.motion.yrel, x);
		//fprintf(stderr, "controleurSouris xrel = %d , y = %f\n", (*controleur).evenement.motion.xrel, y);
		(void)x;(void)y;
		}
	return (*controleur).sortie;
	}

void controleurBoutonSouris(controleurT * controleur, int appui)
	{

	(*controleur).appui=appui;
	
	if(appui==1)
		{
		if((*controleur).commandes.sourisX>(*controleur).commandes.rotatifs)
			{
			if((*controleur).commandes.sourisX>(*controleur).commandes.boutons)
				{
				controleurCommandes(controleur, 2);
				}
			else
				{
				controleurCommandes(controleur, 1);
				}
			}
		else
			{
			if((*controleur).commandes.sourisY>(*controleur).commandes.bas)
				{
				controleurCommandes(controleur, 3);
				}
			else
				{
				controleurCommandes(controleur, 0);
				}
			}
		}
	return;
	}

int controleurCommandes(controleurT * controleur, int zone)
	{
	int commande;
	if(zone==2)
		{
		commande = commandeBoutons(&(*controleur).commandes);
		switch(commande)	//	
			{// 0 : Pas de cloison, 1 : Cloison fermée, 2 : Cloison percée, -1 : Maxwell
			case 0: // Cloison
				//systemeChangeCloison(&(*controleur).systeme, 1);
			montageChangeParoiCentrale(&(*controleur).systeme.montage, 1);break;
			case 1: // Trou
				//systemeChangeCloison(&(*controleur).systeme, 2);
			montageChangeParoiCentrale(&(*controleur).systeme.montage, 2);break;
			case 2: // Démon
				montageChangeDemonMaxwell(&(*controleur).systeme.montage);break;
			case 3: // Supprim.
				//systemeChangeCloison(&(*controleur).systeme, 0);
		// Changement du montage
	montageChangeParoiCentrale(&(*controleur).systeme.montage, 0);break;

			case 4:
				thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 0);break;
			case 5:
				thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 1);break;
			case 6:
				thermostatChangeEtat(&(*controleur).systeme.montage.thermostat, 2);break;

			case 7: // Marche
				thermostatChangeEtatGauche(&(*controleur).systeme.montage.thermostat, 1);break;
			case 8: // Arrêt
				thermostatChangeEtatGauche(&(*controleur).systeme.montage.thermostat, 0);break;
			case 9: // Marche
				thermostatChangeEtatDroite(&(*controleur).systeme.montage.thermostat, 1);break;
			case 10: // Arrêt
				thermostatChangeEtatDroite(&(*controleur).systeme.montage.thermostat, 0);break;
			case 11:
				controleurChangeMode(controleur);break;
			case 12: // 
				controleurChangeVitesse(controleur, -1.0);break;
			case 13: // 
				controleurChangeVitesse(controleur, -10);break;
			case 14: // 
				systemeInitialisePosition(&(*controleur).systeme, 0);break;
			case 15: // 
				controleurInitialiseParametres(controleur, 3);break;
			case 16: // 
				;break;
			default:
				;
			}
		}
	if(zone==3)
		{
		commande = commandeTriangles(&(*controleur).commandes);
		switch(commande)	//	
			{
			case 0:
				systemeInitialisePosition(&(*controleur).systeme, 0);break;
			case 1:
				systemeInitialisePosition(&(*controleur).systeme, 1);break;
			case 2:
				systemeInitialisePosition(&(*controleur).systeme, 2);break;
			case 3:
				controleurInitialiseParametres(controleur, 0);break;
			case 4:
				controleurInitialiseParametres(controleur, 1);break;
			case 5:
				controleurInitialiseParametres(controleur, 2);break;
			case 6:
				controleurInitialiseParametres(controleur, 3);break;
			case 7:
				controleurChangeMode(controleur);break;
			case 8:
				controleurChangeVitesse(controleur, -1.0);break;
			case 9:
				controleurChangeVitesse(controleur, 0.91);break;
			case 10:
				controleurChangeVitesse(controleur, 1.1);break;
			case 11:
				controleurChangeVitesse(controleur, -10);break;
		/*	case 12:
				systemeInitialisePosition(&(*controleur).systeme, 1);break;
			case 13:
				systemeInitialisePosition(&(*controleur).systeme, 2);break;
			case 14:
				systemeInitialisePosition(&(*controleur).systeme, 4);break;
			case 15:
				systemeInitialisePosition(&(*controleur).systeme, 5);break;
			case 16:
				systemeInitialisePosition(&(*controleur).systeme, 6);break;
			case 17:
				controleurInitialiseParametres(controleur, 0);break;
			case 18:
				controleurInitialiseParametres(controleur, 1);break;
			case 19:
				controleurInitialiseParametres(controleur, 2);break;
			case 20:
				controleurInitialiseParametres(controleur, 3);break;
		*/	default:
				;
			}
		}
	return 0;
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


int controleurDefile(controleurT * controleur)
	{
	if((*controleur).commandes.sourisX>(*controleur).commandes.rotatifs)
		{
		controleurDefileCommandes(controleur, 1);
		}
	else
		{
		if((*controleur).commandes.sourisY>(*controleur).commandes.bas)
			{
			controleurDefileCommandes(controleur, 3);
			}
		}
	return 0;
	}

int controleurDefileCommandes(controleurT * controleur, int zone)
	{
	int commande;
	if(zone==1)
		{
		commande = commandeRotatifs(&(*controleur).commandes);
		if((*controleur).interface.evenement.wheel.y > 0) // scroll up
			{
			switch(commande)
				{
				case 0:
					montageChangeTrou(&(*controleur).systeme.montage, 1.1);break;
				case 1:
					systemeChangeDiametre(&(*controleur).systeme, 1.1);break;
				case 2:	//	Valeur de la température
					thermostatChangeTemperature(&(*controleur).systeme.montage.thermostat, 1.1);break;
				case 3:	//	Température à gauche
					thermostatChangeTemperatureGauche(&(*controleur).systeme.montage.thermostat,1.1);break;
				case 4:	//	Température à droite
					thermostatChangeTemperatureDroite(&(*controleur).systeme.montage.thermostat,1.1);break;
				case 5:	//	Simulation
					controleurChangeVitesse(controleur, 1.1);break;
				case 6:	//	Nombre
					systemeChangeNombre(&(*controleur).systeme, 1);break;
				default:
					;
				}
			}
		else if((*controleur).interface.evenement.wheel.y < 0) // scroll down
			{
			switch(commande)	
				{
				case 0:
					montageChangeTrou(&(*controleur).systeme.montage, 0.91);break;
				case 1:
					systemeChangeDiametre(&(*controleur).systeme, 0.91);break;
				case 2:
					thermostatChangeTemperature(&(*controleur).systeme.montage.thermostat, 0.91);break;
				case 3:
					thermostatChangeTemperatureGauche(&(*controleur).systeme.montage.thermostat, 0.91);break;
				case 4:
					thermostatChangeTemperatureDroite(&(*controleur).systeme.montage.thermostat, 0.91);break;
				case 5:	//	Simulation
					controleurChangeVitesse(controleur, 0.91);break;
				case 6:	//	Nombre
					systemeChangeNombre(&(*controleur).systeme, -1);break;
				default:
					;
				}
			}
		}

	if(zone==3)
		{
		commande = commandeLineaires(&(*controleur).commandes);
		if((*controleur).interface.evenement.wheel.y > 0) // scroll up
			{
			switch(commande)
				{
				default:
					;
				}
			}
		else if((*controleur).interface.evenement.wheel.y < 0) // scroll down
			{
			switch(commande)	
				{
				default:
					;
				}
			}
		}
	return 0;
	}

void controleurAfficheSouris(controleurT * controleur)
	{
	fprintf(stderr, "(*controleur).graphique.hauteur = %d\n", (*controleur).graphique.hauteur);
	fprintf(stderr, "(*controleur).commandes.sourisY = %d\n", (*controleur).commandes.sourisY);
	fprintf(stderr, "(*controleur).graphique.largeur = %d\n", (*controleur).graphique.largeur);
	fprintf(stderr, "(*controleur).commandes.sourisX = %d\n", (*controleur).commandes.sourisX);

	return ;
	}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
