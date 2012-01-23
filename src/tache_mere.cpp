/*************************************************************************
                           MERE.cpp  -  description
 
*************************************************************************/

//---------- Réalisation de la tâche <MERE> (fichier MERE.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système


#include <sys/msg.h>	//pour la boite aux lettres
//#include <sys/sem.h>	//pour les sémaphore
#include <sys/shm.h>	//pour les mémoires partagées
#include "tache_mere.h"
#include <stdio.h>



//------------------------------------------------------ Include personnel


///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

#define DROITS 0666 	// Droits d'accès
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

	cout << "Test" << endl;

	int sensorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	  cout <<"message queue" << sensorServerBox <<  "created\n";


	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );


	//Creation des taches filles

	//createServerPere(sensorServerBox, actuatorServerBox);

	cout << "demande de creation de thread"<<endl;
	
	//pthread_t thread_sensor_box_reader;
	sensor_box_reader tache_sensor_box_reader(sensorServerBox, actuatorServerBox);

	//cout << "Tache construite"<<endl;

	//=====================================
	//PHASE DE DESTRUCTION DE LA TACHE MERE
	//=====================================
	tache_sensor_box_reader.wait();

    cout << "destruction" <<endl;
	

	//Destruction des ressources 
    msgctl(sensorServerBox,IPC_RMID,0);
	msgctl(actuatorServerBox,IPC_RMID,0);

	
	//Terminaison de la tâche mère avec une autodestruction
	pthread_exit(0);

	return 0;

}

/*void createServerPere(int sensorServerBox, int actuatorServerBox)
{
	cout << "Test classe" << endl;
	server_pere papa(sensorServerBox, actuatorServerBox);
	cout << "Classe reussie" << endl;
}*/
