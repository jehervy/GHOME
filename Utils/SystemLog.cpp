/*
 * SystemLog.cpp
 *
 *  Created on: 6 févr. 2012
 *      Author: Pierre
 */

#include "SystemLog.h"
#include "../ServerPere/GhomeDatabase.h"

int SystemLog::AddLog(int a_itype, string a_sMessage)
{
	pthread_mutex_lock (&myMutex);
	GhomeDatabase* pLogSys = new GhomeDatabase("localhost", "boby", "ghome", "GHOME");
	if (pLogSys->OpenDatabase() != 0)
	{
		pLogSys->CloseDatabase();
		delete pLogSys;
		pthread_mutex_unlock (&myMutex);
		return -1;
	}

	if (pLogSys->AddTuple("system_logs", a_itype, a_sMessage) != 0)
	{
		pLogSys->CloseDatabase();
		delete pLogSys;
		pthread_mutex_unlock (&myMutex);
		return -1;
	}
	pLogSys->CloseDatabase();
	delete pLogSys;
	pthread_mutex_unlock (&myMutex);
	return 0;
}

SystemLog::SystemLog() {
	// TODO Auto-generated constructor stub

}

SystemLog::~SystemLog() {
	// TODO Auto-generated destructor stub
}
