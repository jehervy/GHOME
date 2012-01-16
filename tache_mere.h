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
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique
#define NB 1 		// Nombre de sémaphores élémentaires

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

	//Gestion des signaux
	//Ignorance du signal SIGUSR2 pour l'initialisation de la mère et des tâches filles
	struct sigaction action;

	action.sa_handler = SIG_IGN;
	sigemptyset ( &action.sa_mask);
	action.sa_flags = 0;
	sigaction ( SIGUSR2, &action, NULL);

	//Initialisation de l'application
	InitialiserApplication(XTERM);
	 
	//Création des ressources utilisées pour l'application Gestion d'un Carrefour :
	//Construction de la clé publique externe à partir de REFERENCE
	//Les droits d'accès sur les ressources sont donnés par DROITS

	//Création de la boite aux lettres pour la gestion des voitures	
	int boiteVoitureID = msgget (ftok (REFERENCE, '1'), IPC_CREAT | DROITS );

	//Création du sémaphore semEtatFeu 
	int semEtatFeu = semget (ftok (REFERENCE, '2'),NB, IPC_CREAT | DROITS);

	//Initialisation du sémaphore élémentaire n°0 du sémaphore semEtatFeu
	//Au départ, la ressource critique mémoire partagée est libre, donc la 
	//valeur initiale est 1
	semctl ( semEtatFeu, 0 , SETVAL , 1 );

	//Création de la mémoire partagée shmEtatFeu pour la gestion des états des feux
	//Instanciation d'une structure structure_EtatFeu et
	//Attachement de la mémoire partagée shmEtatFeu
	structure_EtatFeu EtatFeu;
	int shmEtatFeu = shmget (ftok (REFERENCE, '3'),sizeof(EtatFeu), IPC_CREAT | DROITS);
	void * ptr_memoireEtatFeu = shmat (shmEtatFeu, NULL, 0);
	
	//mise a jour des valeur en mémoire partagée. ptr_memoireEtatFeu est un pointeur de void,
	//on le caste pour qu'il devienne un pointeur de structure_EtatFeu
	EtatFeu.feuNS=false;
	EtatFeu.feuEO=false;
	*((structure_EtatFeu*)ptr_memoireEtatFeu)=EtatFeu;

	//Création du sémaphore semTempo avec un seul sémaphore élémentaire
	int semTempo = semget (ftok (REFERENCE, '4'),NB, IPC_CREAT | DROITS);

	//Initialisation du sémaphore élémentaire n°0 du sémaphore semTempo
	//Au départ, la ressource critique mémoire partagée est libre, donc la 
	//valeur initiale est 1
	semctl ( semTempo, 0 , SETVAL , 1 );

	//Création de la mémoire partagée shmTempo pour la gestion des durées des feux
	//Instanciation d'une structure structure_Tempo et
	//Attachement de la mémoire partagée shmTempo
	structure_Tempo Tempo;
	int shmTempo = shmget (ftok (REFERENCE, '5'),sizeof(Tempo), IPC_CREAT | DROITS);
	void * ptr_memoireTempo = shmat (shmTempo, NULL, 0);
	
	//Allocation de valeurs aux variables de la structure structure_Tempo
	Tempo.dureeNS = 23;
	Tempo.dureeEO = 12;
	//mise a jour des valeur en mémoire partagée. ptr_memoireTempo est un pointeur de void,
	//on le caste pour qu'il devienne un pointeur de structure_Tempo
	*((structure_Tempo*)ptr_memoireTempo)= Tempo;
	
	//Création des taches filles
	
	pid_t noHeure = ActiverHeure();
	pid_t noGenerateur = ActiverGenerateur(0,boiteVoitureID);
	//pid_t noFeux;
	//pid_t noVoieN;
	//pid_t noVoieS;
	//pid_t noVoieE;
	//pid_t noVoieO;
	//pid_t noCommande;

	//Positionnement du handler FinTache sur le signal SIGUSR2
	/*action.sa_handler = finTache;
	sigaction (SIGUSR2, &action, NULL);*/

	//=====================================
	//PHASE DE DESTRUCTION DE LA TACHE MERE
	//=====================================

	//waitpid(noCommande, NULL,0); // attente de la fin de la tache commande
	sleep(77);
	
	//Destruction des tâches
	/*kill (noVoieO,SIGUSR2);
	kill (noVoieE,SIGUSR2);
	kill (noVoieS,SIGUSR2);
	kill (noVoieN,SIGUSR2);
	kill (noFeux,SIGUSR2);*/
	kill (noHeure,SIGUSR2);
	kill (noGenerateur, SIGCONT);
	kill (noGenerateur, SIGUSR2);

	//Detachement des mémoires partagées
	shmdt(ptr_memoireEtatFeu);
	shmdt(ptr_memoireTempo);

	//Destruction des ressources : mémoires partagées, sémaphores et boite aux lettres
	
	shmctl(shmEtatFeu, IPC_RMID,0);
	semctl(semEtatFeu, 0,IPC_RMID,0);
	shmctl(shmTempo,IPC_RMID,0);
	semctl(semTempo, 0,IPC_RMID,0);
	msgctl(boiteVoitureID,IPC_RMID,0);

	TerminerApplication (true);
	
	//Terminaison de la tâche mère avec une autodestruction
	exit(0);

}
