#include <stdlib.h>
#include <stdio.h>

#include "config.h"

#ifdef TESTING
#include "gmem/tests.h"
#include "monitoring/tests/tests.h"
#include "test/test.h"
#endif

int main()
{
#ifdef TESTING
	Test* test = new Test();

	InferenceTests(test);

	return test->run();
#else
	//========================================
	// PHASE D'INITIALISATION DE LA TACHE MERE
	//========================================

	cout << "Test" << endl;

	int sensorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );


	//Creation des taches filles

	//server_pere papa(sensorServerBox, actuatorServerBox);
	//sensor_box_reader sb(sensorServerBox, actuatorServerBox);
	ghome_database::open_database();


	//=====================================
	//PHASE DE DESTRUCTION DE LA TACHE MERE
	//=====================================

	//sb.wait(); //Attente de fermeture du serveur





	//Destruction des ressources

	msgctl(sensorServerBox,IPC_RMID,0);
	msgctl(actuatorServerBox,IPC_RMID,0);


	//Terminaison de la t�che m�re avec une autodestruction
	pthread_exit(0);

	return 0;
#endif
}
