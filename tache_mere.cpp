/*************************************************************************
                           MERE.cpp  -  description
 
*************************************************************************/

//---------- Réalisation de la tâche <MERE> (fichier MERE.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

#include <pthread.h> 	
#include <sys/msg.h>	//pour la boite aux lettres
//#include <sys/sem.h>	//pour les sémaphore
#include <sys/shm.h>	//pour les mémoires partagées



//------------------------------------------------------ Include personnel


///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

#define DROITS 0660 	// Droits d'accès
//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées


//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
int main()
// Algorithme :
//
{	//========================================
	// PHASE D'INITIALISATION DE LA TACHE MERE
	//========================================



	int sensorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );


	
	//Création des taches filles
	
	//pthread_create( , attribut, routine, argument )
	


	//=====================================
	//PHASE DE DESTRUCTION DE LA TACHE MERE
	//=====================================
	
	//pthread_join( thread, résultat )	
	


	

	//Destruction des ressources 

	msgctl(sensorServerBox,IPC_RMID,0);
	msgctl(actuatorServerBox,IPC_RMID,0);

	
	//Terminaison de la tâche mère avec une autodestruction
	pthread_exit(0);

}
