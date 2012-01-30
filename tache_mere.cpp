/*************************************************************************
                           MERE.cpp  -  description

*************************************************************************/

//---------- R�alisation de la t�che <MERE> (fichier MERE.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include syst�me


#include <sys/msg.h>	//pour la boite aux lettres
//#include <sys/sem.h>	//pour les s�maphore
#include <sys/shm.h>	//pour les m�moires partag�es
#include "tache_mere.h"
#include "ServerPere/sensor_box_reader.h"
#include "ServerPere/ghome_database.h"


//------------------------------------------------------ Include personnel


///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

#define DROITS 0660 	// Droits d'acc�s
//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions priv�es


//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
