#ifndef ENOCEANACTUATORMODEL_H_
#define ENOCEANACTUATORMODEL_H_

#include "AbstractModel.h"
#include "../Utils/DataContext.h"
#include <map>
#include <string>
#include <utility>
#include "../xml/pugixml.hpp"
#include "../config.h"

/*
 * Structure contenant l'ensemble des correspondances id virtuelle <-> id physique
 */
typedef std::map<int,const std::string> mapActuatorsId;

typedef struct {
	std::string m_psEntete;
	std::string m_psActivate;
	std::string m_psDesactivate;
	std::string m_psStatus;
	std::string m_psChecksum;
} infoTrame;

class EnOceanActuatorModel : public AbstractModel
{

public:
	// -------------------------------- METHODES --------------------------------

	EnOceanActuatorModel(int a_iBal);
	virtual ~EnOceanActuatorModel();
	virtual void Start();
	virtual void Stop();

#ifdef TESTING
	mapActuatorsId GetActuatorsId();
	infoTrame GetInfoTrame();
	std::pair<int,int> GetOrdre();
	void SetBalNetwork(int *);
	std::string orderSent;
#endif

private:
	// -------------------------------- METHODES --------------------------------

	virtual void Run();
	/*
	 * Methode permettant de lire le fichier xml qui contient toutes les
	 * correspondances id virtuelle <-> id physique afin de remplir la
	 * structure qui les contient
	 */
	void parserXml(std::string a_sXmlFile);

	/*
	 * Methode permettant de retrouver dans la structure des correspondances
	 * l'id physique qui correspond a l'id virtuelle passee en parametre
	 */
	std::string findId(int a_iVirtualId);

	/*
	 * Cette methode cree la trame a destination du capteur d'id physique id_physique
	 * correspondant a l'information a transmettre : value.
	 */
	void createOrder(std::string a_sPhysicalId, int a_iValue, balMessage &mess);

	infoTrame p_myInfoTrame;


	// -------------------------------- ATTRIBUTS --------------------------------

	mapActuatorsId m_actuatorsId;
	pthread_t m_threadNetwork;
	int m_iBalNetwork;
	};

#endif
