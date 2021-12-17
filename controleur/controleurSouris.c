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

#include "../controleur/controleurSouris.h"

int controleurSourisDefileCommandes(controleurT * controleur, int zone);

int controleurSourisMouvement(controleurT * controleur)
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

void controleurSourisBouton(controleurT * controleur, int appui)
	{

	(*controleur).appui=appui;
	
	if(appui==1)
		{
		if((*controleur).commandes.sourisX>(*controleur).commandes.rotatifs)
			{
			if((*controleur).commandes.sourisX>(*controleur).commandes.boutons)
				{
				if(controleurSourisCommandes(controleur, 2)==1) controleurPostReinitialisation(controleur);
				//controleurSourisCommandes(controleur, 2);
				}
			else
				{
				controleurSourisCommandes(controleur, 1);
				}
			}
		else
			{
			if((*controleur).commandes.sourisY>(*controleur).commandes.bas)
				{
				controleurSourisCommandes(controleur, 3);
				}
			else
				{
				controleurSourisCommandes(controleur, 0);
				}
			}
		}
	return;
	}

int controleurSourisCommandes(controleurT * controleur, int zone)
	{
	int commande;
	int reinitialisation = 0;
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
				systemeInitialisePosition(&(*controleur).systeme, 0);
				controleurPostReinitialisation(controleur);
				break;
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
	return reinitialisation;
	}

int controleurSourisDefile(controleurT * controleur)
	{
	if((*controleur).commandes.sourisX>(*controleur).commandes.rotatifs)
		{
		controleurSourisDefileCommandes(controleur, 1);
		}
	else
		{
		if((*controleur).commandes.sourisY>(*controleur).commandes.bas)
			{
			controleurSourisDefileCommandes(controleur, 3);
			}
		}
	return 0;
	}

int controleurSourisDefileCommandes(controleurT * controleur, int zone)
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

void controleurSourisAffiche(controleurT * controleur)
	{
	fprintf(stderr, "(*controleur).graphique.hauteur = %d\n", (*controleur).graphique.hauteur);
	fprintf(stderr, "(*controleur).commandes.sourisY = %d\n", (*controleur).commandes.sourisY);
	fprintf(stderr, "(*controleur).graphique.largeur = %d\n", (*controleur).graphique.largeur);
	fprintf(stderr, "(*controleur).commandes.sourisX = %d\n", (*controleur).commandes.sourisX);

	return ;
	}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
