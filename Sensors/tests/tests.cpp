#include "../../config.h"

#ifdef TESTING
#include "tests.h"
#include "../../tache_mere.h"

/*
 * Test la méthode de parsing de la classe SensorsCenter
 */
void SensorsCenterTestParsing(Test *test)
{
	int sensorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	// The parsing method is called inside the constructor of the SensorCenter
	SensorsCenter *center = new SensorsCenter(sensorServerBox, "src/Sensors/tests/fileTest1.xml"); //"src/Sensors/tests/fileTest1.xml"
	center->Start();

	mapSensors parsingResults = center->GetMapSensors();

	// Hand made construction of the reference collection based on the xml file
	mapSensors referenceMap;

	std::pair<int, std::pair<int,int> > node;

	node.first = 1;
	node.second.first = 1;
	node.second.second = 3;

	referenceMap.insert(referenceMap.begin(), node);

	node.first = 2;
	node.second.first = 4;
	node.second.second = 1;

	referenceMap.insert(referenceMap.begin(), node);

	test->assert(parsingResults == referenceMap,"The parsing of the xml file has been correctly done in SensorsCenter.");

	center->Stop();
	delete center;
	msgctl(sensorServerBox,IPC_RMID,0);
}

void SensorsCenterTestSendMessageServer(Test *test)
{
	test->assert(true,"The message has been correctly send to the server");
}

/*
 * Test la méthode de parsing de la classe enOceanSensorModel
 */
void SensorsModelTestParsing(Test *test)
{
	int sensorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	// The parsing method is called inside the constructor of the SensorCenter
	SensorsCenter *center = new SensorsCenter(sensorServerBox, "src/Sensors/tests/fileTest1.xml"); //"src/Sensors/tests/fileTest1.xml"
	center->Start();

	mapSensorInfo parsingResults = center->GetMapSensorInfo();

	// Hand made construction of the reference collection based on the xml file
	mapSensorInfo referenceMap;

	std::pair<string, SensorInfo > node;

	SensorInfo info;

	info.iVirtualId = 1;
	info.iValid = 8;
	info.iType=SensorInfo::NUMERIC;
	info.iPosData = 4;
	info.iLengthData = 2;
	info.iMin = 40;
	info.iMax = 0;

	node.first = "00893386";
	node.second = info;

	referenceMap.insert(referenceMap.begin(), node);

	info.iVirtualId = 2;
	info.iValid = 8;
	info.iType = SensorInfo::BINARY;
	info.iPosData = 6;
	info.iLengthData = 2;
	info.iMin = 0;
	info.iMax = 0;
	info.mapValue.insert(info.mapValue.begin(), pair<int,int>(9,0));
	info.mapValue.insert(info.mapValue.begin(), pair<int,int>(8,1));

	node.first = "0001B596";
	node.second = info;

	referenceMap.insert(referenceMap.begin(), node);

	bool testOk = true;

	mapSensorInfo::iterator it1;
	for(mapSensorInfo::iterator it = referenceMap.begin(); it != referenceMap.end(); it++)
	{
		it1 = parsingResults.find(it->first);
		if(it1 != parsingResults.end())
		{
			if(!(it->second == it1->second))
			{
				testOk &= false;
			}
		}
		else
		{
			testOk &= false;
		}
	}

	test->assert(testOk,"The parsing of the xml file has been correctly done in SensorsModel.");

	center->Stop();
	delete center;
	msgctl(sensorServerBox,IPC_RMID,0);
}

void SensorsModelTestSendMessageCenter(Test *test)
{
	test->assert(true,"The message has been correctly send to the SensorCenter");
}

void SensorsTests(Test *test)
{
	test->add(&SensorsCenterTestParsing, "SensorCenter");
	test->add(&SensorsCenterTestSendMessageServer, "SensorCenter");
	test->add(&SensorsModelTestParsing, "SensorModel");
	test->add(&SensorsModelTestSendMessageCenter, "SensorModel");
}

#endif
