/*
 * DataContext.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef DATACONTEXT_H_
#define DATACONTEXT_H_

#define MSGSIZE 255

// Structure networkInfo nécessaire pour paramétrer les "abonnements"
struct networkInfo{
	char address[MSGSIZE];	// Adresse du serveur
	int port;			// Port du serveur
	int bal;			// Référence de la boîte aux lettres
};

// Structure balMessage nécessaire pour les messages envoyés via la boîte aux lettres
struct balMessage{
	long mtype;			// Type du message > 1
	char mtext[MSGSIZE];	// Message
};

class DataContext{

public:
	// Cette fonction permet de "s'abonner" à un flux d'informations.
	// Elle prend comme paramètre une structure networkInfo
	// Elle attendra la réception d'un message par le socket et l'écrira dans une boîte aux lettre
	// quand celui ci arrivera.
	static void* sRcvData(void* a_pArgs);

	// Cette fonction permet d'envoyer des données en continu à un serveur.
	// Elle prend en paramètre une structure networkInfo
	// Elle attendra un message dans la file de messages et l'enverra au serveur
	// quand celui ci arrivera.
	static void* sSndData(void* a_pArgs);
};



#endif /* DATACONTEXT_H_ */
