/*************************************************************************
                           MERE.cpp  -  description
 
*************************************************************************/

//---------- Réalisation de la tâche <MERE> (fichier MERE.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

#include <iostream>
using namespace std;

#include <pthread.h> 	
#include <sys/msg.h>	//pour la boite aux lettres
//#include <sys/sem.h>	//pour les sémaphore
#include <sys/shm.h>	//pour les mémoires partagées
#include "server_pere.h"


//------------------------------------------------------ Include personnel


///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes
#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique
#define NB 1 		// Nombre de sémaphores élémentaires

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées


//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void createServerPere();
