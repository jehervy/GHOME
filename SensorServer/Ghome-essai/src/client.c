#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "client.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    in_addr_t address_inaddr;

    char* id_mess = "";
    char buffer[28+1];
    buffer[28] = '\0';

    if(argc!=3)
    {
    	printf("ERROR usage : client [ip] [port]\n");
    }

    char* address = argv[1];
    address_inaddr = inet_addr(address);

    printf("Client launched.\n");
    portno = atoi(argv[2]);

    printf("Trying to create socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    else
    	printf("socket openned\n");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(&address_inaddr,
         (char *)&serv_addr.sin_addr.s_addr,
         sizeof(address_inaddr));

    serv_addr.sin_port = htons(portno);

    printf("Trying to connect to server...\n");
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    else
    	printf("Connected to server\n");

    printf("Trying to read data\n");
    while(read(sockfd,buffer,28)>=0)
    {
    	char* capteur = "";

        id_mess = get_id(buffer);
        printf("id du capteur : %s\n", id_mess);

        if(strcmp(id_mess,"00893386") == 0)
        	capteur = "Température";
        if(strcmp(id_mess,"0001B596") == 0)
        	capteur = "Fenetre 1";
        if(strcmp(id_mess,"0001B595") == 0)
        	capteur = "Fenetre 2";
        if(strcmp(id_mess,"0021CC07")== 0)
        	capteur = "Interrupteur";
        if(strcmp(id_mess,"00053E7B") == 0)
        	capteur = "Luminosité";
        if(strcmp(id_mess,"10020024") == 0)
        	capteur = "Actionneur";
        if(capteur != "")
        	printf("message recu !\nCapteur %s, data : %s\n\n",capteur, get_data(buffer));
        else
        	printf("pas notre capteur...\n");
    }
    if (n < 0)
         error("ERROR reading from socket");


    close(sockfd);
    return 0;
}

char* get_id(char data[29]){
	char* res = malloc(9*sizeof(char));
	res[8] = '\0';
	strncpy(res,data+16,8);
	return res;
}

char* get_data(char data[29]){
	char* res = malloc(9*sizeof(char));
	res[8] = '\0';
	strncpy(res,data+8,8);
	return res;
}
