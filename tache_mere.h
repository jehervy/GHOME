/*************************************************************************
                           MERE.cpp  -  description

*************************************************************************/

#include <iostream>

using namespace std;

#include <pthread.h>
#include <sys/msg.h>	//pour la boite aux lettres
#include <sys/sem.h>	//pour les sémaphore
#include <sys/shm.h>	//pour les mémoires partagées
#include "ServerPere/ServerPere.h"
#include "ServerPere/SensorBoxReader.h"
#include "ServerPere/GhomeDatabase.h"


#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique
#define NB 1 		// Nombre de sémaphores élémentaires

