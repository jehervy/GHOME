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
	ghome_box(){};
	virtual ~ghome_box(){};
    static bool send_actuator_box(int box,const int typeMes, const int metric, const int room, const int value);
    static bool receive_message(int box, int &typeMes, int &metric, int &room, int &value);

    static bool send_message(int box, const int int1, const int int2);
    static bool receive_message(int box, int &int1, int &int2);

private :

};

#endif /* GHOME_BOX_H_ */
