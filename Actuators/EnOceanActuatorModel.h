#ifndef ENOCEANACTUATORMODEL_H_
#define ENOCEANACTUATORMODEL_H_

#include "AbstractModel.h"
#include "../Utils/DataContext.h"
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
	virtual void Run();

	/*
	 * M�thode permettant de lire le fichier xml qui contient toutes les
	 * correspondances id virtuelle <-> id physique afin de remplir la
	 * structure qui les contient
	 */
	void parserXml(std::string a_sXmlFile);

	/*
	 * M�thode permettant de retrouver dans la structure des correspondances
	 * l'id physique qui correspond � l'id virtuelle pass�e en param�tre
	 */
	std::string findId(int a_iVirtualId);

	/*
	 * Cette m�thode cr�e la trame � destination du capteur d'id physique id_physique
	 * correspondant � l'information� transmettre : value.
	 */
	void createOrder(std::string a_sPhysicalId, int a_iValue, balMessage &mess);

	std::string m_psEntete;
	std::string m_psActivate;
	std::string m_psDesactivate;
	std::string m_psStatus;
	std::string m_psChecksum;



	// -------------------------------- ATTRIBUTS --------------------------------

	mapActuatorsId m_actuatorsId;
	pthread_t m_threadNetwork;
	int m_iBalNetwork;
	};

#endif
