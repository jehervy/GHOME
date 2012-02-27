/*
 * EnOceanSensorModel.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef ENOCEANSENSORMODEL_H_
#define ENOCEANSENSORMODEL_H_

#include "AbstractModel.h"
#include <map>
#include <string>

/*
 * Structure contenant les informations permettant de décrypter les données d'un trame issues
 * d'un capteur
 */
struct SensorInfo{
	enum Type { NUMERIC, BINARY };

	int iVirtualId;
	Type iType;
	int iValid;
	int iPosData;
	int iLengthData;
	int iMin;
	int iMax;
	std::map<int, int> mapValue;
#ifdef TESTING
	bool operator==(const SensorInfo &info)
	{
		return ((info.iVirtualId == iVirtualId) &&
				(info.iType == iType) &&
				(info.iValid == iValid) &&
				(info.iPosData == iPosData) &&
				(info.iLengthData == iLengthData) &&
				(info.iMin == iMin) &&
				(info.iMax == iMax) &&
				(info.mapValue == mapValue));
	}
#endif
};

/*
 * Map mettant en relation idPhysique et information sur le capteur
 */
typedef std::multimap<std::string, SensorInfo> mapSensorInfo;

/*
 * Classe EnOceanSensorModel permet de décrypter les trames issues des capteurs enOcean
 * Elle implémente la classe abstraite AbstractModel
 */
class EnOceanSensorModel : public AbstractModel{

public:
	/*
	 * Constructeur
	 * Prend en paramètre la boîte aux lettre du SensorsCenter dans laquelle sera placé le message décrypté
	 */
	EnOceanSensorModel(int a_iBal);

	/*
	 * Destructeur
	 */
	virtual ~EnOceanSensorModel();

	/*
	 * Méthode permettant de créer la boîte aux lettres à associer au DataContext
	 * et de lancer le thread du DataContext
	 */
	virtual void Start();

	/*
	 * Méthode permettant de supprimer la boîte aux lettre associée au DataContext
	 * et de supprimer le thread du DataContext
	 */
	virtual void Stop();

#ifdef TESTING
	mapSensorInfo GetMapSensorInfo(){return m_sensorInfo;}
#endif

private:

	/*
	 * Thread principal du model, récupère un message dans la boîte aux lettre du DataContext, le traite
	 * et le place dans la boîte aux lettres du SensorsCenter
	 */
	virtual void Run();

	/*
	 * Méthode permettant de parser le fichier xml de correspondance idPhysique <-> informations capteur
	 */
	void ParserXml(const char *a_XmlFile);

	mapSensorInfo m_sensorInfo;
	pthread_t m_threadNetwork;
	int m_iBalNetwork;

};

#endif /* ENOCEANSENSORMODEL_H_ */
