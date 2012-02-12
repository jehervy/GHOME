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


class GhomeBox {
public:
	GhomeBox(){};
		/*
	 * Constructeur
	 */
	virtual ~GhomeBox(){};
		/*
	 * Destructeur
	 */
    static bool SendActuatorBox(int box,const int typeMes, const int metric, const int room, const int value);
        /*
     * Place un nouveau message dans une file de message
     * passee en parametre
     */
    static bool ReceiveMessage(int box, int &typeMes, int &metric, int &room, int &value);
      /*
     * Lis le premier message dans une file de message
     * passee en parametre
     */

    static bool SendMessage(int box, const int int1, const int int2);
    static bool ReceiveMessage(int box, int &int1, int &int2);

private :

};

#endif /* GHOME_BOX_H_ */
