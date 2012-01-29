/*
 * sensor_box_reader.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: Pierre
 */

#include "sensor_box_reader.h"
#include "../Utils/ghome_box.h"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<sys/errno.h>

void perror(const char *s);


extern int errno ;



sensor_box_reader::~sensor_box_reader() {
	// TODO Auto-generated destructor stub
}


sensor_box_reader::sensor_box_reader(int sensorServerBox, int actuatorServerBox):
		p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox)
{
		cout << "id boite " << sensorServerBox << endl;
		cout << "Appel constructeur"<<endl;

		open_thread_sensor_box_reader();
}



int sensor_box_reader::run()
{
	cout << "Debut du run" << endl;



	  // message to send (test) TODO : virer
	ghome_box::send_actuator_box(p_sensorServerBox,2,23,45,12);


		  // read the message from queue

	  for(;;)
	  {
		  ghome_box::receive_message(p_sensorServerBox, id, metric, room, value);

		  if(id==1) //C'est un message de type information
		  {
		  	// TODO : base de donnee

		  } else if(id==2) { //C'est un message de type pilotage
		  	// TODO : base de donnee et actuatorServerBox
		  ghome_box::send_actuator_box(p_actuatorServerBox, id,metric,room,value);
		  					  }
				//msgctl(sensorServerBox,IPC_RMID,0);
		  }

	//}
		return 0;
}


void *sensor_box_reader::init(void * ptr)
{
	sensor_box_reader* p = (sensor_box_reader*)ptr;
	p->run();
	return (0);
}




int sensor_box_reader::open_thread_sensor_box_reader()
{
	cout << "On est dans open_thread_sensor_box_reader"<<endl;
	int check = pthread_create(&thread_sensor_box_reader, NULL, &sensor_box_reader::init, this);
	return check;
}

void sensor_box_reader::wait()
{
	pthread_join (thread_sensor_box_reader, NULL);
}
