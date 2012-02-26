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
#include "../Utils/SystemLog.h"
#include <errno.h>


class ServerPere {
public:
	/*
	 * Constructeur
	 */
	ServerPere();

	/*
	 * Constructeur
	 */
	ServerPere(int a_iSensorServerBox, int a_iActuatorServerBox);

	/*
	 * Destructeur
	 */
	virtual ~ServerPere();

	/*
	 * Cree un thread gérant le socket de communication
	 * entre le serveur C++ et le client Web (Apache).
	 * Appelle une methode static.
	 */
	int Start();


	/*
	 * Cree une instance de classe de communication_client
	 * a chaque nouvelle connexion.
	 * Est appelée dans un nouveau thread.
	 */
	void *CreateCommClient();


	/*
	 * Methode static permettant l'appel de
	 * createCommClient dans un nouveau
	 * thread.
	 */
	static void  *sCreateCommClientCallBack(void* a_pPtr);


	/*
	 * Crée un socket serveur, et attend les
	 * connexions de clients.
	 * Gère la création de file_descriptor.
	 */
	void *OpenSocket();


	/*
	 * Methode static permettant l'appel de
	 * open_socket dans un nouveau thread.
	 */
	static void *sOpenSocketCallBack(void* a_pPtr);

	/*
	 * Ajoute un nouveau file_descriptor
	 * a la connexion d'un nouveau client
	 * dans le vector.
	 */
	int InsertFd(int fd);

	/*
	 * Supprime le file_descriptor,
	 * lorsque qu'un client se deconnecte,
	 * de vector.
	 */
    int DeleteFd(int fd);

    /*
     * Methode bloquante sur la fin du
     * thread de gestion des connexions
     * clients.
     */
    void Wait();

    /*
     * Ferme le socket.
     * Tue le thread de gestion des connexions
     * clients.
     */
	void Stop();

	/*
	 * Met à jour l'état du socket
	 */
	void SetOpened(bool a_bEtat);

	/*
	 * Cherche un File Descriptor dans
	 * le vector
	 */
	int GetFd(int a_iPosition);

	/*
	 * Port d'ecoute du socket
	 */
	static const int SERVER_PERE_PORT = 3000;

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
