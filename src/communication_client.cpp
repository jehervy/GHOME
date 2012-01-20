/*
 * communicationclient.cpp
 *
 *  Created on: 17 janv. 2012
 *      Author: remi
 */

#include "communication_client.h"

communication_client::communication_client() {


}

communication_client::communication_client(int sensorServerBox, int actuatorServerBox, int fd) :
p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox), p_fd(fd){
transfer_message();

}

communication_client::~communication_client() {

}

void communication_client::transfer_message()
{
	cout << "On est dans la communication client" << endl;
	cout <<p_fd<<endl;
	   int n;
	   char buffer[256];
for(;;)
{
		   bzero(buffer,256);
		   n = read(p_fd,buffer,255);
		   if (n < 0)
		   {
		    //perror("ERROR reading from socket");
		   }
		   if (n > 0)
		   {
		    printf("Here is the message: %s\n",buffer);
		   }
		   n = write(p_fd,"I got your message",18);
		   if (n < 0)
		   {
		    //perror("ERROR writing to socket");
		   }
}


}



