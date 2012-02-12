#ifndef ENOCEANACTUATORMODEL_H_
#define ENOCEANACTUATORMODEL_H_

#include "AbstractModel.h"
#include "DataContext.h"
#include <map>
#include <string>
#include <utility>
#include "../xml/pugixml.hpp"

/*
 * Structure contenant l'ensemble des correspondances id virtuelle <-> id physique
 */
typedef std::map<int,const std::string> mapActuatorsId;

class EnOceanActuatorModel : public AbstractModel
{

public:
	// -------------------------------- METHODES --------------------------------

	EnOceanActuatorModel(int a_iBal);

	virtual void start();

	virtual void stop();

private:
	// -------------------------------- METHODES --------------------------------
	virtual void run();

	/*
	 * Méthode permettant de lire le fichier xml qui contient toutes les
	 * correspondances id virtuelle <-> id physique afin de remplir la
	 * structure qui les contient
	 */
	void parserXml(std::string a_sXmlFile);

	/*
	 * Méthode permettant de retrouver dans la structure des correspondances
	 * l'id physique qui correspond à l'id virtuelle passée en paramètre
	 */
	std::string findId(int a_iVirtualId);

	/*
	 * Cette méthode crée la trame à destination du capteur d'id physique id_physique
	 * correspondant à l'informationà transmettre : value.
	 */
	balMessage createOrder(std::string a_sPhysicalId, int a_iValue);


	// -------------------------------- ATTRIBUTS --------------------------------

	mapActuatorsId m_actuatorsId;
	pthread_t m_threadNetwork;
	int m_iBalNetwork;
	};

#endif
