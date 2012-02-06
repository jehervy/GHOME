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
	GhomeDatabase* pLogSys = new GhomeDatabase("localhost", "boby", "ghome", "GHOME");
	pLogSys->OpenDatabase();
	pthread_mutex_lock (&myMutex);
	pLogSys->AddTuple("system_logs", a_itype, a_sMessage);
	pthread_mutex_unlock (&myMutex);
	pLogSys->CloseDatabase();
	return 0;
}

SystemLog::SystemLog() {
	// TODO Auto-generated constructor stub

}

SystemLog::~SystemLog() {
	// TODO Auto-generated destructor stub
}
