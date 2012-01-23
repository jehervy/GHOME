/*
 * sensor_box_reader.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: Pierre
 */

#include "sensor_box_reader.h"
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
		msg.mtype = 1;
		bzero(msg.mtext, MSGTXTLEN);
		msg.mtext[0] = 'm';
		msg.mtext[1] = 's';
		msg.mtext[2] = 'g';
		cout << msg.mtext << endl;
		cout << "Appel open_thread_sensor_box_reader"<< endl;
		open_thread_sensor_box_reader();
}



int sensor_box_reader::run()
{
	cout << "Debut du run" << endl;
	int rc;
	bzero(msg.mtext, MSGTXTLEN);

	int intMsg[4];

	//for(;;)
	//{
	  // message to send
	  msg.mtype = 1; // set the type of message
	  intMsg[0] = 1;
	  intMsg[1] = 23;
	  intMsg[2] = 0;
	  intMsg[3] = 15;

	  memcpy (&msg.mtext, &intMsg[0], sizeof(intMsg[0]));
	  memcpy (&msg.mtext[4], &intMsg[1], sizeof(intMsg[1]));
	  memcpy (&msg.mtext[8], &intMsg[2], sizeof(intMsg[2]));
	  memcpy (&msg.mtext[12], &intMsg[3], sizeof(intMsg[3]));


	  // send the message to queue
	  rc = msgsnd(p_sensorServerBox, &msg, sizeof(msg.mtext), 0); // the last param can be: 0, IPC_NOWAIT, MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR.
	  if (rc < 0)
	  {
	    perror( strerror(errno) );
	    printf("msgsnd failed, rc = %d", rc);
	    return 1;
	  }
	  else
	  {
		  cout << "Message envoyé";

	  }


		  // read the message from queue
		  rc = msgrcv(p_sensorServerBox, &msg, sizeof(msg.mtext), 0, 0);
		  if (rc < 0)
		  {
		    perror( strerror(errno) );
		    cout << errno;
		    printf("msgrcv failed, rc=%d\n", rc);
		    return 1;
		  }
		  else
		  {
			  cout << "Message recu";
			  bzero(intMsg, sizeof(intMsg));

				  memcpy (&intMsg[0], &msg.mtext, sizeof(intMsg[0]));
				  memcpy ( &intMsg[1],&msg.mtext[4], sizeof(intMsg[1]));
				  memcpy ( &intMsg[2],&msg.mtext[8], sizeof(intMsg[2]));
				  memcpy ( &intMsg[3],&msg.mtext[12], sizeof(intMsg[3]));


					cout << intMsg[0] << endl;
					cout << intMsg[1] << endl;
					cout << intMsg[2] << endl;
					cout << intMsg[3] << endl;


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

