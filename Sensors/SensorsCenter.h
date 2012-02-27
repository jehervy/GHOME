/*
 * SensorsCenter.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef SENSORSCENTER_H_
#define SENSORSCENTER_H_

#include <map>
#include <utility>
#include <pthread.h>
#include "EnOceanSensorModel.h"

/*
 * Map mettant en relation idVirtuel et pièce et métrique
 */
typedef std::map<int, std::pair<int,int> > mapSensors;

/*
 * Class SensorsCenter
 * Elle centralise les données haut niveau issues des capteurs, les traite
 * avant de les passer dans le moteur d'inférence puis d'envoyer les données
 * et les ordres de pilotage au serveur central
 */
class SensorsCenter{
	public:
		/*
		 * Constructeur
		 * Prend en paramètre la boîte aux lettre du serveur principal et le chemin
		 * du fichier xml mettant en relation idVirtuel et pièce et métrique
		 */
		SensorsCenter(int a_iBalServer, const char* a_pXmlFile);

		/*
		 * Destructeur
		 */
		virtual ~SensorsCenter();

		/*
		 * Méthode permettant de lancer le thread principal du sensorCenter
		 */
		void Start();

		/*
		 * Méthode permettant de suspendre le thread principal du sensorCenter
		 */
		void Stop();

		/*
		 * Getters nécessaires pour les tests
		 */
#ifdef TESTING
		int GetBalServer(){return m_iBalServer;}
		int GetBalModel(){return m_iBalModel;}
		mapSensors GetMapSensors(){return m_sensors;}
		mapSensorInfo GetMapSensorInfo(){return m_pModel->GetMapSensorInfo();}
#endif

	private:

		/*
		 * Méthode permettant de parser le fichier xml
		 * mettant en relation idVirtuel et pièce et métrique
		 */
		void ParserXML(const char* a_pXmlFile);

		/*
		 * Thread principal du SensorsCenter, il attend un message dans une boîte aux lettres
		 * puis vas traduire le message pour qu'il soit interprétable à haut niveau.
		 * Le message sera envoyé au serveur central via une voite aux lettres après être passé dans
		 * le moteur d'inférence.
		 */
		void Run();

		/*
		 * Callback permettant de lancer le thread
		 */
		static void *sCallback(void *a_pCxt)
		{
			((SensorsCenter*)a_pCxt)->Run();
			return 0;
		}

		// ATTRIBUTS
		int m_iBalServer;
		int m_iBalModel;

		pthread_t m_thread;
		mapSensors m_sensors;
		EnOceanSensorModel *m_pModel;


};

#endif /* SENSORSCENTER_H_ */
