#include <stdlib.h>
#include <stdio.h>
#include "ServerPere/GhomeDatabase.h"
#include "Sensors/SensorsCenter.h"
#include "config.h"
#include "tache_mere.h"

#ifdef TESTING
#include "tests.h"
#endif

int main()
{
#ifdef TESTING
	Test* test = new Test();

	InferenceTests(test);
	GmemTests(test);
	//ServerPereTests(test);

	return test->run();
#else
	//========================================
	// PHASE D'INITIALISATION DE LA TACHE MERE
	//========================================

	int sensorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	//Creation des taches filles

	/*ServerPere * papa = new ServerPere(sensorServerBox, actuatorServerBox);
	papa->Start();*/

	SensorBoxReader sb(sensorServerBox, actuatorServerBox);

	SensorsCenter *center = new SensorsCenter(sensorServerBox, "src/etc/sensors.xml");
	center->Start();

	//ghome_database::open_database();

	//=====================================
	//PHASE DE DESTRUCTION DE LA TACHE MERE
	//=====================================


	sb.Wait();
	//papa->Wait();
	//delete papa;
	center->Stop();
	delete center;

	//Destruction des ressources
	msgctl(sensorServerBox,IPC_RMID,0);
	msgctl(actuatorServerBox,IPC_RMID,0);

	//Terminaison de la tâche mère avec une autodestruction
	pthread_exit(0);

	return 0;
#endif
}

/*int main () {
	ghome_database* log_ecriture = new ghome_database("localhost", "root", "mysql", "GHOME");
	log_ecriture->open_database();
	log_ecriture->add_tuple("log", 1, 21, 42);

	return 0;
}

*/
