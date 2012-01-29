/*
 * write_message.h
 *
 *  Created on: 24 janv. 2012
 *      Author: remi
 */

#ifndef GHOME_BOX_H_
#define GHOME_BOX_H_
#define MSGTXTLEN 255


struct msg_buf {
  long mtype;
  char mtext[MSGTXTLEN];
}  ;


class ghome_box {
public:
	ghome_box();
	virtual ~ghome_box();
    static void send_actuator_box(int box,int typeMes, int metric, int room, int value);
    static void receive_message(int box);

private :

};

#endif /* GHOME_BOX_H_ */
