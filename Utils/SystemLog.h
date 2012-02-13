/*
 * SystemLog.h
 *
 *  Created on: 6 févr. 2012
 *      Author: Pierre
 */

#ifndef SYSTEMLOG_H_
#define SYSTEMLOG_H_

#include <pthread.h>
#include <string>


static pthread_mutex_t myMutex;

class SystemLog {
public:
	SystemLog();
	//Ajoute le message a_sMessage associé au type a_itype à la table system_logs
	static int AddLog(int a_itype, std::string a_sMessage);
	virtual ~SystemLog();
	static const int  ERROR = 0;
	static const int SUCCESS = 1;
};

#endif /* SYSTEMLOG_H_ */
