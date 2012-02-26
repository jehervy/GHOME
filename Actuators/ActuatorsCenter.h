#ifndef ACTUATORCENTER_H_
#define ACTUATORCENTER_H_

#include <map>
#include <utility>
#include <string>
#include <pthread.h>
#include "EnOceanActuatorModel.h"
#include "../xml/pugixml.hpp"
#include "../config.h"

/*
 * Structure contenant l'ensemble des correspondances metric/room <-> id virtuelle
 */
typedef std::map<std::pair<int,int>,int>  mapActuators;


/*
 * La classe ActuatorCenter centralise les ActuatorModels, les implémentations des capteurs.
 * Elle assure la liaison entre le serveur central et les models.
 * Elle est également responsable de la correspondance (métrique,pièce) <-> id Virtuel
 */
class ActuatorsCenter{
	public:
		ActuatorsCenter(int a_iBalServer, const std::string a_sXmlFile);
		virtual ~ActuatorsCenter();
		void Start();
		void Stop();

#ifdef TESTING
		mapActuators GetActuators();
		int GetBalModel();
		EnOceanActuatorModel* GetModel();
#endif

	private:
		// METHODES
		void Run();
		/*
		 * Methode permettant de lire le fichier xml qui contient toutes les
		 * correspondances metric/room <-> id virtuelle afin de remplir la
		 * structure qui les contient
		 */
		void parserXML(const std::string a_sXmlFile);
		static void *callback(void *cxt)
		{
			((ActuatorsCenter*)cxt)->Run();
			return 0;
		}

		/*
		 * Methode permettant de retrouver dans la structure des correspondances
		 * metric/room <-> id virtuelle l'id virtuelle du capteur qui correspond
		 * a la metrique "metric" et la piece "room" passes en parametres
		 */
		int findVirtualId(int a_iMetric, int a_iRoom);

		// ATTRIBUTS
		int m_iBalServer;
		int m_iBalModel;
		pthread_t m_pThread;
		mapActuators m_pActuators;
		EnOceanActuatorModel *m_pModel;
};

#endif
