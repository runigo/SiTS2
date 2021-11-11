/*
Copyright novembre 2021, Stephan Runigo
runigo@free.fr
SiTS 2.3.2  simulateur de thermodynamique statistique
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

#include "observables.h"

int observablesMiseAJourAmplitudes(observablesT * observables);
float observablesMiseAJourAmplitude(observableT * observable);

int observablesMiseAJourNombre(observablesT * observables, systemeT * systeme);
int observablesMiseAJourLibreParcoursMoyen(observablesT * observables, systemeT * systeme);
int observablesMiseAJourTemperature(observablesT * observables, systemeT * systeme);

int observablesMiseAJourPression(observablesT * observables, systemeT * systeme);
int observablesMiseAJourDensite(observablesT * observables, systemeT * systeme);

float observablesAbsolue(float valeur);

float observablesAbsolue(float valeur)
	{
	if(valeur<0) return -valeur;
	return valeur;
	}

int observablesInitialise(observablesT * observables)
	{
	int i, j;
	for(j=0;j<CAPTEURS;j++)
		{
		(*observables).observable[j].dureeCapteur=DUREE_CAPTEURS;
		for(i=0;i<DUREE_CAPTEURS;i++)
			{
			(*observables).observable[j].gauche[i]=0;
			(*observables).observable[j].droite[i]=0;
			}
		}

	(*observables).observable[5].dureeCapteur=DY_ENERGIE;

	(*observables).index=0;

	return 0;
	}

float observablesMiseAJourAmplitude(observableT * observable)
	{
				// Calcul le maximum d'un capteur
	int i;
	float max;

	max = 0.0;
	for(i=0;i<DUREE_CAPTEURS;i++)
		{
		if((*observable).gauche[i] > max)
			{
			max = (*observable).gauche[i];
			}
		if((*observable).droite[i] > max)
			{
			max = (*observable).droite[i];
			}
		}
	(*observable).maximumCapteur = max;

	return max;
	}

int observablesMiseAJourAmplitudes(observablesT * observables)
	{
				// Mise à jour des maximum des capteurs
	int j;

	for(j=0;j<CAPTEURS;j++)
		{
		observablesMiseAJourAmplitude(&(*observables).observable[j]);
		}
	return 0;
	}

int observablesMiseAJour(observablesT * observables, systemeT * systeme)
	{
						// Évolution de l'index

	(*observables).index ++;
	if((*observables).index==DUREE_CAPTEURS)
		{
		(*observables).index=0;
		}

						// Mise à jour des observables

	observablesMiseAJourNombre(observables,systeme);

	observablesMiseAJourLibreParcoursMoyen(observables, systeme);

	observablesMiseAJourTemperature(observables, systeme);

	observablesMiseAJourPression(observables, systeme);

	observablesMiseAJourDensite(observables, systeme);

	//observablesMiseAJourAmplitudes(observables);

	return 0;
	}

int observablesMiseAJourNombre(observablesT * observables, systemeT * systeme)
	{
						// Calcul du nombre de particules à gauche et à droite
	int i;
	int nbGauche=0;
	int nbDroite=0;

	for(i=0;i<(*systeme).nombre;i++)
		{
		if((*systeme).mobile[i].droite==0)
			{
			nbGauche ++;
			}
		else
			{
			nbDroite ++;
			}
		}

	(*observables).observable[1].gauche[(*observables).index]=nbGauche;
	(*observables).observable[1].droite[(*observables).index]=nbDroite;
	
	observablesMiseAJourAmplitude(&(*observables).observable[1]);

	return 0;
	}

int observablesMiseAJourTemperature(observablesT * observables, systemeT * systeme)
	{
						// Calcul de la température à gauche et à droite
	int i;
	double ecGauche=0.0;
	double ecDroite=0.0;

	for(i=0;i<(*systeme).nombre;i++)
		{
		if((*systeme).mobile[i].droite==0)
			{
			ecGauche=ecGauche+mobileEnergieCinetique(&(*systeme).mobile[i]);
			}
		else
			{
			ecDroite=ecDroite+mobileEnergieCinetique(&(*systeme).mobile[i]);
			}
		}

		// Mise à jour de l'énergie
	(*observables).observable[3].gauche[(*observables).index]=ecGauche;
	(*observables).observable[3].droite[(*observables).index]=ecDroite;

		// Mise à jour de la température
	if((*observables).observable[1].gauche[(*observables).index]!=0.0)
		{
		(*observables).observable[0].gauche[(*observables).index]=ecGauche/(*observables).observable[1].gauche[(*observables).index];
		}
	else
		{
		(*observables).observable[0].gauche[(*observables).index]=ecGauche;
		}

	if((*observables).observable[1].droite[(*observables).index]!=0.0)
		{
		(*observables).observable[0].droite[(*observables).index]=ecDroite/(*observables).observable[1].droite[(*observables).index];
		}
	else
		{
		(*observables).observable[0].droite[(*observables).index]=ecDroite;
		}

	observablesMiseAJourAmplitude(&(*observables).observable[3]);
	observablesMiseAJourAmplitude(&(*observables).observable[0]);

	return 0;
	}

int observablesMiseAJourLibreParcoursMoyen(observablesT * observables, systemeT * systeme)
	{
						// Calcul du libre parcours moyen à gauche et à droite
	int i;
	double lpmGauche=0.0;
	double lpmDroite=0.0;

	for(i=0;i<(*systeme).nombre;i++)
		{
		if((*systeme).mobile[i].droite==0)
			{
			lpmGauche = lpmGauche + (*systeme).mobile[i].lpm;
			}
		else
			{
			lpmDroite = lpmDroite + (*systeme).mobile[i].lpm;
			}
		}

	if((*observables).observable[1].gauche[(*observables).index]!=0.0)
		{
		(*observables).observable[2].gauche[(*observables).index]=lpmGauche/(*observables).observable[1].gauche[(*observables).index];
		}
	else
		{
		(*observables).observable[2].gauche[(*observables).index]=lpmGauche;
		}

	if((*observables).observable[1].droite[(*observables).index]!=0.0)
		{
		(*observables).observable[2].droite[(*observables).index]=lpmDroite/(*observables).observable[1].droite[(*observables).index];
		}
	else
		{
		(*observables).observable[2].droite[(*observables).index]=lpmDroite;
		}

	observablesMiseAJourAmplitude(&(*observables).observable[2]);

	return 0;
	}

int observablesMiseAJourPression(observablesT * observables, systemeT * systeme)
	{
						// Calcul de la pression à gauche et à droite
	int i;
	float pressionGauche=0.0;
	float pressionDroite=0.0;

	for(i=0;i<(*systeme).nombre;i++)
		{
		//if(observablesAbsolue((*systeme).mobile[i].actuel.x) < DY_PRESSION)
			{
			if((*systeme).mobile[i].droite==0)
				{
				pressionGauche = pressionGauche + observablesAbsolue(((*systeme).mobile[i].actuel.y)-((*systeme).mobile[i].ancien.y));
				//pressionGauche = pressionGauche + observablesAbsolue(((*systeme).mobile[i].nouveau.y)-((*systeme).mobile[i].actuel.y));
				}
			else
				{
				pressionDroite = pressionDroite + observablesAbsolue(((*systeme).mobile[i].actuel.y)-((*systeme).mobile[i].ancien.y));
				//pressionDroite = pressionDroite + observablesAbsolue(((*systeme).mobile[i].actuel.y)-((*systeme).mobile[i].ancien.y));
				}
			}
		}

	(*observables).observable[4].gauche[(*observables).index]=pressionGauche;

	(*observables).observable[4].droite[(*observables).index]=pressionDroite;

	observablesMiseAJourAmplitude(&(*observables).observable[4]);

	return 0;
	}

int observablesMiseAJourDensite(observablesT * observables, systemeT * systeme)
	{
						// Calcul de la fonction de répartition de l'énergie
	int i;
	int index = 0;
	float a;

		// Maximum de l'énergie des particules -> maximum de l'axe oy
	float max = 0.0;
	for(i=0;i<(*systeme).nombre;i++)
		{
		if((*systeme).mobile[i].ec > max)
			{
			max = (*systeme).mobile[i].ec;
			}
		}

		// Remise à zéro
	for(i=0;i<DY_ENERGIE;i++)
		{
		(*observables).observable[5].gauche[i]=0.0;
		(*observables).observable[5].droite[i]=0.0;
		}

		// Coeficient directeur
	if(max!=0.0)
		{
		a = DY_ENERGIE / max ;
		}
	else
		{
		a = DY_ENERGIE ;
		}

		// Calcul des densités
	for(i=0;i<(*systeme).nombre;i++)
		{
		index = (int) (a * (*systeme).mobile[i].ec);
		if(index < DY_ENERGIE && index >= 0)
			{
			if((*systeme).mobile[i].droite==0)
				{
				(*observables).observable[5].gauche[index] = (*observables).observable[5].gauche[index] + 1;
				}
			else
				{
				(*observables).observable[5].droite[index] = (*observables).observable[5].droite[index] + 1;
				}
			}
		}

	observablesMiseAJourAmplitude(&(*observables).observable[5]);

	return 0;
	}


int observablesAffiche(observablesT * observables)
	{
	printf("\nSTATISTIQUES\n");

	printf("	temperature à gauche %f \n", (*observables).observable[0].gauche[(*observables).index]);
	printf("	temperature à droite %f \n\n", (*observables).observable[0].droite[(*observables).index]);
	printf("	nombre à gauche %f \n", (*observables).observable[1].gauche[(*observables).index]);
	printf("	nombre à droite %f \n\n", (*observables).observable[1].droite[(*observables).index]);
	printf("	libre parcours moyen à gauche %f \n", (*observables).observable[2].gauche[(*observables).index]);
	printf("	libre parcours moyen à droite %f \n\n", (*observables).observable[2].droite[(*observables).index]);
	return 0;
	}

