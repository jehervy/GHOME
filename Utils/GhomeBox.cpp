/*
 * write_message.cpp
 *
 *  Created on: 24 janv. 2012
 *      Author: remi
 */

#include "GhomeBox.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<sys/errno.h>
#include <sys/msg.h>

bool GhomeBox::ReceiveMessage(int box, int &typeMes, int &metric, int &room, int &value)
/*
 * Lis le premier message de la file de message box.
 * Met a jour les donnees par passage par references.
 */
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
		return false;

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

		return true;
	}
}

bool GhomeBox::SendActuatorBox(int box,const int typeMes, const int metric, const int room, const int value )
/*
 * Place un message dans la file de message
 * passee en parametre
 */
{
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
		return false;

	}

	return true;
}

bool GhomeBox::SendMessage(int box, const int int1, const int int2)
{
	struct msg_buf msg;
	bzero(msg.mtext, MSGTXTLEN);
	int rc;
	int intMsg[2];
	msg.mtype = 1; // set the type of message

	intMsg[0] = int1;
	intMsg[1] = int2;

	memcpy (&msg.mtext, &intMsg[0], sizeof(intMsg[0]));
	memcpy (&msg.mtext[4], &intMsg[1], sizeof(intMsg[1]));

	// send the message to queue
	rc = msgsnd(box, &msg, sizeof(msg.mtext), 0 | MSG_NOERROR); // the last param can be: 0, IPC_NOWAIT, MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR.
	if (rc < 0)
	{
		perror( strerror(errno) );
		printf("msgsnd failed, rc = %d", rc);

		return false;

	}

	return true;
}

bool GhomeBox::ReceiveMessage(int box, int &int1, int &int2)
{
	int rc;
	int intMsg[2];
	struct msg_buf msg;
	bzero(msg.mtext, MSGTXTLEN);
	rc = msgrcv(box, &msg, sizeof(msg.mtext), 0, 0);
	if (rc < 0)
	{
		perror( strerror(errno) );
		printf("msgrcv failed, rc=%d\n", rc);

		return false;

	}
	else
	{
		bzero(intMsg, sizeof(intMsg));

		memcpy (&intMsg[0], &msg.mtext, sizeof(intMsg[0]));
		memcpy ( &intMsg[1],&msg.mtext[4], sizeof(intMsg[1]));

		int1 = intMsg[0];
		int2 = intMsg[1];

		return true;
	}
}
