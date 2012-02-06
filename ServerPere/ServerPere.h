/*
 * serverpere.h
 *
 *  Created on: 16 janv. 2012
 *      Author: remi
 */

#ifndef SERVERPERE_H_
#define SERVERPERE_H_
#include <pthread.h>
#include "CommunicationClient.h"
#include <vector>


class ServerPere {
public:
	ServerPere();
	/*
	 * Constructeur
	 */
	ServerPere(int a_iSensorServerBox, int a_iActuatorServerBox);
	/*
	 * Constructeur
	 * Appel la methode d'ouverture de socket dans un
	 * nouveau thread
	 */
	virtual ~ServerPere();
	/*
	 * Destructeur
	 */
	int Start();
	/*
	 * Crée un thread gérant le socket de communication
	 * entre le serveur C++ et le client Web (Apache).
	 * Appelle une methode static.
	 */
	void *CreateCommClient();
	/*
	 * Cree une instance de classe de communication_client
	 * a chaque nouvelle connexion.
	 * Est appelée dans un nouveau thread.
	 */
	static void  *sCreateCommClientCallBack(void* a_pPtr);
	/*
	 * Methode static permettant l'appel de
	 * createCommClient dans un nouveau
	 * thread.
	 */
	void *OpenSocket();
	/*
	 * Crée un socket serveur, et attend les
	 * connexions de clients.
	 * Gère la création de file_descriptor.
	 */
	static void *sOpenSocketCallBack(void* a_pPtr);
	/*
	 * Methode static permettant l'appel de
	 * open_socket dans un nouveau thread.
	 */
     int InsertFd(int fd);
    /*
     * Ajoute un nouveau file_descriptor
     * a la connexion d'un nouveau client
     * dans le vector.
     */
     int DeleteFd(int fd);
    /*
     * Supprime le file_descriptor,
     * lorsque qu'un client se deconnecte,
     * de vector.
     */
	void Wait();
	/*
	 * Methode bloquante sur la fin du
	 * thread de gestion des connexions
	 * clients.
	 */
	 void Stop();
	/*
	 * Ferme le socket.
	 * Tue le thread de gestion des connexions
	 * clients.
	 */
	 void SetOpened(bool a_bEtat);
	 /*
	  * Met à jour l'état du socket
	  */


private :
	pthread_t m_ptThreadCommClient;
    int m_iSockfd;
    bool  m_bSocketOpened;
	int m_iSensorServerBox;
	int m_iActuatorServerBox;
	int m_iPFileDescriptor;
	int m_iNbConnection;
};

#endif /* SERVERPERE_H_ */
