#include "../../config.h"

#ifdef TESTING
#include "tests.h"
#include "../../tache_mere.h"

/*
 * This function tests the result of the xml file parsing method in ActuatorsCenter.
 * The result is compared to a handmade collection containing the actual information
 * stashed in the xml file
 */
void ActuatorsTestParsingXmlCenter(Test *test)
{
	bool passed = true;
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	GhomeBox::SendActuatorBox(actuatorServerBox, 2, 1,3,1);

	// The parsing method is called inside the constructor of the ActuatorCenter
	ActuatorsCenter *actuatorsCenter = new ActuatorsCenter(actuatorServerBox, "etc/actuators.xml");
	mapActuators parsingResults = actuatorsCenter->GetActuators();

	// Hand made construction of the reference collection based on the xml file
	mapActuators referenceMap;

	std::pair<std::pair<int,int>,int>  node;
	(node.first).first = 1;
	(node.first).second = 3;
	node.second = 1;
	referenceMap.insert(node);

	/*
	 * Comparison node by node between the two collection :
	 * 1. if something in the parsing result is not in the reference : error
	 * 2. if something in the reference does not appear in the parsing result : error
	 */
	mapActuators::iterator it = referenceMap.begin();
	while(passed && it != referenceMap.end())
	{
		if (parsingResults.find(it->first) != parsingResults.end()
				&& parsingResults.find(it->first)->second == it->second)
			parsingResults.erase(it->first);
		else
			passed = false;
		it++;
	}

	if(parsingResults.size()!=0)
		passed = false;

	test->assert(passed,"The parsing of the xml file has been correctly done in ActuatorsCenter.");

	actuatorsCenter->Stop();
	delete actuatorsCenter;
}

/*
 * This function tests the result of the xml file parsing method in EnOceanctuatorModel.
 * so far the application has t handle just one actuator, so the parsing will be tested
 * this single device. The info in the xml file is :
 * 	entete : A55A6B05
	activate: 50000000
	desactivate: 70000000
    <actuator>
        <virtualId>1</virtualId>
        <physicalId>FF9F1E07</physicalId>
    </actuator>
    status: 10
    checksum: 00
 */

void ActuatorsTestParsingXmlModel(Test *test)
{
	bool res = true;
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	/*
	 * The parsing method is called inside the constructor of the ActuatorModel,
	 * himself initialized by the construtor of the ActuatorsCenter
	 */
	ActuatorsCenter *actuatorsCenter = new ActuatorsCenter(actuatorServerBox, "etc/actuators.xml");

	/*
	 * Comparison between parsing result and xml info :
	 * Step 1 : common info about trames in infoTrame must match the info contained in the xml file
	 * Step 2 : the matching between virtual id and physical id must be listed in the model's collection
	 */

	// STEP 1
	infoTrame infoTrame = actuatorsCenter->GetModel()->GetInfoTrame();
	if(infoTrame.m_psEntete != "A55A6B05" || infoTrame.m_psActivate != "50000000" || infoTrame.m_psDesactivate != "70000000"
			|| infoTrame.m_psStatus != "10" || infoTrame.m_psChecksum != "00")
		res=false;

	//STEP 2
	mapActuatorsId parsingResults = actuatorsCenter->GetModel()->GetActuatorsId();

	mapActuatorsId referenceMap;
	referenceMap.insert(std::pair<int,const std::string> (1,"FF9F1E07"));

	if (res==true)
	{
		for (mapActuatorsId::iterator it= referenceMap.begin(); it != referenceMap.end() ; ++it)
		{
			if(parsingResults.find(it->first) != parsingResults.end())
				parsingResults.erase(it->first);
		}
		if (parsingResults.size()!=0)
			res = false;
	}
	test->assert(res,"The parsing of the xml file has been correctly done in ActuatorModel.");
	actuatorsCenter->Stop();
	delete actuatorsCenter;
}

/*
 * This function must check if the info contained in the message received in the actuatorBalServer
 * and transferred to the model bal by the actuator center has not been changed during transfer.
 * To do so, the model will keep a trace of the last received message in the model bal
 * in a private attribute called "p_order". We just have to check if both the id and the value
 * in the message received by the model match the info sent at the beginning in the actuatorBalServer.
 */
void ActuatorsTestServerToActuatorTransfer(Test *test)
{
	char* correctOrder=(char*) malloc(28*sizeof(char));
	int balNetwork = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	/*
	 * Since we only use one actuator, we are forced to put the metric id and the room id at respectively 1 and 3.
	 * The value however can be chosen randomly. By choosing 1 and 3 for the metric and the id, we force the actuator
	 * to be the one identified by 1 (virtual id)
	 */

	/* generate random value between 0 and 1: */
	int randValue = (rand() * 100)%2;


	// The parsing method is called inside the constructor of the ActuatorCenter
	ActuatorsCenter *actuatorsCenter = new ActuatorsCenter(actuatorServerBox, "etc/actuators.xml",&balNetwork);
	actuatorsCenter->Start();

	GhomeBox::SendActuatorBox(actuatorServerBox,2,1,3,randValue);

	if(randValue == 0)
		correctOrder = (char*) "A55A6B0570000000FF9F1E071000";
	else
		correctOrder = (char*) "A55A6B0550000000FF9F1E071000";

	sleep(1);

	test->assert(strcmp(actuatorsCenter->GetModel()->orderSent.c_str(), correctOrder)==0,"The correct info has been transferred from actuatorBalServer to model bal") ;

	actuatorsCenter->Stop();
	delete actuatorsCenter;
}

/*
 * There are two type of messages that we use in this application so far : information ones, which are designed to
 * transmit information sent by the sensors; and the orders, that must be sent to actuators.
 * This function tests if the actuator center ignores the information messages that might be in the actuatorBalServer
 * though that situation should never happen.
 */

void ActuatorsTestIgnoreNonOrders(Test *test)
{
	balMessage msg;
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	ActuatorsCenter *actuatorsCenter = new ActuatorsCenter(actuatorServerBox, "etc/actuators.xml");
	actuatorsCenter->Start();

	// Stop the model so it won't take the message in the model bal
	actuatorsCenter->GetModel()->Stop();

	// Posting a non order in the actuatorServerBox
	GhomeBox::SendActuatorBox(actuatorServerBox,1,1,3,1);

	// Checking if the model bal did get a message from Center
	test->assert(msgrcv(actuatorsCenter->GetBalModel(), &msg,sizeof(msg.mtext), 0, IPC_NOWAIT) == -1, "The non order message has correctly been ignored by the actuators center.");

	actuatorsCenter->Stop();
	delete actuatorsCenter;
}

void ActuatorsTests(Test *test)
{
	test->add(&ActuatorsTestParsingXmlCenter, "ActuatorsTestParsingXmlCenter");
	test->add(&ActuatorsTestParsingXmlModel, "ActuatorsTestParsingXmlModel");
	test->add(&ActuatorsTestServerToActuatorTransfer, "ActuatorsTestServerToActuatorTransfer");
	test->add(&ActuatorsTestIgnoreNonOrders, "ActuatorsTestIgnoreNonOrders");
}

#endif
