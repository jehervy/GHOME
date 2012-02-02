/*
 * serverpere.h
 *
 *  Created on: 16 janv. 2012
 *      Author: remi
 */

#ifndef SERVERPERE_H_
#define SERVERPERE_H_
#include <pthread.h>
#include "communication_client.h"
#include <vector>


class server_pere {
public:
	server_pere();
	/*
	 * Constructeur
	 */
	server_pere(int sensorServerBox, int actuatorServerBox);
	/*
	 * Constructeur
	 * Appel la methode d'ouverture de socket dans un
	 * nouveau thread
	 */
	virtual ~server_pere();
	/*
	 * Destructeur
	 */
	int open_thread_comm_client();
	/*
	 * Crée un thread gérant le socket de communication
	 * entre le serveur C++ et le client Web (Apache).
	 * Appelle une methode static.
	 */
	void *createCommClient();
	/*
	 * Cree une instance de classe de communication_client
	 * a chaque nouvelle connexion.
	 * Est appelée dans un nouveau thread.
	 */
	static void  *createCommClient_2(void* ptr);
	/*
	 * Methode static permettant l'appel de
	 * createCommClient dans un nouveau
	 * thread.
	 */
	void *open_socket();
	/*
	 * Crée un socket serveur, et attend les
	 * connexions de clients.
	 * Gère la création de file_descriptor.
	 */
	static void *callback(void* ptr);
	/*
	 * Methode static permettant l'appel de
	 * open_socket dans un nouveau thread.
	 */
     int insert_fd(int fd);
    /*
     * Ajoute un nouveau file_descriptor
     * a la connexion d'un nouveau client
     * dans le vector.
     */
     int delete_fd(int fd);
    /*
     * Supprime le file_descriptor,
     * lorsque qu'un client se deconnecte,
     * de vector.
     */
	void wait();
	/*
	 * Methode bloquante sur la fin du
	 * thread de gestion des connexions
	 * clients.
	 */
	 void kill_thread();
	/*
	 * Ferme le socket.
	 * Tue le thread de gestion des connexions
	 * clients.
	 */
	 void set_opened(bool etat);
	 /*
	  * Met à jour l'état du socket
	  */

    pthread_t thread_comm_client;
    int sockfd;
  	bool  s_opened;
private :
	int p_sensorServerBox;
	int p_actuatorServerBox;
	int p_fd;
	int nb_connection;
};

#endif /* SERVERPERE_H_ */
