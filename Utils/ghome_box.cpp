/*
 * write_message.cpp
 *
 *  Created on: 24 janv. 2012
 *      Author: remi
 */

#include "ghome_box.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<sys/errno.h>
#include <sys/msg.h>

void ghome_box::receive_message(int box, int &typeMes, int &metric, int &room, int &value)
{
	int rc;
	int intMsg[4];
	struct msg_buf msg;
	bzero(msg.mtext, MSGTXTLEN);
	rc = msgrcv(box, &msg, sizeof(msg.mtext), 0, 0);
	if (rc < 0)
	{
		perror( strerror(errno) );
		printf("msgrcv failed, rc=%d\n", rc);

	}
	else
	{
		bzero(intMsg, sizeof(intMsg));

		memcpy (&intMsg[0], &msg.mtext, sizeof(intMsg[0]));
		memcpy ( &intMsg[1],&msg.mtext[4], sizeof(intMsg[1]));
		memcpy ( &intMsg[2],&msg.mtext[8], sizeof(intMsg[2]));
		memcpy ( &intMsg[3],&msg.mtext[12], sizeof(intMsg[3]));

		typeMes = intMsg[0];
		metric = intMsg[1];
		room = intMsg[2];
		value = intMsg[3];
	}
}

void ghome_box::send_actuator_box(int box,const int typeMes, const int metric, const int room, const int value ){
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
}
