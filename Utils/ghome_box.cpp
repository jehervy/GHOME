/*
 * write_message.cpp
 *
 *  Created on: 24 janv. 2012
 *      Author: remi
 */

#include "ghome_box.h"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<sys/errno.h>
#include <sys/msg.h>
//#include <sqlite3.h>

ghome_box::ghome_box()
		 {



}

void ghome_box::receive_message(int box)
{
	int rc;
	int intMsg[4];
	struct msg_buf msg;
	bzero(msg.mtext, MSGTXTLEN);
	 rc = msgrcv(box, &msg, sizeof(msg.mtext), 0, 0);
			  if (rc < 0)
			  {
			    perror( strerror(errno) );
			    cout << errno;
			    printf("msgrcv failed, rc=%d\n", rc);

			  }
			  else
			  {
				  cout << "Message recu"<<endl;
				  bzero(intMsg, sizeof(intMsg));

					  memcpy (&intMsg[0], &msg.mtext, sizeof(intMsg[0]));
					  memcpy ( &intMsg[1],&msg.mtext[4], sizeof(intMsg[1]));
					  memcpy ( &intMsg[2],&msg.mtext[8], sizeof(intMsg[2]));
					  memcpy ( &intMsg[3],&msg.mtext[12], sizeof(intMsg[3]));

//					 sensor_box_reader::id=intMsg[0];
//					 sensor_box_reader::metric=intMsg[1];
//					 sensor_box_reader::room=intMsg[2];
//					 sensor_box_reader::value=intMsg[3];




						cout << intMsg[0] << endl;
						cout << intMsg[1] << endl;
						cout << intMsg[2] << endl;
						cout << intMsg[3] << endl;

			  }
}

void ghome_box::send_actuator_box(int box,int typeMes, int metric, int room, int value ){
	struct msg_buf msg;
	bzero(msg.mtext, MSGTXTLEN);
	int rc;
		int intMsg[4];
		msg.mtype = 1; // set the type of message
			  intMsg[0] = typeMes;
			  intMsg[1] = metric;
			  intMsg[2] = room;
			  intMsg[3] = value;

			  memcpy (&msg.mtext, &intMsg[0], sizeof(intMsg[0]));
			  memcpy (&msg.mtext[4], &intMsg[1], sizeof(intMsg[1]));
			  memcpy (&msg.mtext[8], &intMsg[2], sizeof(intMsg[2]));
			  memcpy (&msg.mtext[12], &intMsg[3], sizeof(intMsg[3]));


			  // send the message to queue
			  rc = msgsnd(box, &msg, sizeof(msg.mtext), 0); // the last param can be: 0, IPC_NOWAIT, MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR.
			  if (rc < 0)
			  {
			    perror( strerror(errno) );
			    printf("msgsnd failed, rc = %d", rc);

			  }
			  else
			  {
				  cout << "Message envoy�"<<endl;

			  }
}

ghome_box::~ghome_box() {
	// TODO Auto-generated destructor stub
}

