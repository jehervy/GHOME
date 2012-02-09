/*
 * sensor_box_reader.h
 *
 *  Created on: 22 janv. 2012
 *      Author: Pierre
 */


#include <pthread.h>
#include <sys/msg.h>	//pour la boite aux lettres
//#include <mysql/mysql.h> //acces BDD

#include "GhomeDatabase.h"

#ifndef SENSORBOXREADER_H_
#define SENSORBOXREADER_H_
#define MSGTXTLEN 255

struct msg_buf1 {
  long lMtype;
  char cMtext[MSGTXTLEN];
}  ;



class SensorBoxReader {
public:
	SensorBoxReader(int a_iSensorServerBox, int a_iActuatorServerBox);
	int OpenThreadSensorBoxReader();
	void Wait();
	static void *sInit(void * a_pPtr);
	int Run();
	virtual ~SensorBoxReader();
private :
	int m_iSensorServerBox;
	int m_iActuatorServerBox;
	pthread_t m_threadSensorBoxReader;
	struct msg_buf1 m_msg;


};

#endif /* SENSORBOXREADER_H_ */
