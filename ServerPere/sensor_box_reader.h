/*
 * sensor_box_reader.h
 *
 *  Created on: 22 janv. 2012
 *      Author: Pierre
 */


#include <pthread.h>
#include <sys/msg.h>	//pour la boite aux lettres
#include <mysql.h> //acces BDD

#include "ghome_database.h"

#ifndef SENSORBOXREADER_H_
#define SENSORBOXREADER_H_
#define MSGTXTLEN 255

struct msg_buf1 {
  long mtype;
  char mtext[MSGTXTLEN];
}  ;



class sensor_box_reader {
public:
	sensor_box_reader(int sensorServerBox, int actuatorServerBox);
	int open_thread_sensor_box_reader();
	void wait();
	static void *init(void * ptr);
	int run();
	virtual ~sensor_box_reader();
	int id, metric, room, value;
private :
	int p_sensorServerBox;
	int p_actuatorServerBox;
	pthread_t thread_sensor_box_reader;
	struct msg_buf1 msg;


};

#endif /* SENSORBOXREADER_H_ */
